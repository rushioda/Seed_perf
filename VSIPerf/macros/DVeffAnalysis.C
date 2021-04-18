#include <include/NtupleObj.h>

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

float weight = 0;
float xsec = 1;

namespace DVeff {

  class LLP {
  public:
    TVector3 decay;
    TLorentzVector p4;
    int pid;
    float childOpenAngle;
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
  float nEvents;
  float nLLPs;
  float nLLPs_hasTruthPos;
  float nLLPs_hasReco;
  float nLLPs_hasMoreThanOneRecos;
  float nLLPs_split;
  float nTruthSingleVertices;
  float nTruthMultiVertices;
  float nRecoSplitFiducialVertices;
  float nRecoSplitVertices; //inclusive split rate: N truth vertices -> 2+ reco vertices
  float nRecoAccidentalSplitVertices; //accidental split rate: 1 truth vertex -> 2+ reco vertices
  float nRecoMergedVertices; //merge rate: 2+ truth vertices -> 1 reco vertex
};

Stats::Stats() 
  : nEvents                      (0)
  , nLLPs                        (0)
  , nLLPs_hasTruthPos            (0)
  , nLLPs_hasReco                (0)
  , nLLPs_hasMoreThanOneRecos    (0)
  , nLLPs_split                  (0)
  , nTruthSingleVertices         (0)
  , nTruthMultiVertices          (0)
  , nRecoSplitFiducialVertices   (0)
  , nRecoSplitVertices           (0)
  , nRecoAccidentalSplitVertices (0)
  , nRecoMergedVertices          (0)
{}




void setup( DVeff::DVeffVtx& v, DVeffObj& dveff, const size_t i_llp );
void process( DVeffObj& dveff, PVObj& pv, EventInfoObj& ei, const size_t i_llp, Stats& stats );



std::map<std::string, TObject*> histos;

using EffHistPair = std::pair<std::string, std::string>;

std::map<std::string, EffHistPair> effs;

template<class HISTO, typename ...REST>
void addHist( const std::string name, const std::string title, REST ...rest ) {
  auto* h = new HISTO(name.c_str(), title.c_str(), rest... );
  h->Sumw2();
  histos[name] = h;
}
  
template<class HISTO, typename ...REST>
void fillHist( const std::string name, REST ...rest ) {
  if( histos.find(name) == histos.end() ) {
    std::string err = Form( "%s: %s not found in the histogram list!!", __PRETTY_FUNCTION__, name.c_str() );
    std::cerr << err << std::endl;
    throw std::runtime_error( err );
  }
  ( dynamic_cast<HISTO*>( histos[name] ) )->Fill( rest... , weight);
}

template<class HISTO, typename ...REST>
void fillEff( const std::string name, bool denom_condition, bool numer_condition, REST ...rest ) {
  if( effs.find(name) == effs.end() ) {
    std::string err = Form( "%s: %s not found in the efficiency list!!", __PRETTY_FUNCTION__, name.c_str() );
    std::cerr << err << std::endl;
    throw std::runtime_error( err );
  }
  
  if( denom_condition ) {
    fillHist<HISTO>( effs.at(name).first,  rest... );
    if( numer_condition ) fillHist<HISTO>( effs.at(name).second, rest... );
  }
}


template<class HISTO>
void format( const std::string name ) {}


template<>
void format<TH1F>( const std::string name ) {
  
  auto* denom = dynamic_cast<TH1F*>( histos[ effs[name].first ] );
  auto* numer = dynamic_cast<TH1F*>( histos[ effs[name].second ] );
  
  //denom->Sumw2();
  //numer->Sumw2();

  TEfficiency *heff = new TEfficiency( *numer, *denom );
  heff->SetName( name.c_str() );
  heff->SetUseWeightedEvents(true);
  //heff->SetStatisticOption(TEfficiency::kBUniform); //JS3 - bad
  //heff->SetStatisticOption(TEfficiency::kFNormal); //JS3 - less bad
  heff->SetPosteriorMode(); heff->SetStatisticOption(TEfficiency::kBUniform); //JS3 - okay
  histos[ name ] = heff;
  
  for( int ibin=0; ibin< denom->GetNbinsX(); ibin++) {
    auto width = denom->GetBinWidth(ibin+1);
    denom->SetBinContent( ibin+1, denom->GetBinContent(ibin+1) / width );
    denom->SetBinError  ( ibin+1, denom->GetBinError  (ibin+1) / width );
  }
  denom->Scale(1.0/denom->Integral());
  
  for( int ibin=0; ibin< numer->GetNbinsX(); ibin++) {
    auto width = numer->GetBinWidth(ibin+1);
    numer->SetBinContent( ibin+1, numer->GetBinContent(ibin+1) / width );
    numer->SetBinError  ( ibin+1, numer->GetBinError  (ibin+1) / width );
  }
  numer->Scale(1.0/numer->Integral());
  
}


void DVeffAnalysis( const char* filename="all.root", const char* outputfile="output.root", const char* treename="vsitree" ) {
  
    gStyle->SetOptStat(0);

   gROOT->Reset();
   
   
   //////////////////////////////////////////////////
   // Setup Input trees
   
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject( filename );
   if (!f) {
      f = new TFile( filename );
   }
   auto* tree = (TTree*)( f->FindObjectAny(treename) );
   
   EventInfoObj ei;
   PVObj pv;
   DVeffObj dveff;
   
   // Set branch addresses.
   setBranchAddress( tree, ei );
   setBranchAddress( tree, pv );
   setBranchAddress( tree, dveff );
   
   
   
   //////////////////////////////////////////////////
   // Setup Histograms and efficiencies
  
   // binning
   std::vector<double> rbins = { 0.1, 0.3, 0.5, 0.7, 1.0, 1.5, 2, 2.5, 3, 5, 7, 10, 14, 20, 28, 38, 50, 64, 80, 100, 130, 170, 220, 280, 350, 450, 600 };
   std::vector<double> rbins_splitRate = {2,5,10,20,30,40,50,100,150,200,300};
   //std::vector<double> nbins = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20, 24, 28, 38, 50 };
   std::vector<double> nbins = { 0,2, 3, 4, 6, 8, 10, 15, 20, 25 };
   std::vector<double> nbins_splitRate = { 0, 10, 20 ,30, 40,50 };
   std::vector<double> mubins = {10, 20, 30, 40, 50, 75};

   std::vector<double> n1bins = { 2, 5, 10, 20, 30, 40, 50};
   std::vector<double> r1bins = { 2,10,20,30,50,75,100,125,150,200,300 };
   
   std::vector<double> oAbins = { 0.0,0.1,0.2,0.3, 0.5, 0.7, 1.0, 1.5, 2, 2.5, 3};

   std::vector<double> mbins;
   for( int i=0; i<101; i++) { mbins.emplace_back( pow(10, -1.0 + i*0.05) ); }
  
   // Efficiency
   addHist<TH1F>( "recoEff_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "recoEff_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );

   addHist<TH1F>( "recoEff_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "recoEff_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   
   addHist<TH1F>( "recoEff_fiducial1_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial1_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
  
   addHist<TH1F>( "recoEff_fiducial1_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial1_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
 
   addHist<TH1F>( "recoEff_fiducial2_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial2_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );

   addHist<TH1F>( "recoEff_fiducial2_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial2_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   
   addHist<TH1F>( "acceptance_fiducial1_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "acceptance_fiducial1_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );

   addHist<TH1F>( "acceptance_fiducial1_averageMu_num", ";Average #mu",mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "acceptance_fiducial1_averageMu_den", ";Average #mu",mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial1_averageMu_num", ";Average #mu",mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial1_averageMu_den", ";Average #mu",mubins.size()-1, &(mubins.at(0)) );

   addHist<TH1F>( "acceptance_fiducial1_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "acceptance_fiducial1_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );  
   
   addHist<TH1F>( "acceptance_fiducial2_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "acceptance_fiducial2_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );

   addHist<TH1F>( "acceptance_fiducial2_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "acceptance_fiducial2_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) ); 
   
   addHist<TH1F>( "seedEff_r_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );
   addHist<TH1F>( "seedEff_r_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)) );

   addHist<TH1F>( "seedEff_averageMu_denom", ";Average #mu;Entries", mubins.size()-1, &(mubins.at(0)));
   addHist<TH1F>( "seedEff_averageMu_numer", ";Average #mu;Entries", mubins.size()-1, &(mubins.at(0)));

   addHist<TH1F>( "seedEff_openAngle_denom", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );
   addHist<TH1F>( "seedEff_openAngle_numer", ";#DeltaR(lep,lep);Entries", oAbins.size()-1, &(oAbins.at(0)) );

   
   addHist<TH1F>( "recoEff_fiducial2_mult_denom", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial2_mult_numer", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );

   addHist<TH1F>( "recoEff_fiducial1_mult_denom", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial1_mult_numer", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );

   addHist<TH1F>( "recoEff_fiducial2_averageMu_denom", ";Seed Tracks;Entries",  mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "recoEff_fiducial2_averageMu_numer", ";Seed Tracks;Entries",  mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "recoEff_averageMu_denom", ";Seed Tracks;Entries",  mubins.size()-1, &(mubins.at(0)) );
   addHist<TH1F>( "recoEff_averageMu_numer", ";Seed Tracks;Entries",  mubins.size()-1, &(mubins.at(0)) );

   addHist<TH1F>( "recoEff_mult_denom", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );
   addHist<TH1F>( "recoEff_mult_numer", ";Seed Tracks;Entries", nbins.size()-1, &(nbins.at(0)) );
   
   // Selection efficiency
   addHist<TH1F>( "selEff_denom", ";;Entries", 2, 0, 2 );
   addHist<TH1F>( "selEff_numer", ";;Entries", 2, 0, 2 );
   
   
   effs["recoEff_r"]           = { "recoEff_r_denom",          "recoEff_r_numer" };
   effs["recoEff_fiducial1_r"] = { "recoEff_fiducial1_r_denom", "recoEff_fiducial1_r_numer" };
   effs["recoEff_fiducial2_r"] = { "recoEff_fiducial2_r_denom", "recoEff_fiducial2_r_numer" };

   effs["recoEff_fiducial2_averageMu"] = { "recoEff_fiducial2_averageMu_denom", "recoEff_fiducial2_averageMu_numer" };
   effs["recoEff_averageMu"] = { "recoEff_averageMu_denom", "recoEff_averageMu_numer" };

   effs["acceptance_fiducial1_averageMu"] = { "acceptance_fiducial1_averageMu_den","acceptance_fiducial1_averageMu_num" };
   effs["recoEff_fiducial1_averageMu"] = { "recoEff_fiducial1_averageMu_den","recoEff_fiducial1_averageMu_num" };

   effs["acceptance_fiducial1_r"] = { "acceptance_fiducial1_r_denom", "acceptance_fiducial1_r_numer" };
   effs["acceptance_fiducial2_r"] = { "acceptance_fiducial2_r_denom", "acceptance_fiducial2_r_numer" };
   effs["seedEff_r"] = { "seedEff_r_denom", "seedEff_r_numer" };
   effs["seedEff_averageMu"] = { "seedEff_averageMu_denom", "seedEff_averageMu_numer" };

   effs["recoEff_openAngle"]           = { "recoEff_openAngle_denom",          "recoEff_openAngle_numer" };
   effs["recoEff_fiducial1_openAngle"] = { "recoEff_fiducial1_openAngle_denom", "recoEff_fiducial1_openAngle_numer" };
   effs["recoEff_fiducial2_openAngle"] = { "recoEff_fiducial2_openAngle_denom", "recoEff_fiducial2_openAngle_numer" };
   effs["acceptance_fiducial1_openAngle"] = { "acceptance_fiducial1_openAngle_denom", "acceptance_fiducial1_openAngle_numer" };
   effs["acceptance_fiducial2_openAngle"] = { "acceptance_fiducial2_openAngle_denom", "acceptance_fiducial2_openAngle_numer" };
   effs["seedEff_openAngle"] = { "seedEff_openAngle_denom", "seedEff_openAngle_numer" };  

 
   effs["recoEff_fiducial2_mult"] = { "recoEff_fiducial2_mult_denom", "recoEff_fiducial2_mult_numer" };
   effs["recoEff_fiducial1_mult"] = { "recoEff_fiducial1_mult_denom", "recoEff_fiducial1_mult_numer" };

   effs["recoEff_mult"] = { "recoEff_mult_denom", "recoEff_mult_numer" };
   
   effs["selEff"] = { "selEff_denom", "selEff_numer" };

   effs["splitRate_maxTruthDelta"] = { "splitRate_maxTruthDelta_den","splitRate_maxTruthDelta_num"};
   effs["splitRate_radius"] = { "splitRate_radius_den","splitRate_radius_num"};
   effs["splitRate_trackMult"] = { "splitRate_trackMult_den","splitRate_trackMult_num"};

   effs["accidentalSplitRate_maxTruthDelta"] = {"accidentalSplitRate_maxTruthDelta_den","accidentalSplitRate_maxTruthDelta_num"};
   effs["accidentalSplitRate_radius"] = { "accidentalSplitRate_radius_den","accidentalSplitRate_radius_num"};

   effs["mergeRate_maxTruthDelta"] = {"mergeRate_maxTruthDelta_den","mergeRate_maxTruthDelta_num"};
   effs["mergeRate_radius"] = {"mergeRate_radius_den","mergeRate_radius_num"};


   addHist<TH2F>( "hist_OpeningAngleNTrk", ";Max #DeltaR;Number of tracks;Entries", 50, 0, 6, 10, 0 ,50 );
   
   // Residuals
   addHist<TH2F>( "residual1_r", ";r [mm];#Deltar [mm];Entries", r1bins.size()-1, &(r1bins.at(0)), 25, -0.75, 0.75 );
   addHist<TH2F>( "residual1_z", ";r [mm];#Deltaz [mm];Entries", r1bins.size()-1, &(r1bins.at(0)), 25, -0.75, 0.75 );
   addHist<TH2F>( "residual1_rphi", ";r [mm];r #times #Delta#phi [mm];Entries", r1bins.size()-1, &(r1bins.at(0)), 25, -0.75, 0.75 );

   addHist<TH2F>( "residual1_r_mult", ";Tracks;#Deltar [mm];Entries", n1bins.size()-1, &(n1bins.at(0)), 25, -0.75, 0.75 );
   addHist<TH2F>( "residual1_z_mult", ";Tracks;#Deltaz [mm];Entries", n1bins.size()-1, &(n1bins.at(0)), 25, -0.75, 0.75 );
   addHist<TH2F>( "residual1_rphi_mult", ";Tracks;;r #times #Delta#phi [mm];Entries", n1bins.size()-1, &(n1bins.at(0)), 25, -0.75, 0.75 );
   
   addHist<TH2F>( "residual2_r", ";r [mm];#Deltar [mm];Entries", rbins.size()-1, &(rbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual2_z", ";r [mm];#Deltaz [mm];Entries", rbins.size()-1, &(rbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual2_rphi", ";r [mm];r #times #Delta#phi [mm];Entries", rbins.size()-1, &(rbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_r30", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_r30", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_r30", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_r50", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_r50", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_r50", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_r85", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_r85", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_r85", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_r125", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_r125", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_r125", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_r300", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_r300", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_r300", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH2F>( "residual1_r_mult_pixel", ";Tracks;#Deltar [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_z_mult_pixel", ";Tracks;#Deltaz [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   addHist<TH2F>( "residual1_rphi_mult_pixel", ";Tracks;;r #times #Delta#phi [mm];Entries", nbins.size()-1, &(nbins.at(0)), 6000, -10.0, 10.0 );
   
   addHist<TH1F>( "splitDistance", ";split distance [mm]", 400, 0, 100);
   
   addHist<TH1F>( "splitRate_maxTruthDelta_num",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "splitRate_maxTruthDelta_den",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "splitRate_radius_num",";Radius [mm]",rbins_splitRate.size()-1, &(rbins_splitRate.at(0)));
   addHist<TH1F>( "splitRate_radius_den",";Radius [mm]",rbins_splitRate.size()-1, &(rbins_splitRate.at(0)));
   addHist<TH1F>( "splitRate_trackMult_num",";Track multiplicity",nbins_splitRate.size()-1, &(nbins_splitRate.at(0)));
   addHist<TH1F>( "splitRate_trackMult_den",";Track multiplicity",nbins_splitRate.size()-1, &(nbins_splitRate.at(0)));

   addHist<TH1F>( "mergeRate_maxTruthDelta_num",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "mergeRate_maxTruthDelta_den",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "mergeRate_radius_num",";Radius [mm]",rbins.size()-1, &(rbins.at(0)));
   addHist<TH1F>( "mergeRate_radius_den",";Radius [mm]",rbins.size()-1, &(rbins.at(0)));
   
   addHist<TH1F>( "accidentalSplitRate_maxTruthDelta_num",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "accidentalSplitRate_maxTruthDelta_den",";max truth vertex separation [mm]",20,0,20);
   addHist<TH1F>( "accidentalSplitRate_radius_num",";Radius [mm]",rbins.size()-1, &(rbins.at(0)));
   addHist<TH1F>( "accidentalSplitRate_radius_den",";Radius [mm]",rbins.size()-1, &(rbins.at(0)));
   // TrackMult
   addHist<TH2F>( "mult_reconstructible_r", ";r [mm];Tracks;Entries", rbins.size()-1, &(rbins.at(0)), nbins.size()-1, &(nbins.at(0)) );
   addHist<TH2F>( "mult_tracks_r", ";r [mm];Tracks;Entries", rbins.size()-1, &(rbins.at(0)), nbins.size()-1, &(nbins.at(0)) );
   addHist<TH2F>( "mult_selected_r", ";r [mm];Selected Tracks;Entries", rbins.size()-1, &(rbins.at(0)), nbins.size()-1, &(nbins.at(0)) );
   addHist<TH2F>( "mult_reco_r", ";r [mm];Reconstructed Tracks [GeV];Entries", rbins.size()-1, &(rbins.at(0)), nbins.size()-1, &(nbins.at(0)) );
   addHist<TH2F>( "mult_recoCore_r", ";r [mm];Reconstructed Tracks (Core) [GeV];Entries", rbins.size()-1, &(rbins.at(0)), nbins.size()-1, &(nbins.at(0)) );
   addHist<TH2F>( "multFrac_r", ";r [mm];R_{n}^{full};Entries", rbins.size()-1, &(rbins.at(0)), 100, 0.0, 2.0 );
   addHist<TH2F>( "multFracCore_r", ";r [mm];R_{n}^{core};Entries", rbins.size()-1, &(rbins.at(0)), 100, 0.0, 2.0 );
   
   // TrackMass
   addHist<TH2F>( "mass_reconstructible_r", ";r [mm];Mass [GeV];Entries", rbins.size()-1, &(rbins.at(0)), mbins.size()-1, &(mbins.at(0)) );
   addHist<TH2F>( "mass_tracks_r", ";r [mm];Mass [GeV];Entries", rbins.size()-1, &(rbins.at(0)), mbins.size()-1, &(mbins.at(0)) );
   addHist<TH2F>( "mass_selected_r", ";r [mm];Selected Mass [GeV];Entries", rbins.size()-1, &(rbins.at(0)), mbins.size()-1, &(mbins.at(0)) );
   addHist<TH2F>( "mass_reco_r", ";r [mm];Reconstructed Mass [GeV];Entries", rbins.size()-1, &(rbins.at(0)), mbins.size()-1, &(mbins.at(0)) );
   addHist<TH2F>( "mass_recoCore_r", ";r [mm];Reconstructed Mass (Core) [GeV];Entries", rbins.size()-1, &(rbins.at(0)), mbins.size()-1, &(mbins.at(0)) );
   addHist<TH2F>( "massFrac_r", ";r [mm];R_{m}^{full};Entries", rbins.size()-1, &(rbins.at(0)), 100, 0.0, 2.0 );
   addHist<TH2F>( "massFracCore_r", ";r [mm];R_{m}^{core};Entries", rbins.size()-1, &(rbins.at(0)), 100, 0.0, 2.0 );
   
   // Match score
   addHist<TH1F>( "matchScore", ";Match Score", 200, 0, 1.0 );
   
   //////////////////////////////////////////////////
   // Main loop
   
   Stats stats;
   
   Long64_t nentries = tree->GetEntries();
   
   
   for (Long64_t i=0; i<nentries;i++) {

     tree->GetEntry(i);

     if ((ei.mcChannelNumber >= 309851 && ei.mcChannelNumber <= 309856) || ei.mcChannelNumber == 309865) xsec = 278 / 1e3;
     if ((ei.mcChannelNumber >= 309857 && ei.mcChannelNumber <= 309862) || ei.mcChannelNumber == 309866) xsec = 175 / 1e3;
     weight = ei.mcWeight * xsec;
     
     for( size_t i_llp = 0; i_llp < dveff.LLP_size; ++i_llp ) {
       process( dveff, pv, ei, i_llp, stats );
     }
     
     stats.nEvents+=weight;
   }
   
   
   //////////////////////////////////////////////////
   // Post processing
   
   std::cout << "#########################################################" << std::endl;
   std::cout << Form("Total events : %f", stats.nEvents ) << std::endl;
   std::cout << Form("Total LLPs : %f", stats.nLLPs ) << std::endl;
   std::cout << Form("Total LLPs with truth vertex position(s) : %f", stats.nLLPs_hasTruthPos ) << std::endl;
   std::cout << Form("Total LLPs with reco vertices : %f", stats.nLLPs_hasReco ) << std::endl;
   std::cout << Form("Total LLPs with >1 reco vertices : %f", stats.nLLPs_hasMoreThanOneRecos ) << std::endl;
   std::cout << Form("Total LLPs with split : %f", stats.nLLPs_split ) << std::endl;
   std::cout << std::endl;
   std::cout << Form("Total single truth vertices : %f",stats.nTruthSingleVertices) << std::endl;
   std::cout << Form("Total multiple truth vertices : %f",stats.nTruthMultiVertices) << std::endl;
   std::cout << std::endl;
   std::cout << Form("Total fiducial reco vertices : %f",stats.nRecoSplitFiducialVertices) << std::endl;
   std::cout << Form("Total (inclusive) split reco vertices : %f",stats.nRecoSplitVertices) << std::endl;
   std::cout << Form("Total (accidental) split reco vertices : %f",stats.nRecoAccidentalSplitVertices) << std::endl;
   std::cout << Form("Total merged reco vertices : %f",stats.nRecoMergedVertices) << std::endl;
   std::cout << std::endl;
   std::cout << Form("Inclusive split rate : %f", static_cast<double>(stats.nRecoSplitVertices) / static_cast<double>(stats.nRecoSplitFiducialVertices) ) << std::endl;
   std::cout << Form("Accidental split rate : %f",float(stats.nRecoAccidentalSplitVertices)/stats.nTruthSingleVertices) << std::endl;
   std::cout << Form("Merge rate : %f",float(stats.nRecoMergedVertices)/stats.nTruthMultiVertices) << std::endl;

   //JS3
   int nBins=((TH1F*)histos["acceptance_fiducial1_r_numer"])->GetNbinsX();
   float acceptance[nBins], seed[nBins], core[nBins], totalEff[nBins];
   for (int bin=1; bin<=nBins; bin++){
     acceptance[bin]=float(((TH1F*)histos["acceptance_fiducial1_r_numer"])->GetBinContent(bin))/((TH1F*)histos["acceptance_fiducial1_r_denom"])->GetBinContent(bin);
     seed[bin]=float(((TH1F*)histos["seedEff_r_numer"])->GetBinContent(bin))/((TH1F*)histos["seedEff_r_denom"])->GetBinContent(bin);
     core[bin]=float(((TH1F*)histos["recoEff_fiducial2_r_numer"])->GetBinContent(bin))/((TH1F*)histos["recoEff_fiducial2_r_denom"])->GetBinContent(bin);
     totalEff[bin]=float(((TH1F*)histos["recoEff_r_numer"])->GetBinContent(bin))/((TH1F*)histos["recoEff_r_denom"])->GetBinContent(bin);
   }

   format<TH1F>( "recoEff_r" );
   format<TH1F>( "recoEff_openAngle" );
   format<TH1F>( "recoEff_fiducial1_r" );
   format<TH1F>( "recoEff_fiducial2_r" );
   format<TH1F>( "recoEff_fiducial1_openAngle" );
   format<TH1F>( "recoEff_fiducial2_openAngle" );
   format<TH1F>( "recoEff_fiducial2_averageMu" );
   format<TH1F>( "recoEff_fiducial1_averageMu" );
   format<TH1F>( "acceptance_fiducial1_averageMu" );
   format<TH1F>( "recoEff_averageMu" );
   format<TH1F>( "acceptance_fiducial1_r" );
   format<TH1F>( "acceptance_fiducial2_r" );
   format<TH1F>( "acceptance_fiducial1_openAngle" );
   format<TH1F>( "acceptance_fiducial2_openAngle" );
   format<TH1F>( "seedEff_r" );
   format<TH1F>( "seedEff_averageMu" );
   format<TH1F>( "seedEff_openAngle" );
   format<TH1F>( "recoEff_fiducial2_mult" );
   format<TH1F>( "recoEff_fiducial1_mult" );
   format<TH1F>( "recoEff_mult" );
   format<TH1F>( "selEff" );
   format<TH1F>( "splitRate_maxTruthDelta" );
   format<TH1F>( "splitRate_radius" );
   format<TH1F>( "splitRate_trackMult" );
   format<TH1F>( "accidentalSplitRate_maxTruthDelta" );
   format<TH1F>( "accidentalSplitRate_radius" );
   format<TH1F>( "mergeRate_maxTruthDelta" );
   format<TH1F>( "mergeRate_radius" );

   //JS3
   for (int bin=1; bin<=nBins; bin++){
     if (fabs(acceptance[bin]-((TEfficiency*)histos["acceptance_fiducial1_r"])->GetEfficiency(bin))/acceptance[bin]>0.001 ||
	 fabs(seed[bin]-((TEfficiency*)histos["seedEff_r"])->GetEfficiency(bin))/seed[bin]>0.001 ||
	 fabs(core[bin]-((TEfficiency*)histos["recoEff_fiducial2_r"])->GetEfficiency(bin))/core[bin]>0.001 ||
	 fabs(totalEff[bin]-((TEfficiency*)histos["recoEff_r"])->GetEfficiency(bin))/totalEff[bin]>0.001) {
       std::cout<<"ERROR - central values of efficiencies are incorrect"<<std::endl;
       return;
     }
   }

   auto* ofile = TFile::Open(outputfile, "recreate");
   for( auto& p : histos ) {
     p.second->Write();
   }
   ofile->Close();
   
}



void process( DVeffObj& dveff, PVObj& pv,EventInfoObj& ei, const size_t i_llp, Stats& stats ) {
  
  DVeff::DVeffVtx v;
  
  TVector3 pvPos( pv.x, pv.y, pv.z );
  setup( v, dveff, i_llp );
  
  auto all           = []( const DVeff::DVeffVtx& /*v*/ ) -> bool {
    return true;
  };
  
  auto hasRecoVertex = []( const DVeff::DVeffVtx& v ) -> bool {
    return v.recoVertices.size()>0;
  };
  
  auto hasAtLeast2ReconstructedDescendents = []( const DVeff::DVeffVtx& v ) -> bool {
    return std::count_if( v.descendents.begin(), v.descendents.end(), []( const DVeff::Descendent& d ) { return d.isReconstructed != 0; } ) >= 2;
  };
  
  auto hasAtLeast2SelectedDescendents = []( const DVeff::DVeffVtx& v ) -> bool {
    return std::count_if( v.descendents.begin(), v.descendents.end(), []( const DVeff::Descendent& d ) { return d.isSelected != 0; } ) >= 2;
  };
  
  const auto r = ( v.llp.decay - pvPos ).Perp();
  const auto mult_reconstructible   = std::count_if( v.descendents.begin(), v.descendents.end(), []( const DVeff::Descendent& d ) { return true; } );
  const auto mult_tracks   = std::count_if( v.descendents.begin(), v.descendents.end(), []( const DVeff::Descendent& d ) { return d.isReconstructed != 0; } );
  const auto mult_selected = std::count_if( v.descendents.begin(), v.descendents.end(), []( const DVeff::Descendent& d ) { return d.isSelected != 0; } );
  
  const auto p4_reconstructible
    = std::accumulate( v.descendents.begin(), v.descendents.end(), TLorentzVector{},
                       []( TLorentzVector& sum, const DVeff::Descendent& d ) { return sum += d.p4; } );
  const auto p4_tracks
    = std::accumulate( v.descendents.begin(), v.descendents.end(), TLorentzVector{},
                       [](  TLorentzVector& sum, const DVeff::Descendent& d ) { return d.isReconstructed? sum += d.p4 : sum; } );
  const auto p4_selected
    = std::accumulate( v.descendents.begin(), v.descendents.end(), TLorentzVector{},
                       [](  TLorentzVector& sum, const DVeff::Descendent& d ) { return d.isSelected? sum += d.p4 : sum; } );
  
  //v.print( i_llp );
  //if( hasAtLeast2ReconstructedDescendents(v) ) v.print( i_llp );
  
  // name, denom_condition, numer_condition, variable
  fillEff<TH1F>( "recoEff_r",           all(v),                                 hasRecoVertex(v), r );
  fillEff<TH1F>( "recoEff_openAngle",   all(v),                         hasRecoVertex(v), v.llp.childOpenAngle );

  fillEff<TH1F>( "recoEff_fiducial1_r", hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), r );
  fillEff<TH1F>( "recoEff_fiducial2_r", hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), r );
  fillEff<TH1F>( "recoEff_fiducial2_averageMu", hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), ei.averageMu );

  fillEff<TH1F>( "recoEff_fiducial1_averageMu", hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), ei.averageMu );
  fillEff<TH1F>( "acceptance_fiducial1_averageMu", all(v), hasAtLeast2ReconstructedDescendents(v), ei.averageMu );

  fillEff<TH1F>( "recoEff_averageMu", all(v),      hasRecoVertex(v), ei.averageMu );
  fillEff<TH1F>( "recoEff_fiducial1_openAngle", hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), v.llp.childOpenAngle );
  fillEff<TH1F>( "recoEff_fiducial2_openAngle", hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), v.llp.childOpenAngle );  

 
  fillEff<TH1F>( "acceptance_fiducial1_r", all(v), hasAtLeast2ReconstructedDescendents(v), r );
  fillEff<TH1F>( "acceptance_fiducial2_r", all(v), hasAtLeast2SelectedDescendents(v), r );
  fillEff<TH1F>( "acceptance_fiducial1_openAngle", all(v), hasAtLeast2ReconstructedDescendents(v), v.llp.childOpenAngle );
  fillEff<TH1F>( "acceptance_fiducial2_openAngle", all(v), hasAtLeast2SelectedDescendents(v), v.llp.childOpenAngle );
  fillEff<TH1F>( "seedEff_r", hasAtLeast2ReconstructedDescendents(v), hasAtLeast2SelectedDescendents(v), r );
  fillEff<TH1F>( "seedEff_averageMu", hasAtLeast2ReconstructedDescendents(v), hasAtLeast2SelectedDescendents(v), ei.averageMu );
  fillEff<TH1F>( "seedEff_openAngle", hasAtLeast2ReconstructedDescendents(v), hasAtLeast2SelectedDescendents(v), v.llp.childOpenAngle );
  
  fillEff<TH1F>( "recoEff_fiducial2_mult", hasAtLeast2SelectedDescendents(v),      hasRecoVertex(v), mult_selected*1.0 );
  fillEff<TH1F>( "recoEff_fiducial1_mult", hasAtLeast2ReconstructedDescendents(v), hasRecoVertex(v), mult_selected*1.0 );

  fillEff<TH1F>( "recoEff_mult", all(v),      hasRecoVertex(v), mult_selected*1.0 );
  
  // Some histograms
  for( auto& tp: v.truthPositions ) {
    fillHist<TH2F>( "hist_OpeningAngleNTrk", v.llp.childOpenAngle,tp.nReconstructible );
  }

  for( auto& vtx: v.recoVertices ) {
    
    auto res1_r = (vtx.pos-pvPos).Perp() - (v.llp.decay-pvPos).Perp();
    auto res1_z = (vtx.pos - v.llp.decay).z();
    auto res1_rphi = r*(vtx.pos.Phi() - v.llp.decay.Phi());
    
    fillHist<TH2F>( "residual1_r", r, res1_r );
    fillHist<TH2F>( "residual1_z", r, res1_z );
    fillHist<TH2F>( "residual1_rphi", r, res1_rphi );

    fillHist<TH2F>( "residual1_r_mult", vtx.ntrk,res1_r );
    fillHist<TH2F>( "residual1_z_mult", vtx.ntrk,res1_z );
    fillHist<TH2F>( "residual1_rphi_mult", vtx.ntrk,res1_rphi );
    
    if( r < 30 ) {
      fillHist<TH2F>( "residual1_r_mult_r30", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_r30", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_r30", vtx.ntrk, res1_rphi );
    } else if( r < 50 ) {
      fillHist<TH2F>( "residual1_r_mult_r50", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_r50", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_r50", vtx.ntrk, res1_rphi );
    } else if( r < 85 ) {
      fillHist<TH2F>( "residual1_r_mult_r85", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_r85", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_r85", vtx.ntrk, res1_rphi );
    } else if( r < 125 ) {
      fillHist<TH2F>( "residual1_r_mult_r125", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_r125", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_r125", vtx.ntrk, res1_rphi );
    } else if( r < 300 ) {
      fillHist<TH2F>( "residual1_r_mult_r300", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_r300", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_r300", vtx.ntrk, res1_rphi );
    }
    if( r < 125 ) {
      fillHist<TH2F>( "residual1_r_mult_pixel", vtx.ntrk, res1_r );
      fillHist<TH2F>( "residual1_z_mult_pixel", vtx.ntrk, res1_z );
      fillHist<TH2F>( "residual1_rphi_mult_pixel", vtx.ntrk, res1_rphi );
    }
    
    if( vtx.truthPosIndex >= v.truthPositions.size() ) continue;
    
    auto& tpos = v.truthPositions.at( vtx.truthPosIndex ).pos;
    fillHist<TH2F>( "residual2_r", r, (vtx.pos-pvPos).Perp() - (tpos-pvPos).Perp() );
    fillHist<TH2F>( "residual2_z", r, (vtx.pos - tpos).z() );
    fillHist<TH2F>( "residual2_rphi", r, r*(vtx.pos.Phi() - tpos.Phi()) );
    
    fillHist<TH1F>( "matchScore", vtx.matchScore );
    
    fillHist<TH2F>( "mult_reco_r", r, vtx.ntrk );
    fillHist<TH2F>( "mult_recoCore_r", r, vtx.ntrk_nonAssociated );
  
    fillHist<TH2F>( "mass_reco_r", r, vtx.mass*1.e-3 );
    fillHist<TH2F>( "mass_recoCore_r", r, vtx.mass_nonAssociated*1.e-3 );
  
    fillHist<TH2F>( "multFrac_r", r, static_cast<double>(vtx.ntrk)/static_cast<double>(mult_tracks) );
    fillHist<TH2F>( "multFracCore_r", r, static_cast<double>(vtx.ntrk_nonAssociated)/static_cast<double>(mult_tracks) );
    fillHist<TH2F>( "massFrac_r", r, vtx.mass/p4_tracks.M() );
    fillHist<TH2F>( "massFracCore_r", r, vtx.mass_nonAssociated/p4_tracks.M() );
    
    fillEff<TH1F>( "selEff", true,  ( vtx.mass_nonAssociated > 10. && vtx.ntrk_nonAssociated >=5 ), 0 );
    fillEff<TH1F>( "selEff", true,  ( vtx.mass > 10. && vtx.ntrk >=5 ), 1 );
    
    break;
  }

  //JS3
  //Cluster truth vertices based on production position of LLP descendents (with threshold of 1mm)
  vector< std::pair<TVector3, int> > clusteredTruthVertices;
  
  for(auto& d : v.descendents ){
    
    bool alreadyExists { false };
    
    for(auto& cv : clusteredTruthVertices ) {
      
      if( ( d.prod - cv.first ).Mag() < 1.0 ) {
	cv.second++;
	alreadyExists = true;
      }
    }
    
    if(!alreadyExists) clusteredTruthVertices.emplace_back( std::pair<TVector3, int>( d.prod, 1 ) );
  }
  
  
  //Find maximum distance between any two clustered truth vertices (skipping vertices with less than two reconstructible descendents)
  double maxDistBetweenCTVs { -9999. };
  
  for( auto v1=clusteredTruthVertices.begin(); v1!=clusteredTruthVertices.end(); ++v1 ) {
    if (v1->second < 2) continue;
    
    // Hide: We must start from the next iterator so that we don't look at the identical one
    for( auto v2= std::next( clusteredTruthVertices.begin() ); v2!=clusteredTruthVertices.end(); ++v2 ) {
      if (v2->second < 2) continue;
      
      double dist = std::abs( (v1->first - v2->first).Mag() );
      if( dist > maxDistBetweenCTVs ) maxDistBetweenCTVs = dist;
    }
  }
  
  const double maxDistFromLLP { 10. };
  
  const int nReco_regulated = std::count_if( v.recoVertices.begin(), v.recoVertices.end(), 
                                             [&]( const DVeff::RecoVtx& r) {
                                               return ( r.matchScore > 0. ) && ( ( r.pos - v.llp.decay ).Mag() < maxDistFromLLP ) ;
                                             } );
  
  const bool isFiducial { nReco_regulated > 0 };
  const bool isSplit    { nReco_regulated > 1 };
  
  // inclusive counting
  if( true ) {
    fillEff<TH1F>( "splitRate_maxTruthDelta", isFiducial, isSplit, maxDistBetweenCTVs );
    fillEff<TH1F>( "splitRate_radius",        isFiducial, isSplit, v.llp.decay.Perp() );
    fillEff<TH1F>( "splitRate_trackMult",     isFiducial, isSplit, v.descendents.size() );
    if( isFiducial ) stats.nRecoSplitFiducialVertices+=weight;
    if( isSplit )    stats.nRecoSplitVertices+=weight;
  }
  
  //accidental split rate: den=exactly 1 truth vertex, num=subset with at least 2 reco vertices
  if( clusteredTruthVertices.size() == 1 ) {
    //fillEff<TH1F>( "accidentalSplitRate_maxTruthDelta", isFiducial, isSplit, maxDistBetweenCTVs ); // meaningless
    fillEff<TH1F>( "accidentalSplitRate_radius",        isFiducial, isSplit, v.llp.decay.Perp() );
    if( isFiducial ) stats.nTruthSingleVertices+=weight;
    if( isSplit    ) stats.nRecoAccidentalSplitVertices+=weight;
  }
  
  //merge rate: den=at least two truth vertices, num=subset with one reco vertex
  if( clusteredTruthVertices.size() > 1 ) {
    fillEff<TH1F>( "mergeRate_maxTruthDelta", isFiducial, isSplit, maxDistBetweenCTVs );
    fillEff<TH1F>( "mergeRate_radius",        isFiducial, isSplit, v.llp.decay.Perp() );
    if( isFiducial ) stats.nTruthMultiVertices+=weight;
    if( isSplit    ) stats.nRecoMergedVertices+=weight;
  }

  //JS3
    
  bool split { false };
  for( auto iv = v.recoVertices.begin(); iv != v.recoVertices.end(); ++iv ) {
    for( auto jv = std::next( iv ); jv != v.recoVertices.end(); ++jv ) {
      //if( (*iv).truthPosIndex != (*jv).truthPosIndex ) continue;
      auto res_r_i = ((*iv).pos - v.llp.decay).Mag();
      auto res_r_j = ((*jv).pos - v.llp.decay).Mag();
      if( (*iv).matchScore < 0. ) continue;
      if( (*jv).matchScore < 0. ) continue;
      //if( fabs(res_r_i) > 10.0 || fabs(res_r_j) > 10.0 ) continue;
      //if( fabs(res_r_i) > 5.0 || fabs(res_r_j) > 5.0 ) continue;
      auto dist = ( (*jv).pos - (*iv).pos ).Mag();
      if( dist > 10. ) continue;
      //std::cout << "split --> res_r = (" << res_r_i << ", " << res_r_j << "), pos_r = (" << (*iv).pos.Perp() << ", " << (*jv).pos.Perp() << "), distance = " << dist << ", score = " << (*iv).matchScore << ", " << (*jv).matchScore << std::endl;
      fillHist<TH1F>( "splitDistance", dist );
      split = true;
    }
  }
  if( split ) stats.nLLPs_split+=weight;
  
  fillHist<TH2F>( "mult_reconstructible_r", r, mult_reconstructible );
  fillHist<TH2F>( "mult_tracks_r", r, mult_tracks );
  fillHist<TH2F>( "mult_selected_r", r, mult_selected );
  
  fillHist<TH2F>( "mass_reconstructible_r", r, p4_reconstructible.M()*1.e-3 );
  fillHist<TH2F>( "mass_tracks_r", r, p4_tracks.M()*1.e-3 );
  fillHist<TH2F>( "mass_selected_r", r, p4_selected.M()*1.e-3 );

  stats.nLLPs+=weight;
  if( v.truthPositions.size() ) stats.nLLPs_hasTruthPos+=weight;
  if( v.recoVertices.size()   ) stats.nLLPs_hasReco+=weight;
  if( v.recoVertices.size()>1 ) stats.nLLPs_hasMoreThanOneRecos+=weight;
}





void DVeff::DVeffVtx::print(size_t index) {
  std::cout << Form("LLP [%lu] pid = %8d, mass = %.3e, decay(r,z) = (%8.2f, %8.2f) mm",
                    index, llp.pid, llp.p4.M(), llp.decay.Perp(), llp.decay.z() )
            << std::endl;
  std::cout << "\n" << std::endl;
  std::cout << Form("  * Descendents size = %lu, sum pt = %.3e GeV, sum mass = %.3e GeV; nReco %lu, nSel %lu",
                    descendents   .size(),
                    descendentsSumP4.Pt()*1.e-3, descendentsSumP4.M()*1.e-3,
                    std::accumulate( descendents.begin(), descendents.end(), 0UL, []( size_t s, DVeff::Descendent& d ) { return s + d.isReconstructed; } ),
                    std::accumulate( descendents.begin(), descendents.end(), 0UL, []( size_t s, DVeff::Descendent& d ) { return s + d.isSelected; } ) )
                    << std::endl;
  std::cout << Form("  * TruthPos    size = %lu", truthPositions.size() ) << std::endl;
  std::cout << Form("  * RecoVtx     size = %lu", recoVertices  .size() ) << std::endl;
  std::cout << "\n" << std::endl;
  
  for( size_t it = 0; it < truthPositions.size(); ++it ) {
    const auto& tpos = truthPositions.at(it);
    std::cout << Form("  - TruthPos [%lu]: (r,z) = (%8.2f, %8.2f) mm", it, tpos.pos.Perp(), tpos.pos.z() );
    std::cout << Form(", sum pt = %.3e GeV, sum mass = %8.3e GeV, #descendents %lu",
                      tpos.reconstructibleSumP4.Pt()*1.e-3, 
                      tpos.reconstructibleSumP4.M()*1.e-3,
                      tpos.nReconstructible ) << std::endl;
  }
  std::cout << std::endl;
  for( size_t iv = 0; iv < recoVertices.size(); ++iv ) {
    const auto& vtx = recoVertices.at(iv);
    std::cout << Form("  - RecoVtx [%lu]: (r,z) = (%8.2f, %8.2f) mm", iv, vtx.pos.Perp(), vtx.pos.z() ) << std::endl;
    std::cout << Form("       * TruthPos [%lu], matchScore = %.3f", vtx.truthPosIndex, vtx.matchScore) << std::endl;
    std::cout << Form("       * residual mag  : %.3f um", vtx.residual.Mag()*1.e3 ) << std::endl;
    std::cout << Form("       * ntrk          : %lu (nonAssociated %lu)", vtx.ntrk, vtx.ntrk_nonAssociated ) << std::endl;
    std::cout << Form("       * mass          : %.3e GeV (nonAssociated %.3e GeV)", vtx.mass*1.e-3, vtx.mass_nonAssociated*1.e-3 ) << std::endl;
    std::cout << Form("       * filtered mass : %.3e GeV (nonAssociated %.3e GeV)", vtx.filtered_mass*1.e-3, vtx.filtered_mass_nonAssociated*1.e-3 ) << std::endl;
  }
  std::cout << "\n" << std::endl;
  std::cout << "===================================================\n" <<  std::endl;
}




void setup( DVeff::DVeffVtx& v, DVeffObj& dveff, const size_t i_llp ) {
  
  using namespace DVeff;
  
  // LLP

#define GET_LLP( name ) dveff.LLP_ ## name ->at( i_llp )
  
  v.llp.decay.SetXYZ( GET_LLP( decay_x ), GET_LLP( decay_y ), GET_LLP( decay_z ) );
  
  v.llp.p4.SetPtEtaPhiM( GET_LLP( pt ), GET_LLP( eta ), GET_LLP( phi ), GET_LLP( mass ) );
  
  v.llp.pid = GET_LLP( pid );

  v.llp.childOpenAngle = GET_LLP( childOpenAngle );
  
  // Descendents sum
  v.descendentsSumP4.SetPtEtaPhiM( dveff.descendent_sum_pt  ->at(i_llp),
                                   dveff.descendent_sum_eta ->at(i_llp),
                                   dveff.descendent_sum_phi ->at(i_llp),
                                   dveff.descendent_sum_mass->at(i_llp)  );

  // Descendent particles
  
  for( size_t idesc = 0; idesc <dveff.descendent_size->at(i_llp); ++idesc ) {
#define GET_DESC( name ) dveff.descendent_ ## name ->at(i_llp).at(idesc)
    
    auto desc = Descendent{};
    
    desc.charge = GET_DESC( charge );
    
    desc.pid    = GET_DESC( pid );
    
    desc.p4.SetPtEtaPhiM( GET_DESC( pt ), GET_DESC( eta ), GET_DESC( phi ), GET_DESC( mass ) );
    
    desc.prod.SetXYZ( GET_DESC( prod_x ), GET_DESC( prod_y ), GET_DESC( prod_z ) );
    desc.decay.SetXYZ( GET_DESC( decay_x ), GET_DESC( decay_y ), GET_DESC( decay_z ) );
    
    desc.isReconstructed = GET_DESC( isReconstructed );
    desc.isSelected      = GET_DESC( isSelected );
    
    desc.truthPosIndex   = GET_DESC( truthPosIndex );
    
    v.descendents.emplace_back( desc );
  }
  
  
  // TruthVertexPos
  
  for( size_t ipos = 0; ipos < dveff.truthPos_r->at(i_llp).size(); ++ipos ) {
#define GET_TPOS( name ) dveff.truthPos_ ## name ->at(i_llp).at(ipos)
    
    auto pos = TruthPos{};
    
    pos.pos.SetXYZ( GET_TPOS(x), GET_TPOS(y), GET_TPOS(z) );
    
    pos.nReconstructible = GET_TPOS( nReconstructible );
    pos.reconstructibleSumP4.SetPtEtaPhiM( GET_TPOS( sum_pt ), GET_TPOS( sum_eta ), GET_TPOS( sum_phi ), GET_TPOS( reconstructibleMass ) );
    
    v.truthPositions.emplace_back( pos );
  }
  

  // Reco-vtx
  for( size_t ireco = 0; ireco < dveff.recoVtx_LLP_index->size(); ++ireco ) {
#define GETRECO( name ) dveff.recoVtx_ ## name ->at(ireco)
    
    // Skip if the LLP index doesn't match.
    
    if( GETRECO( LLP_index ) != i_llp ) continue;
    
    RecoVtx vtx;
    
    vtx.truthPosIndex = GETRECO( truthPos_index );
    vtx.matchScore    = GETRECO( matchScore );
    
    vtx.pos.SetXYZ( GETRECO(x), GETRECO(y), GETRECO(z) );
    vtx.residual.SetXYZ( GETRECO( residual_x ), GETRECO( residual_y ), GETRECO( residual_z ) );
    
    vtx.chi2                        = GETRECO( chi2 );
    vtx.chi2_core                   = GETRECO( chi2_core );
    vtx.mass                        = GETRECO( mass );
    vtx.mass_nonAssociated          = GETRECO( mass_nonAssociated );
    vtx.ntrk                        = GETRECO( ntrk );
    vtx.ntrk_nonAssociated          = GETRECO( ntrk_nonAssociated );
    vtx.filtered_mass               = GETRECO( filtered_mass );
    vtx.filtered_mass_nonAssociated = GETRECO( filtered_mass_nonAssociated );
    vtx.filtered_ntrk               = GETRECO( filtered_ntrk );
    vtx.filtered_ntrk_nonAssociated = GETRECO( filtered_ntrk_nonAssociated );
    
    v.recoVertices.emplace_back( vtx );
  }  
}

