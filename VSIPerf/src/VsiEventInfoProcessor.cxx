#include "include/VsiEventInfoProcessor.h"
#include <xAODEventInfo/EventInfo.h>


VsiEventInfoProcessor::VsiEventInfoProcessor( std::string name )
  : ProcessorBase( name )
{}



VsiEventInfoProcessor::~VsiEventInfoProcessor()
{}


void VsiEventInfoProcessor::registerVariables(EL::Worker* /*wk*/) {
  
  m_vars["runNumber"]              = uint32_t { 0 };
  m_vars["mcChannelNumber"]        = uint32_t { 0 }; 
  m_vars["eventNumber"]            = 0ULL;
  m_vars["lumiBlock"]              = uint32_t { 0 };
  m_vars["averageMu"]              = 0.0F;
  m_vars["actualMu"]               = 0.0F;
  m_vars["mcWeight"]               = 0.0F;
}


EL::StatusCode VsiEventInfoProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
  const xAOD::EventInfo              *eventInfo        ( nullptr );
    
  // retrieving
  if( ! event->retrieve( eventInfo, "EventInfo").isSuccess() ){
    std::cerr << __FUNCTION__ << ":  Failed to retrieve event info collection. Exiting." << std::endl;
    throw std::runtime_error("Failed to retrieve event info collection.");
  }
    
  ProcessorBase::setMC( eventInfo->eventType( xAOD::EventInfo::IS_SIMULATION ) );
    
  REFVAR( runNumber,       uint32_t);
  REFVAR( mcChannelNumber, uint32_t);
  REFVAR( eventNumber,     unsigned long long);
  REFVAR( lumiBlock,       uint32_t);
  REFVAR( averageMu,       float);
  REFVAR( actualMu,        float);
  REFVAR( mcWeight,        float);

  r_runNumber   = eventInfo->runNumber();
  r_mcChannelNumber = eventInfo->mcChannelNumber();
  r_eventNumber = eventInfo->eventNumber();
  r_lumiBlock   = eventInfo->lumiBlock();
  r_averageMu   = eventInfo->averageInteractionsPerCrossing();
  r_actualMu    = eventInfo->actualInteractionsPerCrossing();
  r_mcWeight    = eventInfo->mcEventWeight();
  
  return EL::StatusCode::SUCCESS;
}
