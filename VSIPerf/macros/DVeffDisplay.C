#include <include/NtupleObj.h>

#include <TFile.h>
#include <TROOT.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TEfficiency.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLine.h>

#include <iostream>
#include <algorithm>
#include <numeric>

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
  size_t nLLPs_split;
};

Stats::Stats() 
  : nEvents(0), nLLPs(0), nLLPs_hasTruthPos(0), nLLPs_hasReco(0), nLLPs_hasMoreThanOneRecos(0), nLLPs_split(0)
{}



void setup( DVeff::DVeffVtx& v, DVeffObj& dveff, const size_t i_llp );
void process( DVeffObj& dveff, PVObj& pv, DVObj& dv, const size_t i_llp, Stats& stats );



std::map<std::string, TObject*> histos;

using EffHistPair = std::pair<std::string, std::string>;

std::map<std::string, EffHistPair> effs;

template<class HISTO, typename ...REST>
void addHist( const std::string name, const std::string title, REST ...rest ) {
  auto* h = new HISTO(name.c_str(), title.c_str(), rest... );
  histos[name] = h;
}
  
template<class HISTO, typename ...REST>
void fillHist( const std::string name, REST ...rest ) {
  if( histos.find(name) == histos.end() ) {
    std::string err = Form( "%s: %s not found in the histogram list!!", __PRETTY_FUNCTION__, name.c_str() );
    std::cerr << err << std::endl;
    throw std::runtime_error( err );
  }
  ( dynamic_cast<HISTO*>( histos[name] ) )->Fill( rest... );
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
  
  denom->Sumw2();
  numer->Sumw2();
  
  TEfficiency *heff = new TEfficiency( *numer, *denom );
  heff->SetName( name.c_str() );
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
  numer->Scale(1.0/denom->Integral());
  
}


TGraph *gr_llp = new TGraph;
TGraph *gr_desc = new TGraph;
TGraph *gr_recovtx = new TGraph;

std::vector<TLine*> desc_lines_unreco;
std::vector<TLine*> desc_lines_reco;
std::vector<TLine*> dv_trks;

void DVeffDisplay( const char* filename, const char* treename="vsitree" ) {
  
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
   DVObj dv;
   DVeffObj dveff;
   
   // Set branch addresses.
   setBranchAddress( tree, ei );
   setBranchAddress( tree, pv );
   setBranchAddress( tree, dv );
   setBranchAddress( tree, dveff );
   
   
   
   //////////////////////////////////////////////////
   // Setup Histograms and efficiencies
  
   // binning
   std::vector<double> rbins = { 0.1, 0.3, 0.5, 0.7, 1.0, 1.5, 2, 2.5, 3, 5, 7, 10, 14, 20, 28, 38, 50, 64, 80, 100, 130, 170, 220, 280, 350, 450, 600 };
   std::vector<double> nbins = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20, 24, 28, 38, 50, 70, 100, 150, 220 };
   
   std::vector<double> mbins;
   for( int i=0; i<101; i++) { mbins.emplace_back( pow(10, -1.0 + i*0.05) ); }
  
   
   gr_llp->SetMarkerStyle(24);
   gr_llp->SetMarkerSize(1.3);
   gr_llp->SetMarkerColor(kBlue);
   
   gr_desc->SetMarkerStyle(20);
   gr_desc->SetMarkerSize(0.5);
   gr_desc->SetMarkerColor(kViolet);
   
   gr_recovtx->SetMarkerStyle(20);
   gr_recovtx->SetMarkerSize(0.5);
   gr_recovtx->SetMarkerColor(kBlack);
   
   
   auto getGuide = []( const double r ) {
     auto* gr = new TGraph;
     for( int i=0; i<1000; i++ ) {
       auto phi = TMath::TwoPi()/1000.*i;
       gr->SetPoint( gr->GetN(), r*cos(phi), r*sin(phi) );
       gr->SetLineColorAlpha(kGray, 0.5);
     }
     return gr;
   };
  
   std::vector<TGraph*> guides;
   guides.emplace_back( getGuide(33.0) );
   guides.emplace_back( getGuide(50.5) );
   guides.emplace_back( getGuide(85.5) );
   guides.emplace_back( getGuide(122.5) );
  
   guides.emplace_back( getGuide(299.0) );
   guides.emplace_back( getGuide(371.0) );
   guides.emplace_back( getGuide(443.0) );
   guides.emplace_back( getGuide(516.0) );
  
  
   //////////////////////////////////////////////////
   // Main loop
   
   Stats stats;
   
   Long64_t nentries = 100;
   //Long64_t nentries = tree->GetEntries();
   
   
   for (Long64_t i=0; i<nentries;i++) {
     tree->GetEntry(i);
     
     gr_llp->Set(0);
     gr_desc->Set(0);
     gr_recovtx->Set(0);
     
     desc_lines_unreco.clear();
     desc_lines_reco.clear();
     
     auto* canvas = new TCanvas( "canvas", "", 0, 0, 1600, 800);
     canvas->Divide(2, 1);
     
     auto* frame1 = canvas->cd(1)->DrawFrame(-300, -300, 300, 300);
     frame1->SetTitle(";x [mm];y [mm]");
    
     auto* frame2 = canvas->cd(2)->DrawFrame(-300, -300, 300, 300);
     frame2->SetTitle(";x [mm];y [mm]");
     
     
     canvas->cd(1);
     for( auto* g : guides ) { g->Draw("l+"); }
     
     for( size_t i_llp = 0; i_llp < dveff.LLP_size; ++i_llp ) {
       
       process( dveff, pv, dv, i_llp, stats );
       
     }
     
     for( auto* l : desc_lines_unreco ) l->Draw("same");
     for( auto* l : desc_lines_reco   ) l->Draw("same");
     
     gr_desc   ->Draw("p+");
     gr_llp    ->Draw("p+");
     gr_recovtx->Draw("p+");
     
     canvas->cd(2);
     for( auto* g : guides ) { g->Draw("l+"); }
     
     for( size_t idv=0; idv < dv.r->size(); idv++) {
       for( size_t itrk=0; itrk < dv.trk_pt->at(idv).size(); itrk++) {
         auto& r = dv.r->at(idv);
         auto& phi = dv.phi->at(idv);
         auto& pt = dv.trk_pt_wrtSV->at(idv).at(itrk);
         auto& pPhi = dv.trk_phi_wrtSV->at(idv).at(itrk);
         auto* l = new TLine( r*cos(phi), r*sin(phi), r*cos(phi) + pt*cos(pPhi)*1.e-2, r*sin(phi) + pt*sin(pPhi)*1.e-2 );
         l->SetLineColor(kBlue);
         dv_trks.emplace_back( l );
         l->Draw();
       }
     }
     
     
     canvas->SaveAs(Form("display/event%08lld.png", i));
     
     delete canvas;
     
     for( auto* l : desc_lines_unreco ) { delete l; }
     for( auto* l : desc_lines_reco )   { delete l; }
     desc_lines_unreco.clear();
     desc_lines_reco.clear();
     
     for( auto* l : dv_trks )   { delete l; }
     dv_trks.clear();
     
     stats.nEvents++;
   }
   
   
   //////////////////////////////////////////////////
   // Post processing
   
   std::cout << "#########################################################" << std::endl;
   std::cout << Form("Total events : %lu", stats.nEvents ) << std::endl;
   std::cout << Form("Total LLPs : %lu", stats.nLLPs ) << std::endl;
   std::cout << Form("Total LLPs with truth vertex position(s) : %lu", stats.nLLPs_hasTruthPos ) << std::endl;
   std::cout << Form("Total LLPs with reco vertices : %lu", stats.nLLPs_hasReco ) << std::endl;
   std::cout << Form("Total LLPs with >1 reco vertices : %lu", stats.nLLPs_hasMoreThanOneRecos ) << std::endl;
   std::cout << Form("Total LLPs with split : %lu", stats.nLLPs_split ) << std::endl;
   
}



void process( DVeffObj& dveff, PVObj& pv, DVObj& dv, const size_t i_llp, Stats& stats ) {
  
  DVeff::DVeffVtx v;
  
  TVector3 pvPos( pv.x, pv.y, pv.z );
  setup( v, dveff, i_llp );
  
  gr_llp->SetPoint(gr_llp->GetN(), v.llp.decay.x(), v.llp.decay.y() );
  
  TLorentzVector sump4;
  
  for( auto& desc : v.descendents ) {
    gr_desc->SetPoint( gr_desc->GetN(), desc.prod.x(), desc.prod.y() );
    
    auto* l = new TLine( desc.prod.x(), desc.prod.y(),
                         desc.prod.x() + desc.p4.Pt()*cos(desc.p4.Phi())*1.e-2,
                         desc.prod.y() + desc.p4.Pt()*sin(desc.p4.Phi())*1.e-2 );
    
    sump4 += desc.p4;
    
    if( desc.isReconstructed ) {
      l->SetLineColor(kRed);
      desc_lines_reco.emplace_back(l);
    } else {
      l->SetLineColor(kGray+1);
      desc_lines_unreco.emplace_back(l);
    }
  }
  
  if( v.llp.decay.Perp() < 300. ) {
    std::cout << Form("LLP p4 in (pt, eta, phi) = (%.3f, %.3f, %.3f) | out = (%.3f, %.3f, %.3f)",
                      v.llp.p4.Pt()*1.e-3, v.llp.p4.Eta(), v.llp.p4.Phi(), sump4.Pt()*1.e-3, sump4.Eta(), sump4.Phi() )
              << std::endl;
    
    v.print( i_llp );
  }
  
  for( auto& rv : v.recoVertices ) {
    gr_recovtx->SetPoint(gr_recovtx->GetN(), rv.pos.x(), rv.pos.y() );
  }
  
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

