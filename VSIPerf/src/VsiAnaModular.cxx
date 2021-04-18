#include <include/VsiAnaModular.h>

#include <include/VsiTruthHelper.h>
#include <include/VsiEventInfoProcessor.h>
#include <include/VsiTruthProcessor.h>
#include <include/VsiTrackProcessor.h>
#include <include/VsiPrimaryVertexProcessor.h>
#include <include/VsiSecondaryVertexBasicProcessor.h>
#include <include/VsiEfficiencyProcessor.h>
#include <include/VsiEventDisplay.h>

#include <include/AlgConsts.h>

#include <iostream>

// this is needed to distribute the algorithm to the workers
ClassImp(VsiAnaModular)

#define debug 0

#define DEBUG_MSG() std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;

// local convention: use "r_" to express the recording variables

//________________________________________________________________________________________________________________________
VsiAnaModular :: VsiAnaModular()
: ModularAlgorithmBase()
, prop_do_eventInfo       ( true )
, prop_do_truth           ( true )
, prop_do_IDTracksPrescaled ( true )
, prop_do_IDTracksSelected ( true )
, prop_do_primaryVertices ( true )
, prop_do_vsiVertices     ( true )
, prop_do_efficiency      ( true )
, prop_do_display         ( true )
, prop_probeTruth         ( "Rhadron" )
, prop_containerName      ( "SecondaryVertex" )
, prop_release            ( VsiAnaModular::Release::r21 )
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
, prop_displayScope       ( 130 )
, prop_eff_doTruthRecursive ( true )
{}


//________________________________________________________________________________________________________________________
void VsiAnaModular :: initProcessors()
{
  
  //////////////////////////////////////////////////////////////////////////////
  //
  // Setup processors
  //
  
  // EventInfo
  if( prop_do_eventInfo ) {
    auto processor = std::make_unique<VsiEventInfoProcessor>( "EventInfo" );
    m_processors.emplace_back( std::move(processor) );
  }
  
  // Truth
  if( prop_do_truth ) {
    auto processor = std::make_unique<VsiTruthProcessor>( prop_probeTruth + "Truth" );
    processor->setProbeTruth( prop_probeTruth );
    m_processors.emplace_back( std::move(processor) );
  }
  
  // ID tracks, prescaled recording
  if( prop_do_IDTracksPrescaled ) {
    auto processor = std::make_unique<VsiTrackProcessor>( "IDTracksPrescaled" );
    processor->doSample( true );
    processor->doWriteSelectedOnly( false );
    processor->setPrescale( prop_trackStorePrescale );
    m_processors.emplace_back( std::move(processor) );
  }
  
  // ID tracks, saving only selected tracks by VSI
  if( prop_do_IDTracksSelected ) {
    auto processor = std::make_unique<VsiTrackProcessor>( "IDTracksVsiSelected" );
    processor->doSample( false );
    processor->doWriteSelectedOnly( true );
    m_processors.emplace_back( std::move(processor) );
  }
  
  // Primary vertices
  if( prop_do_primaryVertices ) {
    auto processor = std::make_unique<VsiPrimaryVertexProcessor>( "PV" );
    m_processors.emplace_back( std::move(processor) );
  }
  
  // Secondary vertices
  if( prop_do_vsiVertices ) {
    auto processor = std::make_unique<VsiSecondaryVertexBasicProcessor>( "DV" );
    
    auto& bonsaiCfg = processor->bonsaiCfg();
    bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ]        = static_cast<double>( prop_chi2Cut );
    bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = static_cast<VsiTool::HitPatternCondition>( prop_hitPatternReq );
    bonsaiCfg[ VsiBonsai::Config::dropAssociated ]      = static_cast<bool>( prop_doDropAssociated );
    bonsaiCfg[ VsiBonsai::Config::d0_wrtSVCut ]         = static_cast<double>( prop_d0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::z0_wrtSVCut ]         = static_cast<double>( prop_z0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::errd0_wrtSVCut ]      = static_cast<double>( prop_errd0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::errz0_wrtSVCut ]      = static_cast<double>( prop_errz0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::d0signif_wrtSVCut ]   = static_cast<double>( prop_d0signif_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::z0signif_wrtSVCut ]   = static_cast<double>( prop_z0signif_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::chi2_toSVCut ]        = static_cast<double>( prop_chi2_toSVCut );
  
    m_processors.emplace_back( std::move(processor) );
  }
  
  // Efficiency processor
  if( prop_do_efficiency ) {
    auto processor = std::make_unique<VsiEfficiencyProcessor>( "DVeff" );
    processor->setProbeTruth( prop_probeTruth );
    processor->setDoRecursive( prop_eff_doTruthRecursive );
    
    auto& bonsaiCfg = processor->bonsaiCfg();
    bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ]        = static_cast<double>( prop_chi2Cut );
    bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = static_cast<VsiTool::HitPatternCondition>( prop_hitPatternReq );
    bonsaiCfg[ VsiBonsai::Config::dropAssociated ]      = true;
    bonsaiCfg[ VsiBonsai::Config::d0_wrtSVCut ]         = static_cast<double>( prop_d0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::z0_wrtSVCut ]         = static_cast<double>( prop_z0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::errd0_wrtSVCut ]      = static_cast<double>( prop_errd0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::errz0_wrtSVCut ]      = static_cast<double>( prop_errz0_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::d0signif_wrtSVCut ]   = static_cast<double>( prop_d0signif_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::z0signif_wrtSVCut ]   = static_cast<double>( prop_z0signif_wrtSVCut );
    bonsaiCfg[ VsiBonsai::Config::chi2_toSVCut ]        = static_cast<double>( prop_chi2_toSVCut );
  
    m_processors.emplace_back( std::move(processor) );
  }
  
  // Event display
  if( prop_do_display ) {
      auto processor = std::make_unique<VsiEventDisplay>( "Display" );
      processor->initialize();
      processor->setTruthName( prop_probeTruth );
      processor->setScope( prop_displayScope );
      m_processors.emplace_back( std::move( processor ) );
    }
  
}

