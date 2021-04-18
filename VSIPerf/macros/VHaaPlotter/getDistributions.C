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

void getDistributions(const Bool_t doLifetimes = true, const Bool_t  doMasses = true, const Bool_t doDecays = true)
{

   TString path = "../outputs/2018.11.26/10mm/";

   TString ylabel = "Entries";
   std::vector< pair <TString, TString> > variables;
   variables.push_back(make_pair("matchScore", "matchScore"));

   Int_t color = 1;
   TH1F* histo;

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
         color = 1;

         for (auto pair : variables) {
            auto var = pair.first;
            auto var_name = var + Form("_%d", tmp_mass);
            c_taus[var_name] = new TCanvas("c_taus_" + var_name, "c_taus_" + var_name);
            l_taus[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
            l_taus[var_name]->SetFillColor(kWhite);
            l_taus[var_name]->SetBorderSize(0);
         }

         for (auto ctau : ctaus) {
            TString filename = TString::Format("WpH_H125_a%da%d_4b_ctau%d_mc16d.root", tmp_mass, tmp_mass, ctau);
            TFile file(path + filename);
            TString drawOption = "";

            for (auto pair : variables) {
               auto var = pair.first;
               auto var_name = var + Form("_%d", tmp_mass);
               auto xlabel = pair.second;

               if (ctaus.at(0) != ctau)   drawOption = "same";
               histo = (TH1F*)file.Get(var.Data());
               histo->SetTitle(TString::Format("m_{a} = %d GeV, c#tau = %d mm", tmp_mass, ctau));
               histo->SetLineColor(color);
               histo->SetLineWidth(2);
               c_taus[var_name]->cd();
               histo->GetXaxis()->SetTitle(xlabel);
               histo->GetYaxis()->SetTitle(ylabel);
               histo->DrawNormalized(drawOption);
               //          l_taus[var_name]->AddEntry(histo, TString::Format("m_{a} = %d GeV, c#tau = %d mm", tmp_mass, ctau), "l");
               if (ctaus.at(ctaus.size() - 1) == ctau) {
                  //                  l_taus[var_name]->Draw();
                  c_taus[var_name]->BuildLegend(0.2, 0.65, 0.5, 0.85);
                  l.SetTextFont(72);
                  l.SetTextSize(0.045);
                  l.DrawLatex(0.2, 0.87, "ATLAS");
                  l.SetTextFont(42);
                  l.DrawLatex(0.33, 0.87, "Internal");
                  c_taus[var_name]->SaveAs(TString::Format("img/ctau_WpH_H125_a%da%d_4b_mc16d_%s.pdf", tmp_mass, tmp_mass, (var).Data()));
                  c_taus[var_name]->SaveAs(TString::Format("img/ctau_WpH_H125_a%da%d_4b_mc16d_%s.C", tmp_mass, tmp_mass, (var).Data()));
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

      for (auto pair : variables) {
         auto var_name = pair.first;
         c_masses[var_name] = new TCanvas("c_masses_" + var_name, "c_masses_" + var_name);
         l_masses[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
         l_masses[var_name]->SetFillColor(kWhite);
         l_masses[var_name]->SetBorderSize(0);
      }
      color = 1;
      for (auto mass : masses) {
         Int_t tmp_ctau = 10;
         TString filename = TString::Format("WpH_H125_a%da%d_4b_ctau%d_mc16d.root", mass, mass, tmp_ctau);
         TFile file(path + filename);
         TString drawOption = "";

         for (auto pair : variables) {
            auto var_name = pair.first;
            auto xlabel = pair.second;

            if (masses.at(0) != mass) drawOption = "same";
            histo = (TH1F*)file.Get(var_name);
            histo->SetTitle(TString::Format("m_{a} = %d GeV, c#tau = %d mm", mass, tmp_ctau));
            histo->SetLineColor(color);
            c_masses[var_name]->cd();
            histo->GetXaxis()->SetTitle(xlabel);
            histo->GetYaxis()->SetTitle(ylabel);
            c_masses[var_name]->cd();
            histo->DrawNormalized(drawOption);
            //            l_masses[var_name]->AddEntry(histo, TString::Format("m_{a} = %d GeV, c#tau = %d mm", mass, tmp_ctau), "l");

            if (masses.at(masses.size() - 1) == mass) {
               //               l_masses[var_name]->Draw();
               c_masses[var_name]->BuildLegend(0.2, 0.65, 0.5, 0.85);
               l.SetTextFont(72);
               l.SetTextSize(0.045);
               l.DrawLatex(0.2, 0.87, "ATLAS");
               l.SetTextFont(42);
               l.DrawLatex(0.33, 0.87, "Internal");
               c_masses[var_name]->SaveAs(TString::Format("img/mass_WpH_H125_4b_ctau%d_mc16d_%s.pdf", tmp_ctau, var_name.Data()));
               c_masses[var_name]->SaveAs(TString::Format("img/mass_WpH_H125_4b_ctau%d_mc16d_%s.C", tmp_ctau, var_name.Data()));
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
      for (auto pair : variables) {
         auto var_name = pair.first;
         c_decays[var_name] = new TCanvas("c_decays_" + var_name, "c_decays_" + var_name);
         l_decays[var_name] = new TLegend(0.2, 0.65, 0.5, 0.85);
         l_decays[var_name]->SetFillColor(kWhite);
         l_decays[var_name]->SetBorderSize(0);
      }

      color = 1;
      for (auto decay : decays) {
         Int_t tmp_mass = 55;
         Int_t tmp_ctau = 100;
         TString filename = TString::Format("WpH_H125_a%da%d_%s_ctau%d_mc16d.root", tmp_mass, tmp_mass, decay.Data(), tmp_ctau);
         TFile file(path + filename);

         TString drawOption = "";

         for (auto pair : variables) {
            auto var_name = pair.first;
            auto xlabel = pair.second;

            if (decays.at(0) != decay) drawOption = "same";
            histo = (TH1F*)file.Get(var_name);
            //       cout << path + filename + "/" + var_name << endl;
            histo->SetLineColor(color);
            histo->SetTitle(TString::Format("%s, m_{a} = %d GeV, c#tau = %d mm", decay.Data(), tmp_mass, tmp_ctau));
            histo->GetXaxis()->SetTitle(xlabel);
            histo->GetYaxis()->SetTitle(ylabel);
            c_decays[var_name]->cd();
            histo->DrawNormalized(drawOption);
            //       l_decays[var_name]->AddEntry(histo, TString::Format("%s, m_{a} = %d GeV, c#tau = %d mm", decay.Data(), tmp_mass, tmp_ctau), "l");
            //       l_decays[var_name]->Print();
            if (decays.at(decays.size() - 1) == decay) {
               c_decays[var_name]->BuildLegend(0.2, 0.65, 0.5, 0.85);
               //       l_decays[var_name]->Draw();
               l.SetTextFont(72);
               l.SetTextSize(0.045);
               l.DrawLatex(0.2, 0.87, "ATLAS");
               l.SetTextFont(42);
               l.DrawLatex(0.33, 0.87, "Internal");
               c_decays[var_name]->cd();
               c_decays[var_name]->SaveAs(TString::Format("img/10mm/decay_WpH_H125_a%da%d_ctau%d_mc16d_%s.pdf", tmp_mass, tmp_mass, tmp_ctau, var_name.Data()));
               c_decays[var_name]->SaveAs(TString::Format("img/10mm/decay_WpH_H125_a%da%d_ctau%d_mc16d_%s.C", tmp_mass, tmp_mass, tmp_ctau, var_name.Data()));
            }
         }
         color++; //change with decay
         file.Close();
      }
   }
}
