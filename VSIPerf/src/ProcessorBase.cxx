#include "include/ProcessorBase.h"

bool ProcessorBase::s_isMC = false;

xAOD::Vertex* ProcessorBase::s_thePV = nullptr;

ProcessorBase::ProcessorBase( std::string name )
  : m_name( name )
  , m_vars( TreeManager::collection( name ) )
{}


ProcessorBase::~ProcessorBase(){}

EL::StatusCode ProcessorBase::process( xAOD::TEvent* event, xAOD::TStore* store ) {
  
  processCommon( event, store );
  STRONG_CHECK_SC( processDetail( event, store ) );
  
  return EL::StatusCode::SUCCESS;
}


void ProcessorBase::processCommon( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) {
  
  TreeManager::clearVars( m_name );
  
  return;
}


