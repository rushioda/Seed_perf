#include "include/VsiTrackProcessor.h"
#include <xAODTracking/TrackParticleContainer.h>
#include <TRandom2.h>

VsiTrackProcessor::VsiTrackProcessor( std::string name )
  : ProcessorBase( name )
  , m_do_sample( true )
  , m_writeSelectedOnly( false )
  , m_trackStorePrescale( 1.e-2 )
{}



VsiTrackProcessor::~VsiTrackProcessor()
{}


void VsiTrackProcessor::registerVariables(EL::Worker* /*wk*/) {
  
  m_vars["totalSize"]          = uint32_t { 0 };
  m_vars["sampledSize"]        = uint32_t { 0 };
  m_vars["selectedSize"]       = uint32_t { 0 };
  m_vars["d0"]                 = std::vector<float> {};
  m_vars["z0"]                 = std::vector<float> {};
  m_vars["pt"]                 = std::vector<float> {};
  m_vars["eta"]                = std::vector<float> {};
  m_vars["phi"]                = std::vector<float> {};
  m_vars["errd0"]              = std::vector<float> {};
  m_vars["errz0"]              = std::vector<float> {};
  m_vars["chi2"]               = std::vector<float> {};
  m_vars["selected"]           = std::vector<uint32_t> {};
  
}


EL::StatusCode VsiTrackProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
  const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
  auto sc = event->retrieve(recoTracks, "InDetTrackParticles");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiTruthProcessor: not finding xAOD::TrackParticleContainer InDetTrackParticles");
  }
  
  
  REFVAR( totalSize,     uint32_t );
  REFVAR( sampledSize,   uint32_t );
  REFVAR( selectedSize,  uint32_t );
  REFVAR( d0,       std::vector<float> );
  REFVAR( z0,       std::vector<float> );
  REFVAR( pt,       std::vector<float> );
  REFVAR( eta,      std::vector<float> );
  REFVAR( phi,      std::vector<float> );
  REFVAR( errd0,    std::vector<float> );
  REFVAR( errz0,    std::vector<float> );
  REFVAR( chi2,     std::vector<float> );
  REFVAR( selected, std::vector<uint32_t> );
  
  r_totalSize = recoTracks->size();
  
  uint32_t nSampled { 0 };
  uint32_t nSelected { 0 };
  
  for( const auto *trk : *recoTracks ) {
    
    bool selected { false };
    
    if( trk->isAvailable<char>("is_selected") ) {
      if( trk->auxdataConst<char>("is_selected") ) {
        nSelected++;
        selected = true;
      }
    }
      
    if( m_writeSelectedOnly ) {
      if( !selected ) continue;
    }
    
    if( m_do_sample ) {
      if( gRandom->Uniform() > m_trackStorePrescale ) continue;
    }
    
    nSampled++;
    
    r_d0.emplace_back    ( trk->d0() );
    r_z0.emplace_back    ( trk->z0() );
    r_pt.emplace_back    ( trk->pt() );
    r_eta.emplace_back   ( trk->eta() );
    r_phi.emplace_back   ( trk->phi() );
    r_errd0.emplace_back ( trk->definingParametersCovMatrix()(0,0) );
    r_errz0.emplace_back ( trk->definingParametersCovMatrix()(1,1) );
    r_chi2.emplace_back  ( trk->chiSquared() / (trk->numberDoF()+AlgConsts::infinitesimal) );
    r_selected.emplace_back( selected );
    
  }
  
  r_sampledSize  = nSampled;
  r_selectedSize = nSelected;
  
  return EL::StatusCode::SUCCESS;
  
}
