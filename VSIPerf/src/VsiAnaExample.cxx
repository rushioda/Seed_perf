#include <include/VsiAnaExample.h>
#include <include/VsiTruthHelper.h>
#include <include/VsiEventDisplay.h>

#include <include/AlgConsts.h>

#include <AsgTools/MessageCheck.h>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"

#include "TFile.h"
#include "TTree.h"
#include "TVector3.h"
#include "TRandom2.h"

#include <iostream>

// this is needed to distribute the algorithm to the workers
ClassImp(VsiAnaExample)

#define debug 0

#define DEBUG_MSG() std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;

// local convention: use "r_" to express the recording variables

struct VsiAnaExample_impl {
  // class status variables
  bool isMC;
  unsigned long eventCount;
  
  std::map<std::string, TH1*> histos;
  
  std::unique_ptr<TreeManager>  treeManager;
  std::map<std::string, anytype> vars;
  std::vector<const xAOD::TruthVertex*> truthVertices;
  VsiBonsai::Configurator bonsaiCfg;
  
  std::unique_ptr<VsiEventDisplay> display;
  
  void registerVariables();
  
  VsiAnaExample_impl();
  ~VsiAnaExample_impl(){};
};

VsiAnaExample_impl::VsiAnaExample_impl()
  : isMC( false )
  , eventCount( 0 )
  , treeManager ( new TreeManager() )
  , vars( TreeManager::collection("VsiAnaExample") )
  , display( new VsiEventDisplay( "Display" ) )
{}

//________________________________________________________________________________________________________________________
void VsiAnaExample_impl::registerVariables() {
  
  vars["runNumber"]              = uint32_t { 0 };
  vars["eventNumber"]            = 0ULL;
  vars["lumiBlock"]              = uint32_t { 0 };
  vars["averageMu"]              = 0.0F;
  vars["actualMu"]               = 0.0F;
  vars["nTracks"]                = uint32_t { 0 };
  vars["nTracksPVassoc"]         = uint32_t { 0 };
  vars["nSelectedTracks"]        = uint32_t { 0 };
  
  vars["pv_x"]                   = 0.0F;
  vars["pv_y"]                   = 0.0F;
  vars["pv_z"]                   = 0.0F;
  vars["pv_r"]                   = 0.0F;
  vars["pv_phi"]                 = 0.0F;
  
  vars["trk_d0"]                 = std::vector<float> {};
  vars["trk_z0"]                 = std::vector<float> {};
  vars["trk_pt"]                 = std::vector<float> {};
  vars["trk_eta"]                = std::vector<float> {};
  vars["trk_phi"]                = std::vector<float> {};
  vars["trk_errd0"]              = std::vector<float> {};
  vars["trk_errz0"]              = std::vector<float> {};
  vars["trk_chi2"]               = std::vector<float> {};
  vars["trk_selected"]           = std::vector<uint32_t> {};
  
  vars["vtx_x"]                  = std::vector<float> {};
  vars["vtx_y"]                  = std::vector<float> {};
  vars["vtx_z"]                  = std::vector<float> {};
  vars["vtx_r"]                  = std::vector<float> {};
  vars["vtx_phi"]                = std::vector<float> {};
  vars["vtx_covariance"]         = std::vector< std::vector<float> > {};
  vars["vtx_chi2"]               = std::vector<float> {};
  vars["vtx_mass"]               = std::vector<float> {};
  vars["vtx_mass_nonAssociated"] = std::vector<float> {};
  vars["vtx_direction"]          = std::vector<float> {};
  vars["vtx_pt"]                 = std::vector<float> {};
  vars["vtx_H"]                  = std::vector<float> {};
  vars["vtx_HT"]                 = std::vector<float> {};
  vars["vtx_minOpAng"]           = std::vector<float> {};
  vars["vtx_maxOpAng"]           = std::vector<float> {};
  vars["vtx_mind0"]              = std::vector<float> {};
  vars["vtx_maxd0"]              = std::vector<float> {};
  vars["vtx_ntrk"]               = std::vector<uint32_t> {};
  vars["vtx_ntrk_selected"]      = std::vector<uint32_t> {};
  vars["vtx_ntrk_associated"]    = std::vector<uint32_t> {};
  
  vars["vtx_trk_d0"]                 = std::vector<std::vector<float>> {};
  vars["vtx_trk_z0"]                 = std::vector<std::vector<float>> {};
  vars["vtx_trk_qOverP"]             = std::vector<std::vector<float>> {};
  vars["vtx_trk_pt"]                 = std::vector<std::vector<float>> {};
  vars["vtx_trk_eta"]                = std::vector<std::vector<float>> {};
  vars["vtx_trk_phi"]                = std::vector<std::vector<float>> {};
  
  vars["vtx_trk_d0_wrtSV"]           = std::vector<std::vector<float>> {};
  vars["vtx_trk_z0_wrtSV"]           = std::vector<std::vector<float>> {};
  vars["vtx_trk_pt_wrtSV"]           = std::vector<std::vector<float>> {};
  vars["vtx_trk_eta_wrtSV"]          = std::vector<std::vector<float>> {};
  vars["vtx_trk_phi_wrtSV"]          = std::vector<std::vector<float>> {};
  vars["vtx_trk_errd0_wrtSV"]        = std::vector<std::vector<float>> {};
  vars["vtx_trk_errz0_wrtSV"]        = std::vector<std::vector<float>> {};
  vars["vtx_trk_chi2_toSV"]          = std::vector<std::vector<float>> {};
  
  vars["vtx_trk_final"]              = std::vector<std::vector<uint32_t>> {};
  vars["vtx_trk_charge"]             = std::vector<std::vector<int>> {};
  vars["vtx_trk_isSelected"]         = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_isAssociated"]       = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nPixelHits"]         = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nSCTHits"]           = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nTRTHits"]           = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nPixelBarrelLayers"] = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nPixelEndCapLayers"] = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nSctBarrelLayers"]   = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nSctEndCapLayers"]   = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_hitPattern"]         = std::vector<std::vector<uint32_t>> {};
  vars["vtx_trk_dEdx"]               = std::vector<std::vector<float>> {};
  vars["vtx_trk_nUsedHitsdEdx"]      = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_nIBLOverflowsdEdx"]  = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_truthPid"]           = std::vector<std::vector<int>> {};
  vars["vtx_trk_truthIsPointingToClosestTruthVtx"] = std::vector<std::vector<uint8_t>> {};
  vars["vtx_trk_truthIsPointingToReprTruthVtx"]    = std::vector<std::vector<uint8_t>> {};
  
  vars["vtx_njet"]                   = std::vector<size_t> {};
  vars["vtx_jet_pt"]                 = std::vector<std::vector<float>> {};
  vars["vtx_jet_eta"]                = std::vector<std::vector<float>> {};
  vars["vtx_jet_phi"]                = std::vector<std::vector<float>> {};
  vars["vtx_jet_mass"]               = std::vector<std::vector<float>> {};
  vars["vtx_jet_ntrk"]               = std::vector<std::vector<size_t>> {};
  
  vars["vtx_closestTruth_x"]            = std::vector<float> {};
  vars["vtx_closestTruth_y"]            = std::vector<float> {};
  vars["vtx_closestTruth_z"]            = std::vector<float> {};
  vars["vtx_closestTruth_r"]            = std::vector<float> {};
  vars["vtx_closestTruth_phi"]          = std::vector<float> {};
  vars["vtx_closestTruth_inE"]          = std::vector<float> {};
  vars["vtx_closestTruth_outE"]         = std::vector<float> {};
  vars["vtx_closestTruth_distance"]     = std::vector<float> {};
  vars["vtx_closestTruth_parent"]       = std::vector<int> {};
  vars["vtx_closestTruth_parent_pt"]    = std::vector<float> {};
  vars["vtx_closestTruth_parent_eta"]   = std::vector<float> {};
  vars["vtx_closestTruth_parent_phi"]   = std::vector<float> {};
  vars["vtx_closestTruth_outP_pt"]      = std::vector< std::vector<float> > {};
  vars["vtx_closestTruth_outP_eta"]     = std::vector< std::vector<float> > {};
  vars["vtx_closestTruth_outP_phi"]     = std::vector< std::vector<float> > {};
  vars["vtx_closestTruth_outP_charge"]  = std::vector< std::vector<int> > {};
  vars["vtx_closestTruth_outP_pid"]     = std::vector< std::vector<int> > {};
  vars["vtx_closestTruth_outP_isReco"]  = std::vector< std::vector<uint8_t> > {};
  
  vars["vtx_reprTruth_x"]            = std::vector<float> {};
  vars["vtx_reprTruth_y"]            = std::vector<float> {};
  vars["vtx_reprTruth_z"]            = std::vector<float> {};
  vars["vtx_reprTruth_r"]            = std::vector<float> {};
  vars["vtx_reprTruth_phi"]          = std::vector<float> {};
  vars["vtx_reprTruth_inE"]          = std::vector<float> {};
  vars["vtx_reprTruth_outE"]         = std::vector<float> {};
  vars["vtx_reprTruth_matchScore"]   = std::vector<float> {};
  vars["vtx_reprTruth_parent"]       = std::vector<int> {};
  vars["vtx_reprTruth_parent_pt"]    = std::vector<float> {};
  vars["vtx_reprTruth_parent_eta"]   = std::vector<float> {};
  vars["vtx_reprTruth_parent_phi"]   = std::vector<float> {};
  vars["vtx_reprTruth_outP_pt"]      = std::vector< std::vector<float> > {};
  vars["vtx_reprTruth_outP_eta"]     = std::vector< std::vector<float> > {};
  vars["vtx_reprTruth_outP_phi"]     = std::vector< std::vector<float> > {};
  vars["vtx_reprTruth_outP_charge"]  = std::vector< std::vector<int> > {};
  vars["vtx_reprTruth_outP_pid"]     = std::vector< std::vector<int> > {};
  vars["vtx_reprTruth_outP_isReco"]  = std::vector< std::vector<uint8_t> > {};
  
  vars["truth_vtx_r"]               = std::vector<float> {};
  vars["truth_vtx_z"]               = std::vector<float> {};
  vars["truth_vtx_phi"]             = std::vector<float> {};
  vars["truth_vtx_parent_pt"]       = std::vector<float> {};
  vars["truth_vtx_parent_eta"]      = std::vector<float> {};
  vars["truth_vtx_parent_phi"]      = std::vector<float> {};
  vars["truth_vtx_parent_pid"]      = std::vector<int> {};
  vars["truth_vtx_parent_prod_x"]   = std::vector<float> {};
  vars["truth_vtx_parent_prod_y"]   = std::vector<float> {};
  vars["truth_vtx_parent_prod_z"]   = std::vector<float> {};
  vars["truth_vtx_parent_prod_r"]   = std::vector<float> {};
  vars["truth_vtx_parent_prod_phi"] = std::vector<float> {};
  vars["truth_vtx_outP_pt"]         = std::vector<std::vector<float>> {};
  vars["truth_vtx_outP_eta"]        = std::vector<std::vector<float>> {};
  vars["truth_vtx_outP_phi"]        = std::vector<std::vector<float>> {};
  vars["truth_vtx_outP_pid"]        = std::vector<std::vector<int>> {};
  vars["truth_vtx_outP_isReco"]     = std::vector<std::vector<uint8_t>> {};
  
  std::cout << "============================================================" << std::endl;
  std::cout << __PRETTY_FUNCTION__ << ": Tree variables" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;
  for( auto& pair : vars ) {

#if debug
    std::cout << pair.first << ": ";
    pair.second.print();
#else
    std::cout << pair.first << ", ";
#endif
    
    try {
      
      treeManager->book( pair.first.c_str(), pair.second );
      
    } catch( TreeManager::error& e ) {
      
      if( e == TreeManager::error::not_possible_to_branch ) {
        std::cerr << __PRETTY_FUNCTION__ << ": could not create a branch for variable " << pair.first << std::endl;
      } else if( e == TreeManager::error::unsupported_type ) {
      }
      
      throw e;
    }
    
  }
#if debug
#else
  std::cout << std::endl;
#endif
  std::cout << "============================================================" << std::endl;
  
}



// With the above macro, GETREF( varName, type ) will be expanded as:
// auto& r_varName = m_impl->vars["varName"].getVar<type>();
#ifdef REFVAR
#undef REFVAR
#define REFVAR( name, type ) auto& r_##name = m_impl->vars.at(#name).getVar<type>();
#endif


#ifndef AUXDYN
#define AUXDYN( obj, type, varname ) ( obj->isAvailable<type>(varname)? obj->auxdataConst<type>(varname) : AlgConsts::invalidFloat )
#endif

//________________________________________________________________________________________________________________________
VsiAnaExample :: VsiAnaExample()
: EL::Algorithm()
, m_impl( new VsiAnaExample_impl )
, prop_probeTruth         ( "Rhadron" )
, prop_containerName      ( "SecondaryVertices" )
, prop_outputName         ( "svTree_trimmed" )
, prop_release            ( VsiAnaExample::Release::r21 )
, prop_chi2Cut            ( 5.0 )
, prop_hitPatternReq      ( VsiTool::HitPatternCondition::NONE )
, prop_doDropAssociated   ( false )
, prop_fillTracks         ( false )
, prop_trackStorePrescale ( 1.e-3 )
, prop_d0_wrtSVCut        ( 0.8 /*AlgConsts::invalidFloat*/ )
, prop_z0_wrtSVCut        ( 1.2 /*AlgConsts::invalidFloat*/ )
, prop_errd0_wrtSVCut     ( AlgConsts::invalidFloat )
, prop_errz0_wrtSVCut     ( AlgConsts::invalidFloat )
, prop_d0signif_wrtSVCut  ( 5.0 /* AlgConsts::invalidFloat*/ )
, prop_z0signif_wrtSVCut  ( 5.0 /*AlgConsts::invalidFloat*/ )
, prop_chi2_toSVCut       ( 5.0 /*AlgConsts::invalidFloat*/ )
, prop_doDisplay          ( false )
, prop_displayDoBatch     ( false )
, prop_displayMaxEvents   ( 100 )
{
  
  VsiBonsai::setDefaultConfig( m_impl->bonsaiCfg );
  
  m_impl->bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ] = 5.0;
  m_impl->bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = VsiTool::HitPatternCondition::NONE;
  m_impl->bonsaiCfg[ VsiBonsai::Config::dropAssociated ] = false;
  
  std::cout  << __PRETTY_FUNCTION__ << ": Output name = " << prop_outputName << std::endl;
  std::cout  << __PRETTY_FUNCTION__ << ": Container name = " << prop_containerName << std::endl;
  
}


//________________________________________________________________________________________________________________________
EL::StatusCode VsiAnaExample :: initialize ()
{
  
  std::cout  << __PRETTY_FUNCTION__ << ": Output name = " << prop_outputName << std::endl;
  std::cout  << __PRETTY_FUNCTION__ << ": Container name = " << prop_containerName << std::endl;

  TH1::SetDefaultSumw2();
  
  m_impl->histos["trk_nSelected"] = new TH1F("trk_nSelected", "", 1000, 0, 5000);
  m_impl->histos["trk_d0"]        = new TH1F("trk_d0", "", 1000, -300, 300);
  m_impl->histos["trk_z0"]        = new TH1F("trk_z0", "", 1000, -1500, 1500);
  m_impl->histos["trk_pt"]        = new TH1F("trk_pt", "", 1000, 0, 100);
  m_impl->histos["trk_eta"]       = new TH1F("trk_eta", "", 200, -5.0, 5.0);
  m_impl->histos["trk_phi"]       = new TH1F("trk_phi", "", 100, -TMath::Pi(), TMath::Pi());
  m_impl->histos["trk_chi2"]      = new TH1F("trk_chi2", "", 1000, 0, 100);
  m_impl->histos["trk_all_d0"]    = new TH1F("trk_all_d0", "", 1000, -300, 300);
  m_impl->histos["trk_all_z0"]    = new TH1F("trk_all_z0", "", 1000, -1500, 1500);
  m_impl->histos["trk_all_pt"]    = new TH1F("trk_all_pt", "", 1000, 0, 100);
  m_impl->histos["trk_all_eta"]   = new TH1F("trk_all_eta", "", 200, -5.0, 5.0);
  m_impl->histos["trk_all_phi"]   = new TH1F("trk_all_phi", "", 100, -TMath::Pi(), TMath::Pi());
  m_impl->histos["trk_all_chi2"]  = new TH1F("trk_all_chi2", "", 1000, 0, 100);
  m_impl->histos["vtx_mass"]      = new TH1F("mass", "", 2000, 0, 20);
  m_impl->histos["vtx_ntrk"]      = new TH1F("ntrk", "", 200, 0, 200);
  m_impl->histos["vtx_r"]         = new TH1F("r", "", 300, 0, 300);
  m_impl->histos["vtx_z"]         = new TH1F("z", "", 1000, -1500, 1500);
  m_impl->histos["vtx_chi2"]      = new TH1F("chi2", "", 1000, 0, 100);
  
  for( auto& pair : m_impl->histos ) {
    wk()->addOutput( pair.second );
  }
  
  m_impl->treeManager->initialize( prop_outputName, wk()->getOutputFile ( "VsiAnaExample" ) );
  
  m_impl->registerVariables();
  
  m_impl->bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ]        = static_cast<double>( prop_chi2Cut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = static_cast<VsiTool::HitPatternCondition>( prop_hitPatternReq );
  m_impl->bonsaiCfg[ VsiBonsai::Config::dropAssociated ]      = static_cast<bool>( prop_doDropAssociated );
  m_impl->bonsaiCfg[ VsiBonsai::Config::d0_wrtSVCut ]         = static_cast<double>( prop_d0_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::z0_wrtSVCut ]         = static_cast<double>( prop_z0_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::errd0_wrtSVCut ]      = static_cast<double>( prop_errd0_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::errz0_wrtSVCut ]      = static_cast<double>( prop_errz0_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::d0signif_wrtSVCut ]   = static_cast<double>( prop_d0signif_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::z0signif_wrtSVCut ]   = static_cast<double>( prop_z0signif_wrtSVCut );
  m_impl->bonsaiCfg[ VsiBonsai::Config::chi2_toSVCut ]        = static_cast<double>( prop_chi2_toSVCut );
  
  
  // Display initialization
  if( prop_doDisplay ) {
    m_impl->display->setMaxEvents( prop_displayMaxEvents );
    m_impl->display->setDoBatch  ( prop_displayDoBatch );
    m_impl->display->initialize  ();
  }
  
  return EL::StatusCode::SUCCESS;
}



//________________________________________________________________________________________________________________________
EL::StatusCode VsiAnaExample :: execute ()
{
  
  try {
    
#if debug
    setMsgLevel( MSG::DEBUG );
    VsiBonsai::setOutputLevel( MSG::DEBUG );
#endif

    m_impl->eventCount++;
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Input setup
    //
  
    xAOD::TEvent *event = wk()->xaodEvent();
  
    // xAOD grips
    const xAOD::EventInfo              *eventInfo        ( nullptr );
    const xAOD::TruthParticleContainer *truthParticles   ( nullptr );
    const xAOD::TruthVertexContainer   *truthVertices    ( nullptr );
    const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
    const xAOD::TrackParticleContainer *selectedTracks   ( nullptr );
    const xAOD::VertexContainer        *primaryVertices  ( nullptr );
    const xAOD::VertexContainer        *vsiVertices      ( nullptr );
  
    // retrieving
    if( ! event->retrieve( eventInfo, "EventInfo").isSuccess() ){
      Error("execute()", "Failed to retrieve event info collection. Exiting." );
      return EL::StatusCode::FAILURE;
    }

    // check if the event is data or MC
    // (many tools are applied either to data or MC)
    // check if the event is MC
    if(eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) ){
      m_impl->isMC = true; // can do something with this later
    }
  
    ANA_CHECK( event->retrieve(recoTracks,       "InDetTrackParticles") );
    ANA_CHECK( event->retrieve(primaryVertices,  "PrimaryVertices") );
    ANA_CHECK( event->retrieve(vsiVertices,      "VrtSecInclusive_" + prop_containerName) );
  
    if( m_impl->isMC ) {
      ANA_CHECK( event->retrieve(truthVertices,    "TruthVertices") );
      ANA_CHECK( event->retrieve(truthParticles,   "TruthParticles") );
    }
    
    if( static_cast<Release>(prop_release) == Release::r20p7 ) {
      ANA_CHECK( event->retrieve(selectedTracks, "VrtSecInclusive_SelectedTrackParticles") );
    }
  
  
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Output setup
    //
  
    // Clear all variables
    for( auto& pair : m_impl->vars ) {
      pair.second.clear();
    }
  
  
    // local convention: use "r_" to express the recording variables
  
    // With the above macro, REFVAR( varName, type ) will be expanded as:
    // auto& r_varName = m_impl->vars["varName"].getVar<type>();
  
    REFVAR( runNumber,       uint32_t);
    REFVAR( eventNumber,     unsigned long long);
    REFVAR( lumiBlock,       uint32_t);
    REFVAR( nTracks,         uint32_t);
    REFVAR( nTracksPVassoc,  uint32_t);
    REFVAR( nSelectedTracks, uint32_t);
    REFVAR( averageMu,       float);
    REFVAR( actualMu,        float);
  
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // EventInfo part
    //
  
#if debug
    if( vsiVertices->size() > 0 ) {
      ATH_MSG_DEBUG( "\n==========================================================================" );
      ATH_MSG_DEBUG( "Event count  = " << m_impl->eventCount );
      ATH_MSG_DEBUG( "Event number = " << eventInfo->eventNumber() );
      ATH_MSG_DEBUG( "==========================================================================" );
    }
#endif
  
  
    r_runNumber   = eventInfo->runNumber();
    r_eventNumber = eventInfo->eventNumber();
    r_lumiBlock   = eventInfo->lumiBlock();
    r_averageMu   = eventInfo->averageInteractionsPerCrossing();
    r_actualMu    = eventInfo->actualInteractionsPerCrossing();
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth part
    //
  
    if( m_impl->isMC && truthVertices && truthParticles ) {
      do_truth( truthVertices );
    }
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Tracks part
    //
  
    unsigned nPVassoc { 0 };
    unsigned nSelected { 0 };
  
#if debug
    ATH_MSG_DEBUG( "------------------------------------" );
    ATH_MSG_DEBUG( Form("Loop over reco tracks: size = %lu", recoTracks->size())  );
#endif
  
  
    do_tracks( recoTracks, selectedTracks, primaryVertices, nPVassoc, nSelected );
    

#if debug
    if( vsiVertices->size() > 0 ) {
      ATH_MSG_DEBUG( Form("Tracks: nReco = %lu, nSelected = %u, nPVassoc = %u", recoTracks->size(), nSelected, nPVassoc) );
    }
  

    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // PV part
    //
  
    // loop over primary vertices
    ATH_MSG_VERBOSE( "------------------------------------" );
    ATH_MSG_VERBOSE( Form("Loop over primary vertices: size = %lu", primaryVertices->size()) );
    for( auto *vertex : *primaryVertices ) {
      ATH_MSG_VERBOSE( Form("  vertex: type = %d   |   (r, z, phi) = (%7.2f, %7.2f, %5.2f)   |   num_trks = %3lu   |   chi2/ndof = %5.1f",
                        vertex->vertexType(),
                        sqrt(vertex->x()*vertex->x() + vertex->y()*vertex->y()), vertex->z(), atan2(vertex->y(), vertex->x()), vertex->nTrackParticles(), vertex->chiSquared()/vertex->numberDoF() )
                );
    }
  
#endif
  
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // PV part
    //
    
    REFVAR( pv_x, float );
    REFVAR( pv_y, float );
    REFVAR( pv_z, float );
    REFVAR( pv_r, float );
    REFVAR( pv_phi, float );
  
    xAOD::Vertex *thePV( nullptr );
  
    for( auto *vertex : *primaryVertices ) {
      if( vertex->vertexType() == xAOD::VxType::PriVtx ) {
        thePV = vertex;
        nPVassoc = vertex->nTrackParticles();
        break;
      }
    }
    if( !thePV ) return StatusCode::SUCCESS;
    
    r_pv_x   = thePV->x();
    r_pv_y   = thePV->y();
    r_pv_z   = thePV->z();
    r_pv_r   = thePV->position().perp();
    r_pv_phi = thePV->position().phi();
  
    r_nTracks         = recoTracks->size();
    r_nTracksPVassoc  = nPVassoc;
    r_nSelectedTracks = nSelected;
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // vsiVertices part
    //
  
    if( vsiVertices ) {
      
      do_vsiVertices( vsiVertices, thePV );
      
    }
  
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth part
    //
  
    if( m_impl->isMC && truthVertices && truthParticles ) {
      do_truth( truthVertices );
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Event display (optional for detailed scrutiny)
    //
    
    if( prop_doDisplay ) {
      m_impl->display->process( event );
    }
  
  
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Fill tree
    //
  
    m_impl->treeManager->fill();
  


#if debug
    std::cout << "Enter y to continue: " << std::flush;
    char tmp;
    std::cin >> tmp;
    if( 0 == tmp ) return StatusCode::FAILURE;
#endif
    
  } catch( std::out_of_range& e ) {
    ATH_MSG_INFO( __PRETTY_FUNCTION__ << "Exception was thrown while retrieving tree variables: " << e.what() );
    ATH_MSG_INFO( "possible combinations of registered var names/types are:" );
    for( auto& pair : m_impl->vars ) {
      std::cout << pair.first << ": ";
      pair.second.print();
    }
    throw e;
  }
  
  return StatusCode::SUCCESS;
}


//________________________________________________________________________________________________________________________
EL::StatusCode VsiAnaExample :: finalize ()
{
  ATH_MSG_INFO( __PRETTY_FUNCTION__ );
  ATH_MSG_INFO( "processed events = " << m_impl->eventCount );
  return StatusCode::SUCCESS;
}



//________________________________________________________________________________________________________________________
void VsiAnaExample::do_vsiVertices( const xAOD::VertexContainer* vsiVertices, const xAOD::Vertex* thePV ) {
    
  xAOD::TEvent *event = wk()->xaodEvent();
  const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
  auto sc = event->retrieve(recoTracks,       "InDetTrackParticles");
  if( sc.isFailure() ) {
    return;
  }
  
  // local convention: use "r_" to express the recording variables

  // With the above macro, REFVAR( varName, type ) will be expanded as:
  // auto& r_varName = m_impl->vars["varName"].getVar<type>();
  REFVAR( vtx_x,                  std::vector<float> );
  REFVAR( vtx_y,                  std::vector<float> );
  REFVAR( vtx_z,                  std::vector<float> );
  REFVAR( vtx_r,                  std::vector<float> );
  REFVAR( vtx_phi,                std::vector<float> );
  REFVAR( vtx_covariance,         std::vector< std::vector<float> > );
  REFVAR( vtx_chi2,               std::vector<float> );
  REFVAR( vtx_mass,               std::vector<float> );
  REFVAR( vtx_mass_nonAssociated, std::vector<float> );
  REFVAR( vtx_direction,          std::vector<float> );
  REFVAR( vtx_pt,                 std::vector<float> );
  REFVAR( vtx_H,                  std::vector<float> );
  REFVAR( vtx_HT,                 std::vector<float> );
  REFVAR( vtx_minOpAng,           std::vector<float> );
  REFVAR( vtx_maxOpAng,           std::vector<float> );
  REFVAR( vtx_mind0,              std::vector<float> );
  REFVAR( vtx_maxd0,              std::vector<float> );
  REFVAR( vtx_ntrk,               std::vector<uint32_t> );
  //REFVAR( vtx_ntrk_selected,      std::vector<int> );
  //REFVAR( vtx_ntrk_associated,    std::vector<int> );
  
  REFVAR( vtx_trk_d0,                 std::vector< std::vector<float> > );
  REFVAR( vtx_trk_z0,                 std::vector< std::vector<float> > );
  REFVAR( vtx_trk_qOverP,             std::vector< std::vector<float> > );
  REFVAR( vtx_trk_pt,                 std::vector< std::vector<float> > );
  REFVAR( vtx_trk_eta,                std::vector< std::vector<float> > );
  REFVAR( vtx_trk_phi,                std::vector< std::vector<float> > );
  REFVAR( vtx_trk_d0_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( vtx_trk_z0_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( vtx_trk_pt_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( vtx_trk_eta_wrtSV,          std::vector< std::vector<float> > );
  REFVAR( vtx_trk_phi_wrtSV,          std::vector< std::vector<float> > );
  REFVAR( vtx_trk_errd0_wrtSV,        std::vector< std::vector<float> > );
  REFVAR( vtx_trk_errz0_wrtSV,        std::vector< std::vector<float> > );
  REFVAR( vtx_trk_chi2_toSV  ,        std::vector< std::vector<float> > );
  REFVAR( vtx_trk_final,              std::vector< std::vector<uint32_t> > );
  REFVAR( vtx_trk_charge,             std::vector< std::vector<int> > );
  REFVAR( vtx_trk_isSelected,         std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_isAssociated,       std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nPixelHits,         std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nSCTHits,           std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nTRTHits,           std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nPixelBarrelLayers, std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nPixelEndCapLayers, std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nSctBarrelLayers,   std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_nSctEndCapLayers,   std::vector< std::vector<uint8_t> > );
  REFVAR( vtx_trk_hitPattern,         std::vector< std::vector<uint32_t> > );
  REFVAR( vtx_trk_dEdx,               std::vector< std::vector<float> >   );
  REFVAR( vtx_trk_nUsedHitsdEdx,      std::vector< std::vector<uint8_t> >   );
  REFVAR( vtx_trk_nIBLOverflowsdEdx,  std::vector< std::vector<uint8_t> >   );
  REFVAR( vtx_trk_truthPid,           std::vector< std::vector<int> >   );
  REFVAR( vtx_trk_truthIsPointingToClosestTruthVtx, std::vector< std::vector<uint8_t> >     );
  REFVAR( vtx_trk_truthIsPointingToReprTruthVtx,    std::vector< std::vector<uint8_t> >     );
  
  REFVAR( vtx_njet,                   std::vector< size_t > );
  REFVAR( vtx_jet_pt,                 std::vector< std::vector<float> > );
  REFVAR( vtx_jet_eta,                std::vector< std::vector<float> > );
  REFVAR( vtx_jet_phi,                std::vector< std::vector<float> > );
  REFVAR( vtx_jet_mass,               std::vector< std::vector<float> > );
  REFVAR( vtx_jet_ntrk,               std::vector< std::vector<size_t> > );
  
  REFVAR( vtx_reprTruth_x,               std::vector<float> );
  REFVAR( vtx_reprTruth_y,               std::vector<float> );
  REFVAR( vtx_reprTruth_z,               std::vector<float> );
  REFVAR( vtx_reprTruth_r,               std::vector<float> );
  REFVAR( vtx_reprTruth_phi,             std::vector<float> );
  REFVAR( vtx_reprTruth_inE,             std::vector<float> );
  REFVAR( vtx_reprTruth_outE,            std::vector<float> );
  REFVAR( vtx_reprTruth_matchScore,      std::vector<float> );
  REFVAR( vtx_reprTruth_parent,          std::vector<int>   );
  REFVAR( vtx_reprTruth_parent_pt,       std::vector<float>   );
  REFVAR( vtx_reprTruth_parent_eta,      std::vector<float>   );
  REFVAR( vtx_reprTruth_parent_phi,      std::vector<float>   );
  REFVAR( vtx_reprTruth_outP_pt,         std::vector< std::vector<float> >   );
  REFVAR( vtx_reprTruth_outP_eta,        std::vector< std::vector<float> >   );
  REFVAR( vtx_reprTruth_outP_phi,        std::vector< std::vector<float> >   );
  REFVAR( vtx_reprTruth_outP_charge,     std::vector< std::vector<int> >     );
  REFVAR( vtx_reprTruth_outP_pid,        std::vector< std::vector<int> >     );
  REFVAR( vtx_reprTruth_outP_isReco,     std::vector< std::vector<uint8_t> > );

  REFVAR( vtx_closestTruth_x,               std::vector<float> );
  REFVAR( vtx_closestTruth_y,               std::vector<float> );
  REFVAR( vtx_closestTruth_z,               std::vector<float> );
  REFVAR( vtx_closestTruth_r,               std::vector<float> );
  REFVAR( vtx_closestTruth_phi,             std::vector<float> );
  REFVAR( vtx_closestTruth_inE,             std::vector<float> );
  REFVAR( vtx_closestTruth_outE,            std::vector<float> );
  REFVAR( vtx_closestTruth_distance,        std::vector<float> );
  REFVAR( vtx_closestTruth_parent,          std::vector<int>   );
  REFVAR( vtx_closestTruth_parent_pt,       std::vector<float>   );
  REFVAR( vtx_closestTruth_parent_eta,      std::vector<float>   );
  REFVAR( vtx_closestTruth_parent_phi,      std::vector<float>   );
  REFVAR( vtx_closestTruth_outP_pt,         std::vector< std::vector<float> >   );
  REFVAR( vtx_closestTruth_outP_eta,        std::vector< std::vector<float> >   );
  REFVAR( vtx_closestTruth_outP_phi,        std::vector< std::vector<float> >   );
  REFVAR( vtx_closestTruth_outP_charge,     std::vector< std::vector<int> >     );
  REFVAR( vtx_closestTruth_outP_pid,        std::vector< std::vector<int> >     );
  REFVAR( vtx_closestTruth_outP_isReco,     std::vector< std::vector<uint8_t> > );

#if debug
  // loop over secondary vertices
  if( vsiVertices->size() > 0 ) {
    ATH_MSG_DEBUG( "------------------------------------" );
    ATH_MSG_DEBUG( Form("Loop over secondary vertices: size = %lu", vsiVertices->size()) );
  }
  
  for( auto *vertex : *vsiVertices ) printVertex( vertex );
  ATH_MSG_DEBUG( "------------------------------------" );
#endif
  
  
  std::vector<VsiBonsai::Trimmer> trimmers {
    VsiBonsai::dropAssociated,
    VsiBonsai::chi2Filter,
    VsiBonsai::ipWrtSVFilter,
    VsiBonsai::hitPatternFilter
  };
  
  for( auto *vertex : *vsiVertices ) {
    
    // Filter tracks based on VsiBonsai configuration
    std::vector<const xAOD::TrackParticle*> filteredTracks;
    
    for( size_t itrk=0; itrk<vertex->nTrackParticles(); itrk++) {
      
      auto* trk = vertex->trackParticle( itrk );
      if( !trk ) continue;
      
      {
        bool flag = false;
        
        for( auto trimmer : trimmers ) {
            
          if( !trimmer( vertex, trk, thePV, m_impl->bonsaiCfg ) ) {
            flag = true;
            break;
          }
        }
        
        if( flag ) continue;
      }
      
      filteredTracks.emplace_back( trk );
    }
    
    if( filteredTracks.size() < 2 ) continue;
    
    
    m_impl->histos["vtx_ntrk"]->Fill( filteredTracks.size() );
    m_impl->histos["vtx_r"]->Fill( vertex->position().perp() );
    m_impl->histos["vtx_z"]->Fill( vertex->z() );
    m_impl->histos["vtx_chi2"]->Fill( vertex->chiSquared()/vertex->numberDoF() );
    
    
    const TLorentzVector& sumP4 = VsiBonsai::sumP4( filteredTracks );
    const double scalarSumP  =  VsiBonsai::scalarSumP( filteredTracks );
    const double scalarSumPt =  VsiBonsai::scalarSumPt( filteredTracks );
    
    const TVector3 pos( vertex->x(), vertex->y(), vertex->z() );
    
    auto direction = sumP4.Vect().Dot( pos ) / sumP4.Vect().Mag() / pos.Mag();
    auto vtx_pt = sumP4.Pt();
    
    auto mass = sumP4.M();
    
    float mass_nonAssociated { AlgConsts::invalidFloat };
    if( vertex->isAvailable<float>("mass_selectedTracks") ) {
      mass_nonAssociated = vertex->auxdataConst<float>("mass_selectedTracks");
    }
    
    
    r_vtx_x                  .emplace_back( vertex->x() );
    r_vtx_y                  .emplace_back( vertex->y() );
    r_vtx_z                  .emplace_back( vertex->z() );
    r_vtx_r                  .emplace_back( vertex->position().perp() );
    r_vtx_phi                .emplace_back( vertex->position().phi() );
    r_vtx_covariance         .emplace_back( vertex->covariance() );
    r_vtx_chi2               .emplace_back( vertex->chiSquared()/vertex->numberDoF() );
    r_vtx_ntrk               .emplace_back( filteredTracks.size() );
    r_vtx_mass               .emplace_back( mass );
    r_vtx_pt                 .emplace_back( vtx_pt );
    r_vtx_H                  .emplace_back( scalarSumP );
    r_vtx_HT                 .emplace_back( scalarSumPt );
    r_vtx_direction          .emplace_back( direction );
    r_vtx_mass_nonAssociated .emplace_back( mass_nonAssociated );
    r_vtx_minOpAng           .emplace_back( VsiBonsai::minOpAng( filteredTracks ) );
    r_vtx_maxOpAng           .emplace_back( VsiBonsai::maxOpAng( filteredTracks ) );
    r_vtx_mind0              .emplace_back( VsiBonsai::mind0   ( filteredTracks ) );
    r_vtx_maxd0              .emplace_back( VsiBonsai::maxd0   ( filteredTracks ) );
    
    
    // Tracks associated to the vertex
#define BACK_ELEM( var ) r_##var.resize( r_##var.size()+1 ); auto& var = r_##var.back();
    
    BACK_ELEM( vtx_trk_d0 );
    BACK_ELEM( vtx_trk_z0 );
    BACK_ELEM( vtx_trk_pt );
    BACK_ELEM( vtx_trk_eta );
    BACK_ELEM( vtx_trk_phi );
    BACK_ELEM( vtx_trk_qOverP );
    BACK_ELEM( vtx_trk_d0_wrtSV );
    BACK_ELEM( vtx_trk_z0_wrtSV );
    BACK_ELEM( vtx_trk_pt_wrtSV );
    BACK_ELEM( vtx_trk_eta_wrtSV );
    BACK_ELEM( vtx_trk_phi_wrtSV );
    BACK_ELEM( vtx_trk_errd0_wrtSV );
    BACK_ELEM( vtx_trk_errz0_wrtSV );
    BACK_ELEM( vtx_trk_chi2_toSV );
    BACK_ELEM( vtx_trk_final );
    BACK_ELEM( vtx_trk_charge );
    BACK_ELEM( vtx_trk_isSelected );
    BACK_ELEM( vtx_trk_isAssociated );
    BACK_ELEM( vtx_trk_nPixelHits );
    BACK_ELEM( vtx_trk_nSCTHits );
    BACK_ELEM( vtx_trk_nTRTHits );
    BACK_ELEM( vtx_trk_nPixelBarrelLayers );
    BACK_ELEM( vtx_trk_nPixelEndCapLayers );
    BACK_ELEM( vtx_trk_nSctBarrelLayers );
    BACK_ELEM( vtx_trk_nSctEndCapLayers );
    BACK_ELEM( vtx_trk_hitPattern );
    BACK_ELEM( vtx_trk_dEdx );
    BACK_ELEM( vtx_trk_nUsedHitsdEdx );
    BACK_ELEM( vtx_trk_nIBLOverflowsdEdx );
    BACK_ELEM( vtx_trk_truthPid );
    BACK_ELEM( vtx_trk_truthIsPointingToClosestTruthVtx );
    BACK_ELEM( vtx_trk_truthIsPointingToReprTruthVtx );

#undef BACK_ELEM
    
    
#if debug
  ATH_MSG_DEBUG( Form("  PostFiltered vertex: type = %d   |   (r, z, phi) = (%7.2f, %7.2f, %5.2f)   |   num_trks = %3lu   |   chi2/ndof = %5.1f  | mass = %.3e",
                    vertex->vertexType(),
                    vertex->position().perp(), vertex->z(), vertex->position().phi(),
                    vertex->nTrackParticles(),
                    vertex->chiSquared()/vertex->numberDoF(),
                    AUXDYN( vertex, float, "vtx_mass" )  )
            << ( vertex->isAvailable<char>("isFake")? Form(" | isFake = %u", vertex->auxdataConst<char>("isFake") ) : "" )
            );
#endif
  
    for( auto* trk : filteredTracks ) {
      
      if( !trk ) continue;
      
#if debug
      
      if( trk->isAvailable<float>("d0_wrtSV_Leptons") ) {
        
        ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                     "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV(leptons) = (%8.1e, %8.1e)  | *Fake/PU*",
                     trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                     trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                     trk->d0(), trk->z0(),
                     AUXDYN( trk, float, "d0_wrtSV_Leptons" ),
                     AUXDYN( trk, float, "z0_wrtSV_Leptons" ) )
             );
        
      } else if( trk->isAvailable<float>("d0_wrtSV") ) {
        
        ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                     "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV = (%8.1e, %8.1e)  | *Fake/PU*",
                     trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                     trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                     trk->d0(), trk->z0(),
                     AUXDYN( trk, float, "d0_wrtSV" ),
                     AUXDYN( trk, float, "z0_wrtSV" ) )
             );
        
      } else {
        ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                     "   (d0, z0) = (%8.1e, %8.1e)  | *Fake/PU*",
                     trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                     trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                     trk->d0(),
                     trk->z0() )
             );
        
      }
        
#endif
        
      uint8_t nPixelHits        { 0 };
      uint8_t nSCTHits          { 0 };
      uint8_t nTRTHits          { 0 };
      float   pixeldEdx         { 0.0 };
      uint8_t nUsedHitsdEdx     { trk->numberOfUsedHitsdEdx() };
      uint8_t nIBLOverflowsdEdx { trk->numberOfIBLOverflowsdEdx() };
      
      trk->summaryValue( nPixelHits,        xAOD::numberOfPixelHits );
      trk->summaryValue( nSCTHits,          xAOD::numberOfSCTHits );
      trk->summaryValue( nTRTHits,          xAOD::numberOfTRTHits );
      trk->summaryValue( pixeldEdx,         xAOD::pixeldEdx );
      
      uint8_t nPixelBarrelLayers = VsiTool::nPixelBarrelLayers( trk->hitPattern() );
      uint8_t nPixelEndCapLayers = VsiTool::nPixelEndCapLayers( trk->hitPattern() );
      uint8_t nSctBarrelLayers   = VsiTool::nSctBarrelLayers  ( trk->hitPattern() );
      uint8_t nSctEndCapLayers   = VsiTool::nSctEndCapLayers  ( trk->hitPattern() );
      
      vtx_trk_charge       .emplace_back( static_cast<int>( trk->charge() ) );
      vtx_trk_d0       .emplace_back( trk->d0() );
      vtx_trk_z0       .emplace_back( trk->z0() );
      vtx_trk_qOverP       .emplace_back( trk->qOverP()  );
      vtx_trk_pt           .emplace_back( trk->pt() );
      vtx_trk_eta          .emplace_back( trk->eta() );
      vtx_trk_phi          .emplace_back( trk->phi() );
      
      std::vector<std::string> vars { "d0_wrtSV", "z0_wrtSV", "pt_wrtSV", "eta_wrtSV", "phi_wrtSV", "errd0_wrtSV", "errz0_wrtSV", "chi2_toSV"  };
      
      bool flag { true };
      for( auto& var : vars ) {
        if( ! trk->isAvailable<float>( var.c_str() ) ) flag = false;
      }

      if( flag ) {
        vtx_trk_d0_wrtSV     .emplace_back( AUXDYN( trk, float, "d0_wrtSV"    ) );
        vtx_trk_z0_wrtSV     .emplace_back( AUXDYN( trk, float, "z0_wrtSV"    ) );
        vtx_trk_pt_wrtSV     .emplace_back( AUXDYN( trk, float, "pt_wrtSV"    ) );
        vtx_trk_eta_wrtSV    .emplace_back( AUXDYN( trk, float, "eta_wrtSV"   ) );
        vtx_trk_phi_wrtSV    .emplace_back( AUXDYN( trk, float, "phi_wrtSV"   ) );
        vtx_trk_errd0_wrtSV  .emplace_back( AUXDYN( trk, float, "errd0_wrtSV" ) );
        vtx_trk_errz0_wrtSV  .emplace_back( AUXDYN( trk, float, "errz0_wrtSV" ) );
        vtx_trk_chi2_toSV    .emplace_back( AUXDYN( trk, float, "chi2_toSV"   ) );
      } else {
        vtx_trk_d0_wrtSV     .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_z0_wrtSV     .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_pt_wrtSV     .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_eta_wrtSV    .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_phi_wrtSV    .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_errd0_wrtSV  .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_errz0_wrtSV  .emplace_back( AlgConsts::invalidFloat );
        vtx_trk_chi2_toSV    .emplace_back( AlgConsts::invalidFloat );
      }
      
      
      typedef ElementLink<xAOD::TruthParticleContainer> truthLink;
      const xAOD::TruthParticle *truth { nullptr };
      if( trk->isAvailable< truthLink >("truthParticleLink") ) {
        try {
          const truthLink& link = trk->auxdataConst< truthLink >("truthParticleLink");
          truth = *link;
        } catch(...) {}
      }
      
      if( trk->isAvailable<char>("is_svtrk_final") ) {
        vtx_trk_final        .emplace_back( trk->auxdataConst<char>("is_svtrk_final") );
      } else {
        vtx_trk_final        .emplace_back( 0 );
      }
      if( trk->isAvailable<char>("is_selected") ) {
        vtx_trk_isSelected .emplace_back( trk->auxdataConst<char>("is_selected") );
      } else {
        vtx_trk_isSelected .emplace_back( 0 );
      }
      if( trk->isAvailable<char>("is_associated") ) {
        vtx_trk_isAssociated .emplace_back( trk->auxdataConst<char>("is_associated") );
      } else {
        vtx_trk_isAssociated .emplace_back( 0 );
      }
      vtx_trk_nPixelHits         .emplace_back( nPixelHits );
      vtx_trk_nSCTHits           .emplace_back( nSCTHits );
      vtx_trk_nTRTHits           .emplace_back( nTRTHits );
      vtx_trk_nPixelBarrelLayers .emplace_back( nPixelBarrelLayers );
      vtx_trk_nPixelEndCapLayers .emplace_back( nPixelEndCapLayers );
      vtx_trk_nSctBarrelLayers   .emplace_back( nSctBarrelLayers );
      vtx_trk_nSctEndCapLayers   .emplace_back( nSctEndCapLayers );
      vtx_trk_hitPattern         .emplace_back( trk->hitPattern() );
      vtx_trk_dEdx               .emplace_back( pixeldEdx );
      vtx_trk_nUsedHitsdEdx      .emplace_back( nUsedHitsdEdx );
      vtx_trk_nIBLOverflowsdEdx  .emplace_back( nIBLOverflowsdEdx );
      vtx_trk_truthPid           .emplace_back( truth? truth->pdgId() : AlgConsts::invalidInt );
        
    }
    
    if( 0 == vertex->nTrackParticles() ) ATH_MSG_WARNING( "WARNING: vertex ntrk = 0!" );
    if( vtx_trk_pt.size()  == 0 ) ATH_MSG_WARNING( "WARNING: track pt size = 0!" );
    if( vtx_trk_eta.size() == 0 ) ATH_MSG_WARNING( "WARNING: track eta size = 0!" );
    if( vtx_trk_phi.size() == 0 ) ATH_MSG_WARNING( "WARNING: track phi size = 0!" );
    if( vtx_trk_d0.size()  == 0 ) ATH_MSG_WARNING( "WARNING: track d0 size = 0!" );
    if( vtx_trk_z0.size()  == 0 ) ATH_MSG_WARNING( "WARNING: track z0 size = 0!" );
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // DV-Jets (experimental)
    //
    
    // parameters: cone R, power (antikt: -2), pT threshold [MeV]
    auto jets = VsiBonsai::kTalg( filteredTracks, 0.4, -2.0, 3.e3 );
    
    ATH_MSG_DEBUG( " --> found " << jets.size() << " jets" );
    
    std::vector<float> jet_pt;
    std::vector<float> jet_eta;
    std::vector<float> jet_phi;
    std::vector<float> jet_mass;
    std::vector<size_t> jet_ntrk;
    
    for( auto& jet : jets ) {
      auto sumP4 = VsiBonsai::sumP4( jet );
      jet_pt   .emplace_back( sumP4.Pt()  );
      jet_eta  .emplace_back( sumP4.Eta() );
      jet_phi  .emplace_back( sumP4.Phi() );
      jet_mass .emplace_back( sumP4.M()   );
      jet_ntrk .emplace_back( jet.size()  );
      
      ATH_MSG_DEBUG( "   Jet: trk size = " << jet.size() << ", pt = " << sumP4.Pt() << ", eta = " << sumP4.Eta() << ", phi = " << sumP4.Phi() );
    }
    ATH_MSG_DEBUG( "------------------------------------" );
    
    r_vtx_njet     .emplace_back( jets.size() );
    r_vtx_jet_pt   .emplace_back( jet_pt   );
    r_vtx_jet_eta  .emplace_back( jet_eta  );
    r_vtx_jet_phi  .emplace_back( jet_phi  );
    r_vtx_jet_mass .emplace_back( jet_mass );
    r_vtx_jet_ntrk .emplace_back( jet_ntrk );
    
    
    if( ! m_impl->isMC ) continue;
    
    xAOD::TEvent *event = wk()->xaodEvent();
    const xAOD::TruthVertexContainer   *truthVertices    ( nullptr );
    auto statusCode = event->retrieve(truthVertices,    "TruthVertices");
    if( statusCode.isFailure() ) {
      continue;
    }
    
    
#define BACK_ELEM( var ) r_##var.resize( r_##var.size()+1 ); auto& var = r_##var.back();
    
    BACK_ELEM( vtx_closestTruth_outP_pt );
    BACK_ELEM( vtx_closestTruth_outP_eta );
    BACK_ELEM( vtx_closestTruth_outP_phi );
    BACK_ELEM( vtx_closestTruth_outP_charge );
    BACK_ELEM( vtx_closestTruth_outP_pid );
    BACK_ELEM( vtx_closestTruth_outP_isReco );
    
    BACK_ELEM( vtx_reprTruth_outP_pt );
    BACK_ELEM( vtx_reprTruth_outP_eta );
    BACK_ELEM( vtx_reprTruth_outP_phi );
    BACK_ELEM( vtx_reprTruth_outP_charge );
    BACK_ELEM( vtx_reprTruth_outP_pid );
    BACK_ELEM( vtx_reprTruth_outP_isReco );
    
#undef BACK_ELEM
    
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth matching (distance based)
    //
    
    // loop over all truth vertices, and pick-up the closest truth vertex
    const xAOD::TruthVertex* closestTruthVertex { nullptr };
    
    double recoTruthDist { AlgConsts::maxValue };
    
    for( const auto *truthVertex : *truthVertices ) {
      TVector3 reco_pos( vertex->x(), vertex->y(), vertex->z() );
      TVector3 truth_pos( truthVertex->x(), truthVertex->y(), truthVertex->z() );
      double dist = ( reco_pos - truth_pos ).Mag();
      if( recoTruthDist > dist ) {
        recoTruthDist = dist;
        closestTruthVertex = truthVertex;
      }
    }
    
    
    if( closestTruthVertex ) {
      
      TLorentzVector p4sum_in;
      TLorentzVector p4sum_out;
      for( unsigned ip = 0; ip < closestTruthVertex->nIncomingParticles(); ip++ ) {
        auto* particle = closestTruthVertex->incomingParticle(ip);
        if( !particle ) continue;
        TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
        p4sum_in += p4;
      }
      for( unsigned ip = 0; ip < closestTruthVertex->nOutgoingParticles(); ip++ ) {
        auto* particle = closestTruthVertex->outgoingParticle(ip);
        if( !particle ) continue;
        TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
        p4sum_out += p4;
        
        vtx_closestTruth_outP_pt      .emplace_back( particle->pt()    );
        vtx_closestTruth_outP_eta     .emplace_back( particle->eta()   );
        vtx_closestTruth_outP_phi     .emplace_back( particle->phi()   );
        vtx_closestTruth_outP_charge  .emplace_back( static_cast<int>( particle->charge() ) );
        vtx_closestTruth_outP_pid     .emplace_back( particle->pdgId() );
        vtx_closestTruth_outP_isReco  .emplace_back( VsiTruthHelper::isReconstructed( particle, recoTracks ) );
        
      }
      
      const auto* parent = closestTruthVertex->incomingParticle(0);
      
      r_vtx_closestTruth_x           .emplace_back( closestTruthVertex->x()    );
      r_vtx_closestTruth_y           .emplace_back( closestTruthVertex->y()    );
      r_vtx_closestTruth_z           .emplace_back( closestTruthVertex->z()    );
      r_vtx_closestTruth_r           .emplace_back( closestTruthVertex->perp() );
      r_vtx_closestTruth_phi         .emplace_back( closestTruthVertex->phi()  );
      r_vtx_closestTruth_inE         .emplace_back( p4sum_in.E()  );
      r_vtx_closestTruth_outE        .emplace_back( p4sum_out.E() );
      r_vtx_closestTruth_distance    .emplace_back( recoTruthDist );
      r_vtx_closestTruth_parent      .emplace_back( parent? parent->pdgId() : AlgConsts::invalidInt   );
      r_vtx_closestTruth_parent_pt   .emplace_back( parent? parent->pt()    : AlgConsts::invalidFloat );
      r_vtx_closestTruth_parent_eta  .emplace_back( parent? parent->eta()   : AlgConsts::invalidFloat );
      r_vtx_closestTruth_parent_phi  .emplace_back( parent? parent->phi()   : AlgConsts::invalidFloat );
      
      for( auto* trk : filteredTracks ) {
        const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
        vtx_trk_truthIsPointingToClosestTruthVtx.emplace_back( truthProdVtx? (closestTruthVertex == truthProdVtx ) : false );
      }
      
    } else {
      
      vtx_trk_truthIsPointingToClosestTruthVtx.resize( filteredTracks.size(), false );
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth matching (track-truth-matching based)
    //
    
    // List-up all truth vertices linked from tracks
    std::map<const xAOD::TruthVertex*, float> linkedTruthVertices;
    
    for( auto* trk : filteredTracks ) {
      
      const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
      
      if( !truthProdVtx ) continue;
      
      linkedTruthVertices[truthProdVtx] += fabs( trk->pt() ) / scalarSumPt;
    }
    
    if( 0 == linkedTruthVertices.size() ) {
      r_vtx_reprTruth_x         .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_y         .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_z         .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_r         .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_phi       .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_inE       .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_outE      .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_matchScore.emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_parent    .emplace_back( AlgConsts::invalidInt   );
      r_vtx_reprTruth_parent_pt .emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_parent_eta.emplace_back( AlgConsts::invalidFloat );
      r_vtx_reprTruth_parent_phi.emplace_back( AlgConsts::invalidFloat );
      
      vtx_trk_truthIsPointingToReprTruthVtx.resize( filteredTracks.size(), false );
    }
    
    else {
      
      // Pick up the highest score truth vertex as the representative
      // Calculate the pt-weighted score
      auto maxScoreTruth = std::max_element( linkedTruthVertices.begin(), linkedTruthVertices.end(),
                                             []( const std::pair<const xAOD::TruthVertex*, float>& p1, const std::pair<const xAOD::TruthVertex*, float>& p2 ) {
                                               return (p1.second < p2.second); } );
      
      const auto* reprTruthVtx = (*maxScoreTruth).first;
      const auto& score = (*maxScoreTruth).second;
      
      TLorentzVector p4sum_in;
      TLorentzVector p4sum_out;
      for( unsigned ip = 0; ip < reprTruthVtx->nIncomingParticles(); ip++ ) {
        auto* particle = reprTruthVtx->incomingParticle(ip);
        if( !particle ) continue;
        TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
        p4sum_in += p4;
      }
      for( unsigned ip = 0; ip < reprTruthVtx->nOutgoingParticles(); ip++ ) {
        auto* particle = reprTruthVtx->outgoingParticle(ip);
        if( !particle ) continue;
        TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
        p4sum_out += p4;
        
        vtx_reprTruth_outP_pt      .emplace_back( particle->pt()    );
        vtx_reprTruth_outP_eta     .emplace_back( particle->eta()   );
        vtx_reprTruth_outP_phi     .emplace_back( particle->phi()   );
        vtx_reprTruth_outP_charge  .emplace_back( static_cast<int>( particle->charge() ) );
        vtx_reprTruth_outP_pid     .emplace_back( particle->pdgId() );
        vtx_reprTruth_outP_isReco  .emplace_back( VsiTruthHelper::isReconstructed( particle, recoTracks ) );
        
        // ToDo: check if this particle is reconstructed
        
      }
      
      const auto* parent = reprTruthVtx->incomingParticle(0);
      
      r_vtx_reprTruth_x           .emplace_back( reprTruthVtx->x()    );
      r_vtx_reprTruth_y           .emplace_back( reprTruthVtx->y()    );
      r_vtx_reprTruth_z           .emplace_back( reprTruthVtx->z()    );
      r_vtx_reprTruth_r           .emplace_back( reprTruthVtx->perp() );
      r_vtx_reprTruth_phi         .emplace_back( reprTruthVtx->phi()  );
      r_vtx_reprTruth_inE         .emplace_back( p4sum_in.E()  );
      r_vtx_reprTruth_outE        .emplace_back( p4sum_out.E() );
      r_vtx_reprTruth_matchScore  .emplace_back( score         );
      r_vtx_reprTruth_parent      .emplace_back( parent? parent->pdgId() : AlgConsts::invalidInt   );
      r_vtx_reprTruth_parent_pt   .emplace_back( parent? parent->pt()    : AlgConsts::invalidFloat );
      r_vtx_reprTruth_parent_eta  .emplace_back( parent? parent->eta()   : AlgConsts::invalidFloat );
      r_vtx_reprTruth_parent_phi  .emplace_back( parent? parent->phi()   : AlgConsts::invalidFloat );
      
      for( auto* trk : filteredTracks ) {
        const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
        vtx_trk_truthIsPointingToReprTruthVtx.emplace_back( truthProdVtx? ( reprTruthVtx == truthProdVtx ) : false );
      }
      
    }
    
    
  }
  
}


//________________________________________________________________________________________________________________________
void VsiAnaExample::printVertex( const xAOD::Vertex* vertex ) {
  
  ATH_MSG_DEBUG( Form("  vertex: type = %d   |   (r, z, phi) = (%7.2f, %7.2f, %5.2f)   |   num_trks = %3lu   |   chi2/ndof = %5.1f  | mass = %.3e",
                    vertex->vertexType(),
                    vertex->position().perp(), vertex->z(), vertex->position().phi(),
                    vertex->nTrackParticles(),
                    vertex->chiSquared()/vertex->numberDoF(),
                    AUXDYN( vertex, float, "vtx_mass" )  )
            << ( vertex->isAvailable<char>("isFake")? Form(" | isFake = %u", vertex->auxdataConst<char>("isFake") ) : "" )
            );
  
      
  for( size_t itrk = 0; itrk < vertex->nTrackParticles(); itrk++) {
      
    const auto* trk = vertex->trackParticle( itrk );
      
    if( trk->isAvailable<float>("d0_wrtSV_Leptons") ) {
        
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                        "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV(leptons) = (%8.1e, %8.1e)  | *Fake/PU*",
                        trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                        trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                        trk->d0(), trk->z0(),
                        AUXDYN( trk, float, "d0_wrtSV_Leptons" ),
                        AUXDYN( trk, float, "z0_wrtSV_Leptons" ) )
                );
        
    } else if( trk->isAvailable<float>("d0_wrtSV") ) {
        
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                        "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV = (%8.1e, %8.1e)  | *Fake/PU*",
                        trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                        trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                        trk->d0(), trk->z0(),
                        AUXDYN( trk, float, "d0_wrtSV" ),
                        AUXDYN( trk, float, "z0_wrtSV" ) )
                );
        
    } else {
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                        "   (d0, z0) = (%8.1e, %8.1e)  | *Fake/PU*",
                        trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                        trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                        trk->d0(),
                        trk->z0() )
                );
        
    }
      
  }
  
}
    


//________________________________________________________________________________________________________________________
void VsiAnaExample::do_tracks( const xAOD::TrackParticleContainer* recoTracks, const xAOD::TrackParticleContainer* selectedTracks,
                               const xAOD::VertexContainer* /* primaryVertices */,
                               unsigned& /*nPVassoc*/, unsigned& nSelected )
{
  
  REFVAR( trk_d0,       std::vector<float> );
  REFVAR( trk_z0,       std::vector<float> );
  REFVAR( trk_pt,       std::vector<float> );
  REFVAR( trk_eta,      std::vector<float> );
  REFVAR( trk_phi,      std::vector<float> );
  REFVAR( trk_errd0,    std::vector<float> );
  REFVAR( trk_errz0,    std::vector<float> );
  REFVAR( trk_chi2,     std::vector<float> );
  REFVAR( trk_selected, std::vector<uint32_t> );
  
  for( const auto *trk : *recoTracks ) {
    
    if( trk->pt() < 1.e3 ) continue;
    
    m_impl->histos["trk_all_d0"]->Fill( trk->d0() );
    m_impl->histos["trk_all_z0"]->Fill( trk->z0() );
    m_impl->histos["trk_all_pt"]->Fill( trk->pt()/1.e3 );
    m_impl->histos["trk_all_eta"]->Fill( trk->eta() );
    m_impl->histos["trk_all_phi"]->Fill( trk->phi() );
    m_impl->histos["trk_all_chi2"]->Fill( trk->chiSquared()/trk->numberDoF() );
    
    if( static_cast<Release>(prop_release) == Release::r21 ) {
      
      if( trk->isAvailable<char>("is_selected") ) {
        if( trk->auxdataConst<char>("is_selected") ) {
          nSelected++;
        }
      }
      
    }
    
    
    if( prop_fillTracks ) {
      
      if( gRandom->Uniform() < prop_trackStorePrescale ) {
        
        r_trk_d0.emplace_back( trk->d0() );
        r_trk_z0.emplace_back( trk->z0() );
        r_trk_pt.emplace_back( trk->pt() );
        r_trk_eta.emplace_back( trk->eta() );
        r_trk_phi.emplace_back( trk->phi() );
        r_trk_errd0.emplace_back( trk->definingParametersCovMatrix()(0,0) );
        r_trk_errz0.emplace_back( trk->definingParametersCovMatrix()(1,1) );
        r_trk_chi2.emplace_back( trk->chiSquared() / (trk->numberDoF()+AlgConsts::infinitesimal) );
      
        if( static_cast<Release>(prop_release) == Release::r21 ) {
      
          if( trk->isAvailable<char>("is_selected") ) {
            if( trk->auxdataConst<char>("is_selected") ) {
              nSelected++;
              m_impl->histos["trk_d0"]->Fill( trk->d0() );
              m_impl->histos["trk_z0"]->Fill( trk->z0() );
              m_impl->histos["trk_pt"]->Fill( trk->pt()/1.e3 );
              m_impl->histos["trk_eta"]->Fill( trk->eta() );
              m_impl->histos["trk_phi"]->Fill( trk->phi() );
              m_impl->histos["trk_chi2"]->Fill( trk->chiSquared()/trk->numberDoF() );
        
              r_trk_selected.emplace_back( 1 );
            } else {
              r_trk_selected.emplace_back( 0 );
            }
          }
          
          
        }
      }
      
    }
    
#if debug
    ATH_MSG_VERBOSE( Form("  trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |   (d0, z0) = (%7.2f, %7.2f)  | chi2 = %.3e |  %s ",
                 trk->index(), trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(), trk->d0(), trk->z0(), trk->chiSquared()/trk->numberDoF(),
                 trk->isAvailable<char>("is_selected")? (trk->auxdataConst<char>("is_selected")? "selected" : "") : "")
         );
#endif
  }
  
  if( static_cast<Release>(prop_release) == Release::r20p7 ) {
    for( auto *trk : *selectedTracks ) {
      nSelected++;
      m_impl->histos["trk_d0"]->Fill( trk->d0() );
      m_impl->histos["trk_z0"]->Fill( trk->z0() );
      m_impl->histos["trk_pt"]->Fill( trk->pt()/1.e3 );
      m_impl->histos["trk_eta"]->Fill( trk->eta() );
      m_impl->histos["trk_phi"]->Fill( trk->phi() );
      m_impl->histos["trk_chi2"]->Fill( trk->chiSquared()/trk->numberDoF() );
    }
    m_impl->histos["trk_nSelected"]->Fill( selectedTracks->size() );
    
  } else {
    m_impl->histos["trk_nSelected"]->Fill( nSelected );
  }
  
}


//________________________________________________________________________________________________________________________
void VsiAnaExample::do_truth( const xAOD::TruthVertexContainer* truthVertices ) {
  xAOD::TEvent *event = wk()->xaodEvent();
  const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
  auto sc = event->retrieve(recoTracks,       "InDetTrackParticles");
  if( sc.isFailure() ) {
    return;
  }
  
  REFVAR( truth_vtx_r,       std::vector<float> );
  REFVAR( truth_vtx_z,       std::vector<float> );
  REFVAR( truth_vtx_phi,     std::vector<float> );
  
  REFVAR( truth_vtx_parent_pt, std::vector<float> );
  REFVAR( truth_vtx_parent_eta, std::vector<float> );
  REFVAR( truth_vtx_parent_phi, std::vector<float> );
  REFVAR( truth_vtx_parent_pid, std::vector<int> );
  REFVAR( truth_vtx_parent_prod_x, std::vector<float> );
  REFVAR( truth_vtx_parent_prod_y, std::vector<float> );
  REFVAR( truth_vtx_parent_prod_z, std::vector<float> );
  REFVAR( truth_vtx_parent_prod_r, std::vector<float> );
  REFVAR( truth_vtx_parent_prod_phi, std::vector<float> );
  
  REFVAR( truth_vtx_outP_pt,     std::vector< std::vector<float> > );
  REFVAR( truth_vtx_outP_eta,    std::vector< std::vector<float> > );
  REFVAR( truth_vtx_outP_phi,    std::vector< std::vector<float> > );
  REFVAR( truth_vtx_outP_pid,    std::vector< std::vector<int> > );
  REFVAR( truth_vtx_outP_isReco, std::vector< std::vector<uint8_t> > );
  
  static std::map<std::string, bool (*)(const xAOD::TruthVertex*)> pidFuncs;
  if( 0 == pidFuncs.size() ) {
    pidFuncs["Rhadron"]    = VsiTruthHelper::selectRhadron;
    pidFuncs["DarkPhoton"] = VsiTruthHelper::selectDarkPhoton;
    pidFuncs["Bmeson"]     = VsiTruthHelper::selectBmeson;
    pidFuncs["HadInt"]     = VsiTruthHelper::selectHadInt;
    pidFuncs["Kshort"]     = VsiTruthHelper::selectKshort;
  }
  
  m_impl->truthVertices.clear();  
  
  for( const auto *truthVertex : *truthVertices ) {
    if( !pidFuncs[prop_probeTruth]( truthVertex ) ) continue;
    
    m_impl->truthVertices.emplace_back( truthVertex );
    
    r_truth_vtx_r    .emplace_back( truthVertex->perp() );
    r_truth_vtx_z    .emplace_back( truthVertex->z() );
    r_truth_vtx_phi  .emplace_back( truthVertex->phi() );
    
    const auto* parent = truthVertex->incomingParticle(0);
    if( parent ) {
      r_truth_vtx_parent_pt.emplace_back( parent->pt() );
      r_truth_vtx_parent_eta.emplace_back( parent->eta() );
      r_truth_vtx_parent_phi.emplace_back( parent->phi() );
      r_truth_vtx_parent_pid.emplace_back( parent->pdgId() );
      
      const auto* prodVtx = parent->prodVtx();
      r_truth_vtx_parent_prod_x.emplace_back( prodVtx? prodVtx->x() : AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_y.emplace_back( prodVtx? prodVtx->y() : AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_z.emplace_back( prodVtx? prodVtx->z() : AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_r.emplace_back( prodVtx? prodVtx->perp() : AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_phi.emplace_back( prodVtx? prodVtx->phi() : AlgConsts::invalidFloat );
      
    } else {
      r_truth_vtx_parent_pt       .emplace_back ( AlgConsts::invalidFloat );
      r_truth_vtx_parent_eta      .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_phi      .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_pid      .emplace_back( AlgConsts::invalidInt );
      r_truth_vtx_parent_prod_x   .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_y   .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_z   .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_r   .emplace_back( AlgConsts::invalidFloat );
      r_truth_vtx_parent_prod_phi .emplace_back( AlgConsts::invalidFloat );
    }
    
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<int> pid;
    std::vector<uint8_t> isReco;
    
    for( unsigned ip = 0; ip < truthVertex->nOutgoingParticles(); ip++ ) {
      auto* p = truthVertex->outgoingParticle( ip );
        
      if(  fabs( p->charge() ) < 1.0 ) continue;
      
      pt .emplace_back( p->pt()  );
      eta.emplace_back( p->eta() );
      phi.emplace_back( p->phi() );
      pid.emplace_back( p->pdgId() );
      
      bool reco = VsiTruthHelper::isReconstructed( p, recoTracks );
      
      isReco.emplace_back( reco );
    }
    
    r_truth_vtx_outP_pt     .emplace_back( pt  );
    r_truth_vtx_outP_eta    .emplace_back( eta );
    r_truth_vtx_outP_phi    .emplace_back( phi );
    r_truth_vtx_outP_pid    .emplace_back( pid );
    r_truth_vtx_outP_isReco .emplace_back( isReco );
    
  }
}



#undef AUXDYN
#undef REFVAR
