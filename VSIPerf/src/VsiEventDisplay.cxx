#include "include/VsiEventDisplay.h"
#include "include/VsiBonsaiTool.h"

#include <TROOT.h>
#include <TSystem.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TSystem.h>
#include <TStyle.h>
#include <TInterpreter.h>
#include <TEllipse.h>
#include <TLine.h>
#include <TLatex.h>
#include <TMath.h>

#include <numeric>
#include <algorithm>
#include <iostream>
#include <map>



//____________________________________________________________________________________________________

class VsiEventDisplay::Impl {

public:
  
  static unsigned eventCount;
  
  std::vector<TObject*> objs;
  
  std::unique_ptr<TCanvas> canvas;
  
  std::vector< std::unique_ptr<TGraph> > guides;
  std::unique_ptr<TH1> frame1;
  std::unique_ptr<TH1> frame2;
  
  VsiEventDisplay::Client mode;
  
  std::string truthName;
  
  bool doPrint;
  bool doBatch;
  unsigned maxEvents;
  
  double scope { 130 };
  
  TLatex latex;
  Impl();
  ~Impl();
  
  void initialize();
  void clearFrame();
  void plotVsiAnaExample();

};

unsigned VsiEventDisplay::Impl::eventCount = 0;

//____________________________________________________________________________________________________
VsiEventDisplay::Impl::Impl()
  : canvas( nullptr )
  , mode( VsiEventDisplay::Client::kVsiAnaExample )
  , truthName( "RHadron" )
  , doPrint( true )
  , doBatch( true )
  , maxEvents( 1000 )
 {
  

  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  gStyle->SetPadTopMargin(0.05);
  gStyle->SetTitleOffset(1.2, "xy");
  gStyle->SetOptStat(0);
  
  // Barrel layers
  
  // Pixel
  auto getGuide = []( const double r ) {
    auto* gr = new TGraph;
    for( int i=0; i<1000; i++ ) {
      auto phi = TMath::TwoPi()/1000.*i;
      gr->SetPoint( gr->GetN(), r*cos(phi), r*sin(phi) );
    }
    return gr;
  };
  
  guides.emplace_back( getGuide(33.0) );
  guides.emplace_back( getGuide(50.5) );
  guides.emplace_back( getGuide(85.5) );
  guides.emplace_back( getGuide(122.5) );
  
  guides.emplace_back( getGuide(299.0) );
  guides.emplace_back( getGuide(371.0) );
  guides.emplace_back( getGuide(443.0) );
  guides.emplace_back( getGuide(516.0) );
  
  for( auto& guide : guides ) {
    guide->SetLineColor(kGray);
    guide->SetLineStyle(1);
  }
  
  latex.SetTextAlign(12);
  latex.SetTextFont(42);
      
}

//____________________________________________________________________________________________________
VsiEventDisplay::Impl::~Impl(){}


//____________________________________________________________________________________________________
void VsiEventDisplay::Impl::initialize() {
  canvas.reset( new TCanvas("c", "", 0, 0, 1200, 800) );
  canvas->SetTopMargin( 0.07 );
  canvas->SetRightMargin( 0.35 );
}

//____________________________________________________________________________________________________
void VsiEventDisplay::Impl::clearFrame() {
  
  if( !frame1 ) {
    frame1.reset( new TH1F( "frame", ";x [mm];y [mm]", 1000, -scope, scope ) );
    frame1->SetMinimum( -scope );
    frame1->SetMaximum(  scope );
    frame1->SetLineStyle(0);
    frame1->SetLineWidth(0);
    frame1->SetMarkerStyle(0);
    frame1->GetXaxis()->SetTitleOffset(1.5);
    frame1->GetYaxis()->SetTitleOffset(1.8);
    frame1->SetStats(0);
  }
  
  canvas->Clear();
  frame1->Draw();
  
  for( auto& guide : guides ) { guide->Draw("l+"); }
}


//____________________________________________________________________________________________________
void VsiEventDisplay::Impl::plotVsiAnaExample() {
  
  if( eventCount > maxEvents ) return;
  
  const auto& EventInfo = TreeManager::collection("EventInfo");
  const auto& DV        = TreeManager::collection("DV");
  const auto& Truth     = TreeManager::collection( (truthName + "Truth").c_str() );
  
#define GETREF( collection, name, type ) const auto& ref_##collection ##_ ##name = collection.at(#name).getVar<type>();
#define DEBUG() std::cout << __FUNCTION__ << " " << __LINE__ << std::endl;
  
  std::vector<std::vector<std::unique_ptr<TGraph> > > gr_trks(2);
  std::vector<std::vector<std::unique_ptr<TGraph> > > gr_trks_full(2);
  
  
  GETREF( EventInfo, eventNumber,       unsigned long long );
  GETREF( DV, r,             std::vector<float> );
  GETREF( DV, z,             std::vector<float> );
  GETREF( DV, phi,           std::vector<float> );
  GETREF( DV, mass,          std::vector<float> );
  GETREF( DV, chi2,          std::vector<float> );
  GETREF( DV, pt,            std::vector<float> );
  GETREF( DV, direction,     std::vector<float> );
  GETREF( DV, trk_d0,        std::vector< std::vector<float> > );
  GETREF( DV, trk_z0,        std::vector< std::vector<float> > );
  GETREF( DV, trk_pt,        std::vector< std::vector<float> > );
  GETREF( DV, trk_eta,       std::vector< std::vector<float> > );
  GETREF( DV, trk_phi,       std::vector< std::vector<float> > );
  GETREF( DV, trk_charge,    std::vector< std::vector<int> > );
  GETREF( DV, trk_pt_wrtSV,  std::vector< std::vector<float> > );
  GETREF( DV, trk_eta_wrtSV, std::vector< std::vector<float> > );
  GETREF( DV, trk_phi_wrtSV, std::vector< std::vector<float> > );
  GETREF( DV, trk_hitPattern, std::vector< std::vector<uint32_t> > );
  GETREF( Truth, vtx_r,      std::vector<float> );
  GETREF( Truth, vtx_z,      std::vector<float> );
  GETREF( Truth, vtx_phi,    std::vector<float> );
  
  
  if( doBatch ) gROOT->SetBatch();
  
  if( 0 == ref_DV_r.size() ) return;
  
  //////////////////////////////////////
  
  clearFrame();
  
  std::cout << "plotting..." << std::endl;
  std::vector< std::unique_ptr<TGraph> > grs_vtx;
  std::vector< std::unique_ptr<TGraph> > grs_hits;
  std::vector< std::unique_ptr<TGraph> > grs_truth_vtx;
  grs_vtx.emplace_back( new TGraph );
  grs_hits.emplace_back( new TGraph );
  grs_truth_vtx.emplace_back( new TGraph );
  
  for( auto& gr : grs_vtx ) {
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(0.7);
  }
  
  for( auto& gr : grs_truth_vtx ) {
    gr->SetMarkerColor(kAzure);
    gr->SetMarkerStyle(24);
    gr->SetMarkerSize(1.5);
  }
  
  for( auto& gr : grs_hits ) {
    gr->SetMarkerStyle(21);
    gr->SetMarkerSize(0.3);
    gr->SetMarkerColor(kAzure);
  }
  
  latex.SetTextSize(0.03);
  latex.DrawLatexNDC(0.18, 0.88, Form("Event number %llu", ref_EventInfo_eventNumber ) );
  
  latex.SetTextSize(0.015);
  
  size_t panel { 0 };
  
  for( size_t iv = 0; iv < ref_DV_r.size(); iv++ ) {
    
    std::cout << __FUNCTION__ << "vtx index " << iv << std::endl;
        
    const auto& r   = ref_DV_r  .at(iv);
    const auto& phi = ref_DV_phi.at(iv);
        
    auto& trk_pt_wrtSV  = ref_DV_trk_pt_wrtSV .at(iv);
    auto& trk_eta_wrtSV = ref_DV_trk_eta_wrtSV.at(iv);
    auto& trk_phi_wrtSV = ref_DV_trk_phi_wrtSV.at(iv);
        
    for( size_t itrk=0; itrk < trk_pt_wrtSV.size(); itrk++) {
      TVector3 p; p.SetPtEtaPhi( trk_pt_wrtSV.at(itrk)*1.e-3, trk_eta_wrtSV.at(itrk), trk_phi_wrtSV.at(itrk) );
      
      auto* line = new TGraph;
      line->SetPoint(0, r*cos(phi), r*sin(phi) );
      line->SetPoint(1, r*cos(phi) + p.Perp()*cos(p.Phi())*20, r*sin(phi) + p.Perp()*sin(p.Phi())*20 );
      gr_trks.at(panel).emplace_back( line );
      
      auto& d0  = ref_DV_trk_d0.at(iv).at(itrk);
      auto& z0  = ref_DV_trk_z0.at(iv).at(itrk);
      auto& pt  = ref_DV_trk_pt.at(iv).at(itrk);
      //auto& eta = ref_DV_trk_eta.at(iv).at(itrk);
      auto& phi = ref_DV_trk_phi.at(iv).at(itrk);
      auto& q   = ref_DV_trk_charge.at(iv).at(itrk);
      auto& hits= ref_DV_trk_hitPattern.at(iv).at(itrk);
      
      const auto Rh = pt/0.6; // in [mm]
      auto dphi = -q*1.0/Rh; // [rad/mm]
      
      TVector3 perigee( -d0*sin(phi), d0*cos(phi), z0 );
      auto* gr = new TGraph();
      gr->SetPoint( gr->GetN(), perigee.x(), perigee.y() );
      
      for( int ip = 0; ip < 2000; ip++) {
        const auto& prevX = gr->GetX()[gr->GetN()-1];
        const auto& prevY = gr->GetY()[gr->GetN()-1];
        
        if( fabs( prevX ) > scope || fabs( prevY ) > scope ) break;
        
        const auto& newX = prevX + cos(phi+ip*dphi);
        const auto& newY = prevY + sin(phi+ip*dphi);
        
        gr->SetPoint( gr->GetN(), newX, newY );
        
        // hitPattern
        const auto& trkR = hypot(newX, newY);
        
        auto within = []( double r, double rmin, double rmax ) -> bool {
          return (r > rmin && r < rmax);
        };
        
#define REGHIT( layer, radius ) if( VsiTool::hasHit( hits, VsiTool::DetectorType::layer ) && within( trkR, radius, radius+1.0 ) ) \
          { grs_hits.at(panel)->SetPoint( grs_hits.at(panel)->GetN(), newX, newY ); }
        
          REGHIT( pixelBarrel0, 33.0 );
          REGHIT( pixelBarrel1, 50.5 );
          REGHIT( pixelBarrel2, 85.5 );
          REGHIT( pixelBarrel3, 122.5 );
          REGHIT( sctBarrel0, 299.0 );
          REGHIT( sctBarrel1, 371.0 );
          REGHIT( sctBarrel2, 443.0 );
          
#undef REGHIT
            
      }
      gr_trks_full.at(panel).emplace_back( gr );
    }
    
    if( 0 == trk_pt_wrtSV.size() ) {
      std::cout << "WARNING:: associated track is not found" << std::endl;
    }
    
    grs_vtx.at(panel)->SetPoint( grs_vtx.at(panel)->GetN(), r*cos(phi), r*sin(phi) );
    
  }
  
  for( auto& gr_trk : gr_trks_full.at(panel) ) {
    gr_trk->SetLineColor(kGray+1);
    gr_trk->SetLineStyle(3);
    gr_trk->Draw("l+");
  }
  
  for( auto& gr_trk : gr_trks.at(panel) ) {
    gr_trk->SetLineColor(kRed);
    gr_trk->Draw("l+");
  }
  
  if( grs_vtx.at(panel)->GetN() > 0 ) grs_vtx.at(panel)->Draw("p+");
  
  grs_hits.at(panel)->Draw("p+");
  
  
  for( size_t iv = 0; iv < ref_Truth_vtx_r.size(); iv++ ) {
    grs_truth_vtx.at(panel)->SetPoint( grs_truth_vtx.at(panel)->GetN(),
                                       ref_Truth_vtx_r.at(iv)*cos(ref_Truth_vtx_phi.at(iv)),
                                       ref_Truth_vtx_r.at(iv)*sin(ref_Truth_vtx_phi.at(iv)) );
  }
  grs_truth_vtx.at(panel)->Draw("p+");
  
  
  for( size_t iv = 0; iv < ref_DV_r.size(); iv++ ) {
    const auto& r   = ref_DV_r  .at(iv);
    const auto& phi = ref_DV_phi.at(iv);
        
    latex.DrawLatex( r*cos(phi)+10*scope/400, r*sin(phi), Form("[%lu]", iv) );
    
    double xtarget = 0.67+0.10*(iv/6);
    double ytarget = 0.95 - iv *0.13;
    
    latex.DrawLatexNDC( xtarget, ytarget,      Form("[%lu]: (r,z): (%.1f, %.1f), N_{trk}: %zu", iv, ref_DV_r.at(iv), ref_DV_z.at(iv), ref_DV_trk_pt_wrtSV.at(iv).size() ) );
    latex.DrawLatexNDC( xtarget, ytarget-0.03, Form("    m: %.2f GeV, #chi^{2}: %.2e", ref_DV_mass.at(iv)*1.e-3, ref_DV_chi2.at(iv)) );
    latex.DrawLatexNDC( xtarget, ytarget-0.06, Form("    p_{T}: %.2f GeV, dir: %.0f#circ", ref_DV_chi2.at(iv)*1.e-3, acos(ref_DV_direction.at(iv))*TMath::RadToDeg() ) );
  }
  
  panel++;
  
  gPad->Modified();
  gPad->Update();
  
  gSystem->ProcessEvents();
  
  if( doBatch || doPrint ) {
    gSystem->mkdir( "displays", true );
    gPad->SaveAs(Form("displays/event%08llu.pdf", ref_EventInfo_eventNumber));
  }
  
  eventCount++;
  
  if( !doBatch ) {
    std::cout << "continue? > ";
    int a;
    std::cin >> a;
    if( 0 == a ) { exit(1); }
  }
}



//____________________________________________________________________________________________________
VsiEventDisplay::VsiEventDisplay( const std::string name )
  : ProcessorBase( name )
  , m_impl( new VsiEventDisplay::Impl )
{}

//____________________________________________________________________________________________________
VsiEventDisplay::~VsiEventDisplay(){}


//____________________________________________________________________________________________________
void VsiEventDisplay::registerVariables(EL::Worker* /*wk*/) {}

//____________________________________________________________________________________________________
void VsiEventDisplay::initialize() {
  m_impl->initialize();
}

//____________________________________________________________________________________________________
EL::StatusCode VsiEventDisplay::processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) {
  
  if( m_impl->mode == Client::kVsiAnaExample ) {
    m_impl->plotVsiAnaExample();
  }
      
  return EL::StatusCode::SUCCESS;
}


//____________________________________________________________________________________________________
void VsiEventDisplay::setDoPrint( bool flag ) const {
  m_impl->doPrint = flag;
}

//____________________________________________________________________________________________________
void VsiEventDisplay::setDoBatch( bool flag ) const {
  m_impl->doBatch = flag;
}

//____________________________________________________________________________________________________
void VsiEventDisplay::setMaxEvents( const unsigned nmax ) const {
  m_impl->maxEvents = nmax;
}

//____________________________________________________________________________________________________
void VsiEventDisplay::setTruthName( const std::string name ) const {
  m_impl->truthName = name;
}

//____________________________________________________________________________________________________
void VsiEventDisplay::setScope( const double scope ) const {
  m_impl->scope = scope;
}
