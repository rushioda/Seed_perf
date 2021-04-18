#include "include/VsiTruthProcessor.h"
#include "include/VsiTruthHelper.h"
#include <xAODTruth/TruthParticle.h>
#include <xAODTruth/TruthVertex.h>


VsiTruthProcessor::VsiTruthProcessor( std::string name )
  : ProcessorBase( name )
{}



VsiTruthProcessor::~VsiTruthProcessor()
{}


void VsiTruthProcessor::registerVariables(EL::Worker* /*wk*/) {
  
  m_vars["vtx_r"]               = std::vector<float> {};
  m_vars["vtx_z"]               = std::vector<float> {};
  m_vars["vtx_phi"]             = std::vector<float> {};
  m_vars["vtx_parent_pt"]       = std::vector<float> {};
  m_vars["vtx_parent_eta"]      = std::vector<float> {};
  m_vars["vtx_parent_phi"]      = std::vector<float> {};
  m_vars["vtx_parent_pid"]      = std::vector<int> {};
  m_vars["vtx_parent_prod_x"]   = std::vector<float> {};
  m_vars["vtx_parent_prod_y"]   = std::vector<float> {};
  m_vars["vtx_parent_prod_z"]   = std::vector<float> {};
  m_vars["vtx_parent_prod_r"]   = std::vector<float> {};
  m_vars["vtx_parent_prod_phi"] = std::vector<float> {};
  m_vars["vtx_outP_pt"]         = std::vector<std::vector<float>> {};
  m_vars["vtx_outP_eta"]        = std::vector<std::vector<float>> {};
  m_vars["vtx_outP_phi"]        = std::vector<std::vector<float>> {};
  m_vars["vtx_outP_pid"]        = std::vector<std::vector<int>> {};
  m_vars["vtx_outP_isReco"]     = std::vector<std::vector<uint8_t>> {};
  
}


using DescendantProperties = std::tuple< std::vector<float>&, std::vector<float>&, std::vector<float>&,
                                         std::vector<int>&, std::vector<int>&, std::vector<uint8_t>& >;

void traceDescendants( const xAOD::TruthVertex* v,
                       const xAOD::TrackParticleContainer *recoTracks,
                       DescendantProperties prop ) {
  
    auto& pt        = std::get<0>( prop );
    auto& eta       = std::get<1>( prop );
    auto& phi       = std::get<2>( prop );
    auto& pid       = std::get<3>( prop );
    auto& barcode   = std::get<4>( prop );
    auto& isReco    = std::get<5>( prop );
  
    for( unsigned ip = 0; ip < v->nOutgoingParticles(); ip++ ) {
        auto* p = v->outgoingParticle( ip );
      
        if( !p ) {
        	//std::cerr << __PRETTY_FUNCTION__ << ": null outgoing particle detected!" << std::endl;
        	continue;
        }
        
        if( p->hasDecayVtx() ) {
        	traceDescendants( p->decayVtx(), recoTracks, prop );
        }
        
        if( std::abs( p->charge() ) < 1.0 ) continue;
        if( std::find( barcode.begin(), barcode.end(), p->barcode() ) != barcode.end() ) continue;
        
        if( p->hasProdVtx() && p->hasDecayVtx() ) {
            const auto* prod  = p->prodVtx();
            const auto* decay = p->decayVtx();
            auto diffxy = hypot( decay->x() - prod->x(), decay->y() - prod->y() );
            auto diff = hypot( diffxy, decay->z() - prod->z() );
            if( diff < 10.0 ) continue;
        }
        
		pt.emplace_back( p->pt()  );
        eta.emplace_back( p->eta() );
        phi.emplace_back( p->phi() );
        pid.emplace_back( p->pdgId() );
        barcode.emplace_back( p->barcode() );
        
		bool reco = VsiTruthHelper::isReconstructed( p, recoTracks );
        isReco.emplace_back( reco );
	}
           
}



EL::StatusCode VsiTruthProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
  if( !isMC() ) return EL::StatusCode::SUCCESS;
  
  const xAOD::TruthParticleContainer *truthParticles   ( nullptr );
  const xAOD::TruthVertexContainer   *truthVertices    ( nullptr );
  const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
  
  auto sc = event->retrieve(recoTracks,       "InDetTrackParticles");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiTruthProcessor: not finding xAOD::TrackParticleContainer InDetTrackParticles");
  }
  
  sc = event->retrieve(truthVertices,    "TruthVertices");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiTruthProcessor: not finding xAOD::TrackParticleContainer InDetTrackParticles");
  }
  
  sc = event->retrieve(truthParticles,   "TruthParticles");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiTruthProcessor: not finding xAOD::TrackParticleContainer InDetTrackParticles");
  }
  
  
  REFVAR( vtx_r,       std::vector<float> );
  REFVAR( vtx_z,       std::vector<float> );
  REFVAR( vtx_phi,     std::vector<float> );
  
  REFVAR( vtx_parent_pt, std::vector<float> );
  REFVAR( vtx_parent_eta, std::vector<float> );
  REFVAR( vtx_parent_phi, std::vector<float> );
  REFVAR( vtx_parent_pid, std::vector<int> );
  REFVAR( vtx_parent_prod_x, std::vector<float> );
  REFVAR( vtx_parent_prod_y, std::vector<float> );
  REFVAR( vtx_parent_prod_z, std::vector<float> );
  REFVAR( vtx_parent_prod_r, std::vector<float> );
  REFVAR( vtx_parent_prod_phi, std::vector<float> );
  
  REFVAR( vtx_outP_pt,     std::vector< std::vector<float> > );
  REFVAR( vtx_outP_eta,    std::vector< std::vector<float> > );
  REFVAR( vtx_outP_phi,    std::vector< std::vector<float> > );
  REFVAR( vtx_outP_pid,    std::vector< std::vector<int> > );
  REFVAR( vtx_outP_isReco, std::vector< std::vector<uint8_t> > );
  
  
  
  for( const auto *truthVertex : *truthVertices ) {
    
    if( !VsiTruthHelper::pidFuncs[m_probeTruth]( truthVertex ) ) continue;
    
    r_vtx_r    .emplace_back( truthVertex->perp() );
    r_vtx_z    .emplace_back( truthVertex->z() );
    r_vtx_phi  .emplace_back( truthVertex->phi() );
    
    const auto* parent = truthVertex->incomingParticle(0);
    if( parent ) {
      r_vtx_parent_pt.emplace_back( parent->pt() );
      r_vtx_parent_eta.emplace_back( parent->eta() );
      r_vtx_parent_phi.emplace_back( parent->phi() );
      r_vtx_parent_pid.emplace_back( parent->pdgId() );
      
      const auto* prodVtx = parent->prodVtx();
      r_vtx_parent_prod_x.emplace_back( prodVtx? prodVtx->x() : AlgConsts::invalidFloat );
      r_vtx_parent_prod_y.emplace_back( prodVtx? prodVtx->y() : AlgConsts::invalidFloat );
      r_vtx_parent_prod_z.emplace_back( prodVtx? prodVtx->z() : AlgConsts::invalidFloat );
      r_vtx_parent_prod_r.emplace_back( prodVtx? prodVtx->perp() : AlgConsts::invalidFloat );
      r_vtx_parent_prod_phi.emplace_back( prodVtx? prodVtx->phi() : AlgConsts::invalidFloat );
      
    } else {
      r_vtx_parent_pt       .emplace_back ( AlgConsts::invalidFloat );
      r_vtx_parent_eta      .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_phi      .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_pid      .emplace_back( AlgConsts::invalidInt );
      r_vtx_parent_prod_x   .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_prod_y   .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_prod_z   .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_prod_r   .emplace_back( AlgConsts::invalidFloat );
      r_vtx_parent_prod_phi .emplace_back( AlgConsts::invalidFloat );
    }
    
    std::vector<float> pt;
    std::vector<float> eta;
    std::vector<float> phi;
    std::vector<int> pid;
    std::vector<int> barcode;
    std::vector<uint8_t> isReco;
    
 	traceDescendants( truthVertex, recoTracks, { pt, eta, phi, pid, barcode, isReco } );    

    r_vtx_outP_pt     .emplace_back( pt  );
    r_vtx_outP_eta    .emplace_back( eta );
    r_vtx_outP_phi    .emplace_back( phi );
    r_vtx_outP_pid    .emplace_back( pid );
    r_vtx_outP_isReco .emplace_back( isReco );
    
  }
  
  return EL::StatusCode::SUCCESS;
}
