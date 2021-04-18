#include <include/NtupleObj.h>

#include <TChain.h>
#include <TSystemDirectory.h>
#include <TSystemFile.h>
#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TEfficiency.h>
#include <TStyle.h>

#include <iostream>
#include <algorithm>
#include <numeric>
#include <map>

std::map<string, double> variables;
std::vector< pair <string, string> > variables2D;
std::map<string, string> labels;
float llp_dR = 0;
TLorentzVector llp_0_p4;

namespace DVeff {

   class LLP {
   public:
      TVector3 decay;
      TLorentzVector p4;
      int pid;
   };

   class Descendent {
   public:
      float charge;
      int pid;
      TLorentzVector p4;
      TVector3 prod;
      TVector3 decay;
      char isReconstructed;
      char isSelected;
      size_t truthPosIndex;
   };

   class TruthPos {
   public:
      TVector3 pos;
      TLorentzVector reconstructibleSumP4;
      size_t nReconstructible;
   };

   class RecoVtx {
   public:
      size_t truthPosIndex;
      TVector3 pos;
      TVector3 residual;
      float chi2;
      float chi2_core;
      float mass;
      float massFraction;
      float mass_nonAssociated;
      float matchScore;
      float multiplicityFraction;
      float filtered_mass;
      float filtered_mass_nonAssociated;
      size_t ntrk;
      size_t ntrk_nonAssociated;
      size_t filtered_ntrk;
      size_t filtered_ntrk_nonAssociated;
   };

   class DVeffVtx {
   public:
      LLP llp;
      TLorentzVector descendentsSumP4;
      std::vector<Descendent> descendents;
      std::vector<TruthPos>   truthPositions;
      std::vector<RecoVtx>    recoVertices;

      void print(size_t index);
   };


}


class Stats {
public:
   Stats();
   size_t nEvents;
   size_t nLLPs;
   size_t nLLPs_hasTruthPos;
   size_t nLLPs_hasReco;
   size_t nLLPs_hasMoreThanOneRecos;
};

Stats::Stats()
   : nEvents(0), nLLPs(0), nLLPs_hasTruthPos(0), nLLPs_hasReco(0), nLLPs_hasMoreThanOneRecos(0)
{}



void setup(DVeff::DVeffVtx& v, DVeffObj& dveff, const size_t i_llp);
void process(DVeffObj& dveff, PVObj& pv, const size_t i_llp, Stats& stats);



std::map<std::string, TObject*> histos;

using EffHistPair = std::pair<std::string, std::string>;

std::map<std::string, EffHistPair> effs;

auto all           = [](const DVeff::DVeffVtx& /*v*/) -> bool {
   return true;
};

auto hasRecoVertex = [](const DVeff::DVeffVtx& v) -> bool {
   return v.recoVertices.size() > 0;
};

auto hasAtLeast2ReconstructedDescendents = [](const DVeff::DVeffVtx& v) -> bool {
   return std::count_if(v.descendents.begin(), v.descendents.end(), [](const DVeff::Descendent & d)
   {
      return d.isReconstructed != 0;
   }) >= 2;
};

auto hasAtLeast2SelectedDescendents = [](const DVeff::DVeffVtx& v) -> bool {
   return std::count_if(v.descendents.begin(), v.descendents.end(), [](const DVeff::Descendent & d)
   {
      return d.isSelected != 0;
   }) >= 2;
};

template<class HISTO, typename ...REST>
void addHist(const std::string name, const std::string title, REST ...rest)
{
   auto* h = new HISTO(name.c_str(), title.c_str(), rest...);
   histos[name] = h;
}

template<class HISTO, typename ...REST>
void fillHist(const std::string name, REST ...rest)
{
   if (histos.find(name) == histos.end()) {
      std::string err = Form("%s: %s not found in the histogram list!!", __PRETTY_FUNCTION__, name.c_str());
      std::cerr << err << std::endl;
      throw std::runtime_error(err);
   }
   (dynamic_cast<HISTO*>(histos[name]))->Fill(rest...);
}

template<class HISTO, typename ...REST>
void fillEff(const std::string name, bool denom_condition, bool numer_condition, REST ...rest)
{
   if (effs.find(name) == effs.end()) {
      std::string err = Form("%s: %s not found in the efficiency list!!", __PRETTY_FUNCTION__, name.c_str());
      std::cerr << err << std::endl;
      throw std::runtime_error(err);
   }

   if (denom_condition) {
      fillHist<HISTO>(effs.at(name).first,  rest...);
      if (numer_condition) fillHist<HISTO>(effs.at(name).second, rest...);
   }
}


template<class HISTO>
void format(const std::string name) {}


template<>
void format<TH1F>(const std::string name)
{

   auto* denom = dynamic_cast<TH1F*>(histos[ effs[name].first ]);
   auto* numer = dynamic_cast<TH1F*>(histos[ effs[name].second ]);

   denom->Sumw2();
   numer->Sumw2();

   std::vector< pair <string, string> > eff_names;
   eff_names.push_back(make_pair("recoEff", "#varepsilon"));
   eff_names.push_back(make_pair("recoEff_algo", "#varepsilon_{algo}"));
   eff_names.push_back(make_pair("recoEff_core", "#varepsilon_{core}"));
   eff_names.push_back(make_pair("acceptance_recoEff_seed", "A*#varepsilon_{seed}"));
   eff_names.push_back(make_pair("acceptance", "A"));
   eff_names.push_back(make_pair("recoEff_seed", "#varepsilon_{seed}"));

   TEfficiency *heff = new TEfficiency(*numer, *denom);
   heff->SetName(name.c_str());
   for (auto pair : eff_names) {
      string eff_name = pair.first;
      string eff_title = pair.second;
      if (((TString)name).Contains(eff_name)) {
         TString labely = eff_title;
         heff->SetTitle(";" + (TString)denom->GetXaxis()->GetTitle() + ";" + labely);
      }
   }
   histos[ name ] = heff;
   for (int ibin = 0; ibin < denom->GetNbinsX(); ibin++) {
      auto width = denom->GetBinWidth(ibin + 1);
      denom->SetBinContent(ibin + 1, denom->GetBinContent(ibin + 1) / width);
      denom->SetBinError(ibin + 1, denom->GetBinError(ibin + 1) / width);
   }

   for (int ibin = 0; ibin < numer->GetNbinsX(); ibin++) {
      auto width = numer->GetBinWidth(ibin + 1);
      numer->SetBinContent(ibin + 1, numer->GetBinContent(ibin + 1) / width);
      numer->SetBinError(ibin + 1, numer->GetBinError(ibin + 1) / width);
   }

}

template<>
void format<TH2F>(const std::string name)
{

   auto* denom = dynamic_cast<TH2F*>(histos[ effs[name].first ]);
   auto* numer = dynamic_cast<TH2F*>(histos[ effs[name].second ]);

   denom->Sumw2();
   numer->Sumw2();

   std::vector< pair <string, string> > eff_names;
   eff_names.push_back(make_pair("recoEff", "#varepsilon"));
   eff_names.push_back(make_pair("recoEff_algo", "#varepsilon_{algo}"));
   eff_names.push_back(make_pair("recoEff_core", "#varepsilon_{core}"));
   eff_names.push_back(make_pair("acceptance_recoEff_seed", "A*#varepsilon_{seed}"));
   eff_names.push_back(make_pair("acceptance", "A"));
   eff_names.push_back(make_pair("recoEff_seed", "#varepsilon_{seed}"));

   for (auto pair : eff_names) {
      string eff_name = pair.first;
      string eff_title = pair.second;
      if (((TString)name).Contains(eff_name)) {
         TString labelz = eff_title;
         denom->GetZaxis()->SetTitle(labelz);
         numer->GetZaxis()->SetTitle(labelz);
      }
   }
   TEfficiency *heff = new TEfficiency(*numer, *denom);
   heff->SetTitle(";" + (TString)denom->GetXaxis()->GetTitle() + ";" + (TString)denom->GetYaxis()->GetTitle());
   heff->SetName(name.c_str());
   histos[ name ] = heff;
}

void formatEfficiencies(const std::string name)
{

   format<TH1F>("recoEff_" + name);
   format<TH1F>("recoEff_seed_" + name);
   format<TH1F>("recoEff_algo_" + name);
   format<TH1F>("recoEff_core_" + name);
   format<TH1F>("acceptance_" + name);
   format<TH1F>("acceptance_recoEff_seed_" + name);

}

void formatEfficiencies(const std::string name_x, const std::string name_y)
{

   string name2D = name_x + "_vs_" + name_y;
   format<TH2F>("recoEff_" + name2D);
   format<TH2F>("recoEff_seed_" + name2D);
   format<TH2F>("recoEff_algo_" + name2D);
   format<TH2F>("recoEff_core_" + name2D);
   format<TH2F>("acceptance_" + name2D);
   format<TH2F>("acceptance_recoEff_seed_" + name2D);
}

void defineEfficiencies(const std::string name, const std::string lab, const std::vector<double> bins)
{

   addHist<TH1F>("recoEff_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("recoEff_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   addHist<TH1F>("recoEff_seed_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("recoEff_seed_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   addHist<TH1F>("recoEff_algo_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("recoEff_algo_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   addHist<TH1F>("recoEff_core_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("recoEff_core_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   addHist<TH1F>("acceptance_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("acceptance_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   addHist<TH1F>("acceptance_recoEff_seed_" + name + "_denom", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));
   addHist<TH1F>("acceptance_recoEff_seed_" + name + "_numer", ";" + lab + ";Entries", bins.size() - 1, &(bins.at(0)));

   effs["recoEff_" + name]           = { "recoEff_" + name + "_denom",          "recoEff_" + name + "_numer" };
   effs["recoEff_seed_" + name]           = { "recoEff_seed_" + name + "_denom",          "recoEff_seed_" + name + "_numer" };
   effs["recoEff_algo_" + name] = { "recoEff_algo_" + name + "_denom", "recoEff_algo_" + name + "_numer" };
   effs["recoEff_core_" + name] = { "recoEff_core_" + name + "_denom", "recoEff_core_" + name + "_numer" };
   effs["acceptance_" + name] = { "acceptance_" + name + "_denom", "acceptance_" + name + "_numer" };
   effs["acceptance_recoEff_seed_" + name] = { "acceptance_recoEff_seed_" + name + "_denom", "acceptance_recoEff_seed_" + name + "_numer" };

}


void define2DEfficiencies(const std::string name_x, const std::string name_y, const std::string lab_x, const std::string lab_y, const std::vector<double> bins_x, const std::vector<double> bins_y)
{

   string name2D = name_x + "_vs_" + name_y;
   addHist<TH2F>("recoEff_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("recoEff_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));

   addHist<TH2F>("recoEff_seed_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("recoEff_seed_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));

   addHist<TH2F>("recoEff_algo_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("recoEff_algo_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));

   addHist<TH2F>("recoEff_core_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("recoEff_core_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));

   addHist<TH2F>("acceptance_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("acceptance_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));

   addHist<TH2F>("acceptance_recoEff_seed_" + name2D + "_denom", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));
   addHist<TH2F>("acceptance_recoEff_seed_" + name2D + "_numer", ";" + lab_x + ";" + lab_y + ";Entries", bins_x.size() - 1, &(bins_x.at(0)), bins_y.size() - 1, &(bins_y.at(0)));


   effs["recoEff_" + name2D]           = { "recoEff_" + name2D + "_denom",          "recoEff_" + name2D + "_numer" };
   effs["recoEff_seed_" + name2D]           = { "recoEff_seed_" + name2D + "_denom",          "recoEff_seed_" + name2D + "_numer" };
   effs["recoEff_algo_" + name2D] = { "recoEff_algo_" + name2D + "_denom", "recoEff_algo_" + name2D + "_numer" };
   effs["recoEff_core_" + name2D] = { "recoEff_core_" + name2D + "_denom", "recoEff_core_" + name2D + "_numer" };
   effs["acceptance_" + name2D] = { "acceptance_" + name2D + "_denom", "acceptance_" + name2D + "_numer" };
   effs["acceptance_recoEff_seed_" + name2D] = { "acceptance_recoEff_seed_" + name2D + "_denom", "acceptance_recoEff_seed_" + name2D + "_numer" };

}


template<typename ...REST>
void fillEfficiencies(const std::string name, const DVeff::DVeffVtx& v, REST ...rest)
{

   fillEff<TH1F>("recoEff_" + name, all(v), hasRecoVertex(v), rest...);
   fillEff<TH1F>("recoEff_seed_" + name, hasAtLeast2ReconstructedDescendents(v), hasAtLeast2SelectedDescendents(v), rest...);
   fillEff<TH1F>("recoEff_algo_" + name, hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), rest...);
   fillEff<TH1F>("recoEff_core_" + name, hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), rest...);
   fillEff<TH1F>("acceptance_" + name, all(v), hasAtLeast2ReconstructedDescendents(v), rest...);
   fillEff<TH1F>("acceptance_recoEff_seed_" + name, all(v), hasAtLeast2SelectedDescendents(v), rest...);

}

template<typename ...REST>
void fillEfficiencies(const std::string name_x, const std::string name_y, const DVeff::DVeffVtx& v, REST ...rest)
{

   string name2D = name_x + "_vs_" + name_y;

   fillEff<TH2F>("recoEff_" + name2D, all(v), hasRecoVertex(v), rest...);
   fillEff<TH2F>("recoEff_seed_" + name2D, hasAtLeast2ReconstructedDescendents(v), hasAtLeast2SelectedDescendents(v), rest...);
   fillEff<TH2F>("recoEff_algo_" + name2D, hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), rest...);
   fillEff<TH2F>("recoEff_core_" + name2D, hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), rest...);
   fillEff<TH2F>("acceptance_" + name2D, all(v), hasAtLeast2ReconstructedDescendents(v), rest...);
   fillEff<TH2F>("acceptance_recoEff_seed_" + name2D, all(v), hasAtLeast2SelectedDescendents(v), rest...);

}



/////////////////////////////////////////
// Macro which produces histograms
// and VSI efficiencies as a function
// of different variables
/////////////////////////////////////////


void DVeffAnalysis_Giuliano(const char* dirname, const char* outfilename = "output", const char* treename = "vsitree")
{


   gStyle->SetOptStat(0);

   gROOT->Reset();


   //////////////////////////////////////////////////
   // Setup Input trees

   TChain* chain = new TChain();


   TSystemDirectory dir(dirname, dirname);
   TList *files = dir.GetListOfFiles();
   if (files) {
      TSystemFile *file;
      TString fname;
      TIter next(files);
      while ((file = (TSystemFile*)next())) {
         fname = file->GetName();
         TString tmp_treename = (TString)dirname + "/" + (TString)file->GetName() + "/" + (TString)treename;
         if (!file->IsDirectory() && fname.EndsWith("VsiAnaModular.root")) {
            chain->Add(tmp_treename);
         }
      }
   } else cout << "No files in " << dirname << endl;

   cout << "N Entries: " << chain->GetEntries() << endl;

   EventInfoObj ei;
   PVObj pv;
   DVeffObj dveff;

   // Set branch addresses.
   setBranchAddress(chain, ei);
   setBranchAddress(chain, pv);
   setBranchAddress(chain, dveff);


   //////////////////////////////////////////////////
   // Setup Histograms and efficiencies

   //variables
   variables["ntrk"] = 0;
   variables["llp_ctau"] = 0;
   variables["llp_pt"] = 0;
   variables["llp_dR"] = 0;
   variables["actualMu"] = 0;
   variables["r"] = 0;
   variables["z"] = 0;

   //labels
   labels["ntrk"] = "Number of tracks";
   labels["llp_ctau"] = "LLP LF c#tau [mm]";
   labels["llp_pt"] = "LLP p_{T} [GeV]";
   labels["llp_dR"] = "#DeltaR(a_{1}, a_{2})";
   labels["actualMu"] = "actual #mu";
   labels["r"] = "r [mm]";
   labels["z"] = "z [mm]";

   // binning
   std::map<string, std::vector<double>> bins;
   std::vector<double> mbins;

   for (int i = 0; i < 51; i++) {
      mbins.emplace_back(i);
   }
   bins["ntrk"] = mbins;
   mbins.clear();
   bins["z"] = {0, 5, 10, 15, 25, 50, 100, 150, 200, 250, 300, 400, 500 } ;
   bins["llp_ctau"] = {0, 10, 20, 30, 40, 50, 60, 80, 100, 125, 150, 175, 200,  225, 250, 300, 350, 400, 500, 600, 700, 1000} ;
   bins["llp_pt"] = {0, 10, 20, 30, 40, 50, 60, 80, 100, 125, 150, 175, 200,  225, 250, 300, 350, 400, 500, 600, 700, 1000} ;
   bins["llp_dR"] = {0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0} ;
   bins["actualMu"] = { 0, 20, 40, 50, 60, 70, 80, 100 } ;
   bins["r"] = { 0.1, 0.3, 0.5, 0.7, 1.0, 1.5, 2, 2.5, 3, 5, 7, 10, 14, 20, 28, 38, 50, 64, 80, 100, 130, 170, 220, 280, 350, 450, 600 };

   bins["z_2D"] = {0, 5, 10, 25, 50, 100, 200, 300, 500 } ;
   bins["actualMu_2D"] = { 0, 20, 40, 60, 100 } ;
   bins["r_2D"] = { 0.1, 0.3, 0.5, 0.7, 1.0, 2, 3, 5, 10, 20, 38, 64, 100, 170, 220, 350, 600 };
   bins["llp_pt_2D"] = {0, 10, 20, 50, 100, 150, 200, 300, 500, 1000} ;

   std::vector<double> nbins = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20, 24, 28, 38, 50, 70, 100, 150, 220 };

   // 2D plots
   variables2D.push_back(make_pair("actualMu", "r"));
   variables2D.push_back(make_pair("llp_pt", "r"));
   variables2D.push_back(make_pair("z", "r"));
   variables2D.push_back(make_pair("z", "actualMu"));

   // bins 2D
   std::map<string, std::vector<std::vector<double>>> bins2D;


   //TH1
   for (auto kv : bins) {
      auto var_name = kv.first;
      if (((TString)var_name).Contains("2D")) continue;
      std::vector <double> binning = kv.second;
      auto label = labels[kv.first];

      defineEfficiencies(var_name, label, binning);

   }


   //TH2
   for (auto pair : variables2D) {
      auto var_name_x = pair.first;
      auto var_name_y = pair.second;
      std::vector <double> binning_x = bins[var_name_x + "_2D"];
      std::vector <double> binning_y = bins[var_name_y + "_2D"];

      auto label_x = labels[var_name_x];
      auto label_y = labels[var_name_y];

      define2DEfficiencies(var_name_x, var_name_y, label_x, label_y, binning_x, binning_y);

   }

   //Other histograms
   addHist<TH1F>("matchScore", ";p_{T} Weighted Truth Match Score;Entries", 50, 0, 1);

   //////////////////////////////////////////////////
   // Main loop

   Stats stats;

   Long64_t nentries = chain->GetEntries();

   for (Long64_t i = 0; i < nentries; i++) {
      chain->GetEntry(i);

      for (size_t i_llp = 0; i_llp < dveff.LLP_size; ++i_llp) {

         variables["actualMu"] = ei.actualMu;
         process(dveff, pv, i_llp, stats);
         if (dveff.LLP_size != 2) std::cout << "Number of LLP: " << dveff.LLP_size << std::endl;
      }

      stats.nEvents++;
   }


   //////////////////////////////////////////////////
   // Post processing

   std::cout << "#########################################################" << std::endl;
   std::cout << Form("Total events : %lu", stats.nEvents) << std::endl;
   std::cout << Form("Total LLPs : %lu", stats.nLLPs) << std::endl;
   std::cout << Form("Total LLPs with truth vertex position(s) : %lu", stats.nLLPs_hasTruthPos) << std::endl;
   std::cout << Form("Total LLPs with reco vertices : %lu", stats.nLLPs_hasReco) << std::endl;
   std::cout << Form("Total LLPs with >1 reco vertices : %lu", stats.nLLPs_hasMoreThanOneRecos) << std::endl;

   for (auto kv : variables) {
      auto var_name = kv.first;
      formatEfficiencies(var_name);
   }

   for (auto pair : variables2D) {
      auto var_name_x = pair.first;
      auto var_name_y = pair.second;
      formatEfficiencies(var_name_x, var_name_y);
   }

   auto* ofile = TFile::Open("outputs/" + (TString)outfilename + ".root", "recreate");
   for (auto& p : histos) {
      p.second->Write();
   }
   ofile->Close();

}



void process(DVeffObj& dveff, PVObj& pv, const size_t i_llp, Stats& stats)
{

   DVeff::DVeffVtx v;

   TVector3 pvPos(pv.x, pv.y, pv.z);
   setup(v, dveff, i_llp);

   if (i_llp == 0) llp_0_p4 = v.llp.p4;
   if (i_llp > 0) llp_dR = llp_0_p4.DeltaR(v.llp.p4);

   const auto r = (v.llp.decay - pvPos).Perp();
   const auto z = fabs(v.llp.decay.Z() - pvPos.Z());
   variables["z"] = z;
   variables["r"] = r;
   variables["llp_dR"] = llp_dR;
   variables["llp_pt"] = v.llp.p4.Pt() / 1e3;
   variables["llp_ctau"] = (v.llp.decay - pvPos).Mag();

   double ntrk_tot = -1;
   for (size_t iv = 0; iv < v.recoVertices.size(); ++iv) {
      if (ntrk_tot < 0) ntrk_tot = 0;
      const auto& vtx = v.recoVertices.at(iv);
      fillHist<TH1F>("matchScore", vtx.matchScore);
      ntrk_tot += vtx.ntrk_nonAssociated;
   }

   variables["ntrk"] = ntrk_tot;


   for (auto kv : variables) {
      auto var_name = kv.first;
      auto var_val = kv.second;
      // name, denom_condition, numer_condition, variable
      fillEfficiencies(var_name, v, var_val);
   }

   //TH2
   for (auto pair : variables2D) {
      auto var_name_x = pair.first;
      auto var_name_y = pair.second;
      auto var_val_x = variables[var_name_x];
      auto var_val_y = variables[var_name_y];
      fillEfficiencies(var_name_x, var_name_y, v, var_val_x, var_val_y);
   }

   stats.nLLPs++;
   if (v.truthPositions.size()) stats.nLLPs_hasTruthPos++;
   if (v.recoVertices.size()) stats.nLLPs_hasReco++;
   if (v.recoVertices.size() > 1) stats.nLLPs_hasMoreThanOneRecos++;
}





void DVeff::DVeffVtx::print(size_t index)
{
   std::cout << Form("LLP [%lu] pid = %8d, mass = %.3e, decay(r,z) = (%8.2f, %8.2f) mm",
                     index, llp.pid, llp.p4.M(), llp.decay.Perp(), llp.decay.z())
             << std::endl;
   std::cout << "\n" << std::endl;
   std::cout << Form("  * Descendents size = %lu, sum pt = %.3e GeV, sum mass = %.3e GeV; nReco %lu, nSel %lu",
                     descendents   .size(),
                     descendentsSumP4.Pt() * 1.e-3, descendentsSumP4.M() * 1.e-3,
   std::accumulate(descendents.begin(), descendents.end(), 0UL, [](size_t s, DVeff::Descendent & d) {
      return s + d.isReconstructed;
   }),
   std::accumulate(descendents.begin(), descendents.end(), 0UL, [](size_t s, DVeff::Descendent & d) {
      return s + d.isSelected;
   }))
         << std::endl;
   std::cout << Form("  * TruthPos    size = %lu", truthPositions.size()) << std::endl;
   std::cout << Form("  * RecoVtx     size = %lu", recoVertices  .size()) << std::endl;
   std::cout << "\n" << std::endl;

   for (size_t it = 0; it < truthPositions.size(); ++it) {
      const auto& tpos = truthPositions.at(it);
      std::cout << Form("  - TruthPos [%lu]: (r,z) = (%8.2f, %8.2f) mm", it, tpos.pos.Perp(), tpos.pos.z());
      std::cout << Form(", sum pt = %.3e GeV, sum mass = %8.3e GeV, #descendents %lu",
                        tpos.reconstructibleSumP4.Pt() * 1.e-3,
                        tpos.reconstructibleSumP4.M() * 1.e-3,
                        tpos.nReconstructible) << std::endl;
   }
   std::cout << std::endl;
   for (size_t iv = 0; iv < recoVertices.size(); ++iv) {
      const auto& vtx = recoVertices.at(iv);
      std::cout << Form("  - RecoVtx [%lu]: (r,z) = (%8.2f, %8.2f) mm", iv, vtx.pos.Perp(), vtx.pos.z()) << std::endl;
      std::cout << Form("       * TruthPos [%lu], matchScore = %.3f", vtx.truthPosIndex, vtx.matchScore) << std::endl;
      std::cout << Form("       * residual mag  : %.3f um", vtx.residual.Mag() * 1.e3) << std::endl;
      std::cout << Form("       * ntrk          : %lu (nonAssociated %lu)", vtx.ntrk, vtx.ntrk_nonAssociated) << std::endl;
      std::cout << Form("       * mass          : %.3e GeV (nonAssociated %.3e GeV)", vtx.mass * 1.e-3, vtx.mass_nonAssociated * 1.e-3) << std::endl;
      std::cout << Form("       * filtered mass : %.3e GeV (nonAssociated %.3e GeV)", vtx.filtered_mass * 1.e-3, vtx.filtered_mass_nonAssociated * 1.e-3) << std::endl;
   }
   std::cout << "\n" << std::endl;
   std::cout << "===================================================\n" <<  std::endl;
}




void setup(DVeff::DVeffVtx& v, DVeffObj& dveff, const size_t i_llp)
{

   using namespace DVeff;

   // LLP

#define GET_LLP( name ) dveff.LLP_ ## name ->at( i_llp )

   v.llp.decay.SetXYZ(GET_LLP(decay_x), GET_LLP(decay_y), GET_LLP(decay_z));

   v.llp.p4.SetPtEtaPhiM(GET_LLP(pt), GET_LLP(eta), GET_LLP(phi), GET_LLP(mass));

   v.llp.pid = GET_LLP(pid);


   // Descendents sum
   v.descendentsSumP4.SetPtEtaPhiM(dveff.descendent_sum_pt  ->at(i_llp),
                                   dveff.descendent_sum_eta ->at(i_llp),
                                   dveff.descendent_sum_phi ->at(i_llp),
                                   dveff.descendent_sum_mass->at(i_llp));

   // Descendent particles

   for (size_t idesc = 0; idesc < dveff.descendent_size->at(i_llp); ++idesc) {
#define GET_DESC( name ) dveff.descendent_ ## name ->at(i_llp).at(idesc)

      auto desc = Descendent{};

      desc.charge = GET_DESC(charge);

      desc.pid    = GET_DESC(pid);

      desc.p4.SetPtEtaPhiM(GET_DESC(pt), GET_DESC(eta), GET_DESC(phi), GET_DESC(mass));

      desc.prod.SetXYZ(GET_DESC(prod_x), GET_DESC(prod_y), GET_DESC(prod_z));
      desc.decay.SetXYZ(GET_DESC(decay_x), GET_DESC(decay_y), GET_DESC(decay_z));

      desc.isReconstructed = GET_DESC(isReconstructed);
      desc.isSelected      = GET_DESC(isSelected);

      desc.truthPosIndex   = GET_DESC(truthPosIndex);

      v.descendents.emplace_back(desc);
   }


   // TruthVertexPos

   for (size_t ipos = 0; ipos < dveff.truthPos_r->at(i_llp).size(); ++ipos) {
#define GET_TPOS( name ) dveff.truthPos_ ## name ->at(i_llp).at(ipos)

      auto pos = TruthPos{};

      pos.pos.SetXYZ(GET_TPOS(x), GET_TPOS(y), GET_TPOS(z));

      pos.nReconstructible = GET_TPOS(nReconstructible);
      pos.reconstructibleSumP4.SetPtEtaPhiM(GET_TPOS(sum_pt), GET_TPOS(sum_eta), GET_TPOS(sum_phi), GET_TPOS(reconstructibleMass));

      v.truthPositions.emplace_back(pos);
   }


   // Reco-vtx
   for (size_t ireco = 0; ireco < dveff.recoVtx_LLP_index->size(); ++ireco) {
#define GETRECO( name ) dveff.recoVtx_ ## name ->at(ireco)

      // Skip if the LLP index doesn't match.

      if (GETRECO(LLP_index) != i_llp) continue;

      RecoVtx vtx;

      vtx.truthPosIndex = GETRECO(truthPos_index);
      vtx.matchScore    = GETRECO(matchScore);

      vtx.pos.SetXYZ(GETRECO(x), GETRECO(y), GETRECO(z));
      vtx.residual.SetXYZ(GETRECO(residual_x), GETRECO(residual_y), GETRECO(residual_z));

      vtx.chi2                        = GETRECO(chi2);
      vtx.chi2_core                   = GETRECO(chi2_core);
      vtx.mass                        = GETRECO(mass);
      vtx.mass_nonAssociated          = GETRECO(mass_nonAssociated);
      vtx.ntrk                        = GETRECO(ntrk);
      vtx.ntrk_nonAssociated          = GETRECO(ntrk_nonAssociated);
      vtx.filtered_mass               = GETRECO(filtered_mass);
      vtx.filtered_mass_nonAssociated = GETRECO(filtered_mass_nonAssociated);
      vtx.filtered_ntrk               = GETRECO(filtered_ntrk);
      vtx.filtered_ntrk_nonAssociated = GETRECO(filtered_ntrk_nonAssociated);

      v.recoVertices.emplace_back(vtx);
   }
}

