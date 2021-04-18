#include "include/VsiPrimaryVertexProcessor.h"
#include <xAODTracking/VertexContainer.h>

VsiPrimaryVertexProcessor::VsiPrimaryVertexProcessor( std::string name )
  : ProcessorBase( name )
{}



VsiPrimaryVertexProcessor::~VsiPrimaryVertexProcessor()
{}


void VsiPrimaryVertexProcessor::registerVariables(EL::Worker* /*wk*/) {
  
  m_vars["x"]                   = 0.0F;
  m_vars["y"]                   = 0.0F;
  m_vars["z"]                   = 0.0F;
  m_vars["r"]                   = 0.0F;
  m_vars["phi"]                 = 0.0F;
  m_vars["nTracksPVassoc"]      = uint32_t { 0 };
  
}


EL::StatusCode VsiPrimaryVertexProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
  const xAOD::VertexContainer *primaryVertices       ( nullptr );
  auto sc = event->retrieve(primaryVertices, "PrimaryVertices");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiTruthProcessor: not finding xAOD::VertexContainer PrimaryVertices");
  }
  
  REFVAR( x, float );
  REFVAR( y, float );
  REFVAR( z, float );
  REFVAR( r, float );
  REFVAR( phi, float );
  REFVAR( nTracksPVassoc, uint32_t );
  
  for( auto *vertex : *primaryVertices ) {
    if( vertex->vertexType() == xAOD::VxType::PriVtx ) {
      
      s_thePV = vertex;
      
      r_x   = vertex->x();
      r_y   = vertex->y();
      r_z   = vertex->z();
      r_r   = vertex->position().perp();
      r_phi = vertex->position().phi();
      r_nTracksPVassoc = vertex->nTrackParticles();
      
      break;
    }
  }
  
  return EL::StatusCode::SUCCESS;
  
}
