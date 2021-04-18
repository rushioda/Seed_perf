#include <include/NtupleObj.h>

#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TEfficiency.h>
#include <TGraphAsymmErrors.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TStyle.h>

#include <iostream>
#include <cassert>
#include <algorithm>

void efficiency( const char* filename, const char* treename="vsitree" ) {

    gStyle->SetOptStat(0);


//Reset ROOT and connect tree file
   gROOT->Reset();
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject( filename );
   if (!f) {
      f = new TFile( filename );
   }
   auto* tree = (TTree*)( f->FindObjectAny(treename) );

//Declaration of leaves types

   EventInfoObj ei;
   TrackObj trkPrescaled;
   TrackObj trkSel;
   PVObj pv;
   DVObj dv;
   TruthVtxObj truth;

   // Set branch addresses.
   setBranchAddress( tree, ei );
   setBranchAddress( tree, trkPrescaled, "IDTracksPrescaled" );
   setBranchAddress( tree, trkSel,       "IDTracksVsiSelected" );
   setBranchAddress( tree, pv );
   setBranchAddress( tree, dv );
   //setBranchAddress( tree, truth, "Stop1Truth" );
   setBranchAddress( tree, truth, "InvisibleFromHiggsTruth" );
 

   Long64_t nentries = tree->GetEntries();

   std::vector<double> rbins { 1.0, 2.0, 3.0, 5.0, 7.0, 10.0, 14.0, 20.0, 30.0, 50.0, 70.0, 100.0, 140.0, 200.0, 300.0, 500.0 };

   auto *hr_denom = new TH1F("hr_denom", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)));
   auto *hr_numer = new TH1F("hr_numer", ";r [mm];Entries", rbins.size()-1, &(rbins.at(0)));
   auto *hresidual_r = new TH2F("hresidual_r", ";r [mm];3D residual [mm]", rbins.size()-1, &(rbins.at(0)), 1000, 0.0, 5.0 );
   

   for (Long64_t i=0; i<nentries;i++) {
       tree->GetEntry(i);

       std::vector<double> distances;

       for( size_t itru = 0; itru < truth.r->size(); itru++ ) {

           assert( truth.r->size() == truth.z->size()   );
           assert( truth.r->size() == truth.phi->size() );

           TVector3 truth_pos;
           truth_pos.SetXYZ( truth.r->at(itru)*cos(truth.phi->at(itru)),
                             truth.r->at(itru)*sin(truth.phi->at(itru)),
                             truth.z->at(itru) );

           hr_denom->Fill( truth_pos.Perp() );
           
           for( size_t idv = 0; idv < dv.x->size(); idv++ ) {
               assert( dv.x->size() == dv.y->size() );
               assert( dv.x->size() == dv.z->size() );
               
               TVector3 reco_pos;
               
               reco_pos.SetXYZ( dv.x->at(idv), dv.y->at(idv), dv.z->at(idv) );
               
               const double pos_diff = (reco_pos - truth_pos).Mag();
               distances.emplace_back( pos_diff );
           }

           if( 0 == distances.size() ) continue;
           
           auto min = std::min_element( distances.begin(), distances.end() );
           auto minIndex = min - distances.begin();

           if( *min <  5.0 ) {
               hr_numer->Fill( truth_pos.Perp() );
               hresidual_r ->Fill( truth_pos.Perp(), *min );
               break;
           }
       
       }

   }

   for( int ibin=0; ibin<static_cast<int>(rbins.size())-1; ibin++) {
       hr_denom->SetBinContent( ibin+1, hr_denom->GetBinContent(ibin+1)/hr_denom->GetBinWidth(ibin+1) );
       hr_denom->SetBinError  ( ibin+1, hr_denom->GetBinError(ibin+1)/hr_denom->GetBinWidth(ibin+1) );
       
       hr_numer->SetBinContent( ibin+1, hr_numer->GetBinContent(ibin+1)/hr_numer->GetBinWidth(ibin+1) );
       hr_numer->SetBinError  ( ibin+1, hr_numer->GetBinError(ibin+1)/hr_numer->GetBinWidth(ibin+1) );
   }
   hr_denom->SetYTitle("Entries / mm ");
   hr_numer->SetYTitle("Entries / mm ");

   auto* cyield = new TCanvas("cyield", "", 0, 0, 800, 600);
   hr_denom->SetLineColor(kBlack);
   hr_denom->Draw("hist,e");
   hr_numer->Draw("hist,e,same");
   hr_numer->SetLineColor(kAzure);
   gPad->SetLogy();
   gPad->SaveAs("yield.pdf");

   auto* cres = new TCanvas("cres", "", 0, 0, 800, 600);
   hresidual_r->Draw("colz");
   gPad->SetLogy();
   gPad->SaveAs("3Dresidual.pdf");

   auto* ceff = new TCanvas("ceff", "", 0, 0, 800, 600);
   auto* feff = ceff->DrawFrame(1.0, 0.0, 500.0, 1.0);
   gPad->SetLogx();

   auto* heff = new TEfficiency( *hr_numer, *hr_denom );
   auto* eff_gr = heff->CreateGraph();
   eff_gr->SetMarkerStyle(20);
   eff_gr->SetLineWidth(2);
   eff_gr->Draw("pe+");
   gPad->SaveAs("eff.pdf");

   
}
