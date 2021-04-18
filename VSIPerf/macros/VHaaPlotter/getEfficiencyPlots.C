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

void getEfficiencyPlots(const Bool_t doLifetimes = true, const Bool_t  doDecays = true, const Bool_t doMasses = true)
{

   TString path = "../outputs/2018.11.26/100um/";
   TString output_path = "img/100um/";

   std::vector<TString> variables = {
      //1D
      "r",
      "actualMu",
//      "llp_ctau",
//      "llp_pt",
//      "llp_dR",
//      "z",
   };

   std::vector< pair <string, string> > efficiencies;
   efficiencies.push_back(make_pair("recoEff", "#varepsilon"));
   efficiencies.push_back(make_pair("recoEff_algo", "#varepsilon_{algo}"));
   efficiencies.push_back(make_pair("recoEff_core", "#varepsilon_{core}"));
   efficiencies.push_back(make_pair("acceptance_recoEff_seed", "A*#varepsilon_{seed}"));
   efficiencies.push_back(make_pair("acceptance", "A"));
   efficiencies.push_back(make_pair("recoEff_seed", "#varepsilon_{seed}"));



   Int_t color = 1;
   TEfficiency* eff;
   //   TEfficiency* eff;

   TLatex l;
   l.SetNDC();
   l.SetTextColor(kBlack);

   /////////////////////////////////////////
   // different lifetimes 1, 10, 100 mm
   /////////////////////////////////////////

   if (doLifetimes) {

      std::vector<Int_t> ctaus = {100, 10, 1};

      std::vector <Int_t> tmp_masses = {15, 55}; //plot two mass regimes

      std::map<TString, TCanvas*> c_taus;
      std::map<TString, TLegend*> l_taus;

      for (auto tmp_mass : tmp_masses) {
         for (auto var : variables) {
            for (auto pair : efficiencies) {
               auto eff_name = pair.first;
               auto var_name = eff_name + "_" + var + Form("_%d", tmp_mass);
               c_taus[var_name] = new TCanvas("c_taus_" + var_name, "c_taus_" + var_name);
               l_taus[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
               l_taus[var_name]->SetFillColor(kWhite);
               l_taus[var_name]->SetBorderSize(0);
            }
         }

         for (auto ctau : ctaus) {
            TString filename = TString::Format("WpH_H125_a%da%d_4b_ctau%d_mc16d.root", tmp_mass, tmp_mass, ctau);
            TFile file(path + filename);
            TString drawOption = "";

            for (auto var : variables) {
               for (auto pair : efficiencies) {
                  auto eff_name = pair.first;
                  auto var_name = eff_name + "_" + var + Form("_%d", tmp_mass);
                  auto ylabel = pair.second;

                  if (ctaus.at(0) != ctau)   drawOption = "same";
                  eff = (TEfficiency*)file.Get(eff_name + "_" + var);
                  eff->SetLineColor(color);
                  eff->SetLineWidth(2);
                  c_taus[var_name]->cd();
                  c_taus[var_name]->SetLogx();
                  if (var == "actualMu" || var == "llp_dR") {
                     c_taus[var_name]->SetLogx(false);
                  }
                  //       eff->SetTitle("; " + xlabel + " ; " + ylabel);
                  eff->Draw(drawOption);
                  gPad->Update();
                  auto graph = eff->GetPaintedGraph();
                  //                  graph->GetYaxis()->SetTitle((TString)ylabel);
                  graph->SetMinimum(0);
                  graph->SetMaximum(1.1);
                  gPad->Update();
                  l_taus[var_name]->AddEntry(eff, TString::Format("m_{a} = %d GeV, c#tau = %d mm", tmp_mass, ctau), "l");
                  if (ctaus.at(ctaus.size() - 1) == ctau) {
                     l_taus[var_name]->Draw();
                     l.SetTextFont(72);
                     l.SetTextSize(0.045);
                     l.DrawLatex(0.2, 0.87, "ATLAS");
                     l.SetTextFont(42);
                     l.DrawLatex(0.33, 0.87, "Internal");
                     c_taus[var_name]->SaveAs(TString::Format(output_path + "ctau_WpH_H125_a%da%d_4b_mc16d_%s.pdf", tmp_mass, tmp_mass, (eff_name + "_" + var).Data()));
                     c_taus[var_name]->SaveAs(TString::Format(output_path + "ctau_WpH_H125_a%da%d_4b_mc16d_%s.C", tmp_mass, tmp_mass, (eff_name + "_" + var).Data()));
                  }
               }
            }
            color++; //change with ctau
         }
      }
   }


   /////////////////////////////////////////
   // different masses 15, 55
   /////////////////////////////////////////
   if (doMasses) {

      std::vector<Int_t> masses = {15, 55};

      std::map<TString, TCanvas*> c_masses;
      std::map<TString, TLegend*> l_masses;

      for (auto var : variables) {
         for (auto pair : efficiencies) {
            auto eff_name = pair.first;
            auto var_name = eff_name + "_" + var;
            c_masses[var_name] = new TCanvas("c_masses_" + var_name, "c_masses_" + var_name);
            l_masses[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
            l_masses[var_name]->SetFillColor(kWhite);
            l_masses[var_name]->SetBorderSize(0);
         }
      }
      color = 1;
      for (auto mass : masses) {
         Int_t tmp_ctau = 100;
         TString filename = TString::Format("WpH_H125_a%da%d_4b_ctau%d_mc16d.root", mass, mass, tmp_ctau);
         TFile file(path + filename);
         TString drawOption = "";

         for (auto var : variables) {
            for (auto pair : efficiencies) {
               auto eff_name = pair.first;
               auto var_name = eff_name + "_" + var;
               auto ylabel = pair.second;

               TString xlabel = "";
               if (masses.at(0) != mass) drawOption = "same";
               eff = (TEfficiency*)file.Get(var_name);
               eff->SetLineColor(color);
               eff->SetLineWidth(2);
               c_masses[var_name]->cd();
               c_masses[var_name]->SetLogx();
               if (var == "actualMu" || var == "llp_dR") {
                  c_masses[var_name]->SetLogx(false);
               }
               //               eff->SetTitle("; " + xlabel + " ; " + ylabel);
               eff->Draw(drawOption);
               gPad->Update();
               auto graph = eff->GetPaintedGraph();
               graph->SetMinimum(0);
               graph->SetMaximum(1.1);
               gPad->Update();
               l_masses[var_name]->AddEntry(eff, TString::Format("m_{a} = %d GeV, c#tau = %d mm", mass, tmp_ctau), "l");
               if (masses.at(masses.size() - 1) == mass) {
                  l_masses[var_name]->Draw();
                  l.SetTextFont(72);
                  l.SetTextSize(0.045);
                  l.DrawLatex(0.2, 0.87, "ATLAS");
                  l.SetTextFont(42);
                  l.DrawLatex(0.33, 0.87, "Internal");
                  c_masses[var_name]->SaveAs(TString::Format(output_path + "mass_WpH_H125_4b_ctau%d_mc16d_%s.pdf", tmp_ctau, var_name.Data()));
                  c_masses[var_name]->SaveAs(TString::Format(output_path + "mass_WpH_H125_4b_ctau%d_mc16d_%s.C", tmp_ctau, var_name.Data()));
               }
            }
         }
         color++; //change with mass
      }
   }

   /////////////////////////////////////////
   // different decays bb, tautau, uu
   /////////////////////////////////////////
   if (doDecays) {

      std::vector<TString> decays = {"4u", "4b", "4tau"};

      std::map<TString, TCanvas*> c_decays;
      std::map<TString, TLegend*> l_decays;
      for (auto var : variables) {
         for (auto pair : efficiencies) {
            auto eff_name = pair.first;
            auto var_name = eff_name + "_" + var;
            c_decays[var_name] = new TCanvas("c_decays_" + var_name, "c_decays_" + var_name);
            l_decays[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
            l_decays[var_name]->SetFillColor(kWhite);
            l_decays[var_name]->SetBorderSize(0);
         }
      }
      color = 1;
      for (auto decay : decays) {
         Int_t tmp_mass = 55;
         Int_t tmp_ctau = 100;
         TString filename = TString::Format("WpH_H125_a%da%d_%s_ctau%d_mc16d.root", tmp_mass, tmp_mass, decay.Data(), tmp_ctau);
         TFile file(path + filename);
         TString drawOption = "";

         for (auto var : variables) {
            for (auto pair : efficiencies) {
               auto eff_name = pair.first;
               auto var_name = eff_name + "_" + var;
               auto ylabel = pair.second;
               //               TString xlabel = "";
               if (decays.at(0) != decay) drawOption = "same";
               eff = (TEfficiency*)file.Get(var_name);
               eff->SetLineColor(color);
               eff->SetLineWidth(2);
               c_decays[var_name]->cd();
               c_decays[var_name]->SetLogx();
               if (var == "actualMu" || var == "llp_dR") {
                  c_decays[var_name]->SetLogx(false);
               }
               //               eff->SetTitle("; " + xlabel + " ; " + ylabel);
               eff->Draw(drawOption);
               gPad->Update();
               auto graph = eff->GetPaintedGraph();
               graph->SetMinimum(0);
               graph->SetMaximum(1.1);
               gPad->Update();
               l_decays[var_name]->AddEntry(eff, TString::Format("%s, m_{a} = %d GeV, c#tau = %d mm", decay.Data(), tmp_mass, tmp_ctau), "l");
               if (decays.at(decays.size() - 1) == decay) {
                  l_decays[var_name]->Draw();
                  l.SetTextFont(72);
                  l.SetTextSize(0.045);
                  l.DrawLatex(0.2, 0.87, "ATLAS");
                  l.SetTextFont(42);
                  l.DrawLatex(0.33, 0.87, "Internal");
                  c_decays[var_name]->SaveAs(TString::Format(output_path + "decay_WpH_H125_a%da%d_ctau%d_mc16d_%s.pdf", tmp_mass, tmp_mass, tmp_ctau, var_name.Data()));
                  c_decays[var_name]->SaveAs(TString::Format(output_path + "decay_WpH_H125_a%da%d_ctau%d_mc16d_%s.C", tmp_mass, tmp_mass, tmp_ctau, var_name.Data()));
               }
            }
         }
         color++; //change with decay
      }
   }
}
