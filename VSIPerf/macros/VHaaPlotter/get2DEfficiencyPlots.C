#include <TCanvas.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TPad.h>
#include <TLatex.h>
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

void get2DEfficiencyPlots()
{

   TString path = "../outputs/";

   std::vector< pair <string, string> > variables;
   variables.push_back(make_pair("actualMu", "r"));
   variables.push_back(make_pair("llp_pt", "r"));
   variables.push_back(make_pair("z", "r"));
   variables.push_back(make_pair("z", "actualMu"));

   std::vector< pair <string, string> > efficiencies;
   efficiencies.push_back(make_pair("recoEff", "#varepsilon"));
   efficiencies.push_back(make_pair("recoEff_algo", "#varepsilon_{algo}"));
   efficiencies.push_back(make_pair("recoEff_core", "#varepsilon_{core}"));
   efficiencies.push_back(make_pair("acceptance_recoEff_seed", "A*#varepsilon_{seed}"));
   efficiencies.push_back(make_pair("acceptance", "A"));
   efficiencies.push_back(make_pair("recoEff_seed", "#varepsilon_{seed}"));


   TEfficiency* eff;
   TH2F* h2d;

   TLatex l;
   l.SetNDC();
   l.SetTextColor(kBlack);

   /////////////////////////////////////////
   // different lifetimes 1, 10, 100 mm
   /////////////////////////////////////////

   std::vector<Int_t> ctaus = {100, 10, 1};
   //      std::vector<TString> decays = {"4u", "4b", "4tau"};
   std::vector <Int_t> tmp_masses = {15, 55}; //plot two mass regimes

   std::map<TString, TCanvas*> c_taus;
   std::map<TString, TLegend*> l_taus;

   for (auto tmp_mass : tmp_masses) {
      for (auto ctau : ctaus) {
         for (auto pair_var : variables) {
            auto var1 = pair_var.first;
            auto var2 = pair_var.second;
            TString var_name = var1 + "_vs_" +  var2;
            for (auto pair : efficiencies) {
               auto eff_name = pair.second;
               auto eff_tag = pair.first + "_" + var_name + Form("_%d_%d", tmp_mass, ctau);
               c_taus[eff_tag] = new TCanvas("c_" + eff_tag, "c_" + eff_tag, 700, 600);
               l_taus[eff_tag] = new TLegend(0.2, 0.65, 0.5, 0.85);
               l_taus[eff_tag]->SetFillColor(kWhite);
               l_taus[eff_tag]->SetBorderSize(0);
            }
         }
         TString filename = TString::Format("WpH_H125_a%da%d_4b_ctau%d_mc16d.root", tmp_mass, tmp_mass, ctau);
         TFile file(path + filename);
         //    if (!file) continue;
         for (auto pair_var : variables) {
            auto var1 = pair_var.first;
            auto var2 = pair_var.second;
            TString var_name = var1 + "_vs_" +  var2;
            for (auto pair : efficiencies) {
               auto eff_name = pair.second;
               auto eff_tag = pair.first + "_" + var_name + Form("_%d_%d", tmp_mass, ctau);
               eff = (TEfficiency*)file.Get(pair.first + "_" + var_name);
               h2d = (TH2F*)eff->CreateHistogram();

               if (!eff) {
                  cout << "Missing TEfficiency or Histogram with tag: " +  pair.first + "_" + var_name << endl;
                  continue;
               }
               c_taus[eff_tag]->cd();
               c_taus[eff_tag]->SetRightMargin(0.20);
               c_taus[eff_tag]->SetLogx();
               c_taus[eff_tag]->SetLogy();
               if (eff_tag.Contains("vs_actualMu")) {
                  c_taus[eff_tag]->SetLogy(false);
               } else if (eff_tag.Contains("actualMu_vs")) {
                  c_taus[eff_tag]->SetLogx(false);
               }

               h2d->Draw("COLZ");
               h2d->GetZaxis()->SetRangeUser(0, 1);
               h2d->GetZaxis()->SetTitle((TString)eff_name);
               l.SetTextFont(72);
               l.SetTextSize(0.045);
               l.DrawLatex(0.2, 0.87, "ATLAS");
               l.SetTextFont(42);
               l.DrawLatex(0.33, 0.87, "Internal");
               c_taus[eff_tag]->SaveAs(TString::Format("img/c2D_WpH_H125_a%da%d_4b_ctau%d_mc16d_%s.pdf", tmp_mass, tmp_mass, ctau, (pair.first + "_" + var_name).Data()));
            }
         }
      }
   }
}
