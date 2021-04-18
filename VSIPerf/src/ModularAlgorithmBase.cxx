#include <include/ModularAlgorithmBase.h>

#include <include/TreeManager.h>
#include <include/AnyType.h>
#include <include/AlgConsts.h>
#include <include/strongErrorCheck.h>

#include <AsgTools/MessageCheck.h>

#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"
#include <xAODEventInfo/EventInfo.h>

#include "TFile.h"
#include "TTree.h"

#include <vector>
#include <map>
#include <string.h>
#include <TH1.h>
#include <iostream>
#include <fstream>


// this is needed to distribute the algorithm to the workers
ClassImp(ModularAlgorithmBase)

#define debug 0

#define DEBUG_MSG() std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;

// local convention: use "r_" to express the recording variables

//________________________________________________________________________________________________________________________
void ModularAlgorithmBase::registerVariables() {
  
  for( auto& processor : m_processors ) {
    processor->registerVariables( wk() );
  }
  
  std::cout << "============================================================" << std::endl;
  std::cout << __PRETTY_FUNCTION__ << ": Tree variables" << std::endl;
  std::cout << "------------------------------------------------------------" << std::endl;
  
  for( auto& collection : TreeManager::collections() ) {
    
    auto& collection_name = collection.first;
    auto& vars = collection.second;
    
    std::cout << "\n\n[ " << collection_name << " ]\n" << std::endl;
    
    std::map< std::string, std::vector<std::string> > varGroup;
    
    for( auto& pair : vars ) {

#if debug
      std::cout << pair.first << ": ";
      pair.second.print();
#else
      
      auto& varname = pair.first;
      std::string head, rest;
      if( varname.find("_") == std::string::npos ) {
        head = "";
        rest = varname;
      } else {
        head = varname.substr(0, varname.find("_") );
        rest = varname.substr(varname.find("_")+1 );
      }

      if( varGroup.find( head ) == varGroup.end() ) {
        varGroup[ head ] = std::vector<std::string> { rest };
      } else {
        varGroup[ head ].emplace_back( rest );
      }
      
      //std::cout << pair.first << ", ";
#endif
    
      try {
      
        m_treeManager->book( ( collection_name + "." + pair.first).c_str(), pair.second );
      
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
    for( auto& group : varGroup ) {
      if( group.first != "" ) {
        std::cout << "  " << group.first << "_{ ";
      } else {
        std::cout << "  { ";
      }
      for( auto& n : group.second ) {
        std::cout << n << ", ";
      }
      std::cout << " }, \n" << std::endl;
    }
#endif
    
    std::cout << std::endl;
  }
#if debug
#else
  std::cout << std::endl;
#endif
  std::cout << "============================================================" << std::endl;
  
}


//________________________________________________________________________________________________________________________
ModularAlgorithmBase :: ModularAlgorithmBase()
: EL::Algorithm()
, m_eventCount( 0 )
, m_treeManager( new TreeManager() )
{
  
  std::cout  << __PRETTY_FUNCTION__ << ": Output name = " << prop_outputName << std::endl;
  
}


//________________________________________________________________________________________________________________________
EL::StatusCode ModularAlgorithmBase :: initialize ()
{
  
  std::cout  << __PRETTY_FUNCTION__ << ": Output name = " << prop_outputName << std::endl;
  std::cout  << __PRETTY_FUNCTION__ << ": Output filename = " << prop_outputFileName << std::endl;
  
  initProcessors();
  
  TH1::SetDefaultSumw2();
  
  if( !m_treeManager ) m_treeManager.reset( new TreeManager() );
  m_treeManager->initialize( prop_outputName, wk()->getOutputFile ( prop_outputFileName ) );
  
  registerVariables();
  
  return EL::StatusCode::SUCCESS;
}



//________________________________________________________________________________________________________________________
EL::StatusCode ModularAlgorithmBase :: execute ()
{
  
  try {
    
    if( 0 == m_eventCount % 1000 ) {
      std::cout <<  __PRETTY_FUNCTION__ << ": processed " << m_eventCount << " events." << std::endl;
    }
    
    m_eventCount++;
    
    
    xAOD::TEvent *event = wk()->xaodEvent();
    xAOD::TStore *store = wk()->xaodStore();
    
    const xAOD::EventInfo* eventInfo { nullptr };
    STRONG_CHECK( event->retrieve( eventInfo, "EventInfo") );
    
    if( prop_do_selectRegion && eventInfo->isAvailable<std::string>("regionName") ) {
      std::string regionName = eventInfo->auxdata<std::string>("regionName");
      
      if( prop_regionName != regionName ) {
        return StatusCode::SUCCESS;
      }
    }
    
    for( auto& processor : m_processors ) {
      processor->process( event, store );
    }
  
    m_treeManager->fill();
    
  } catch( std::out_of_range& e ) {
    ATH_MSG_INFO( __PRETTY_FUNCTION__ << "Exception was thrown while retrieving tree variables: " << e.what() );
    throw e;
  }
  
  return StatusCode::SUCCESS;
}


//________________________________________________________________________________________________________________________
EL::StatusCode ModularAlgorithmBase :: finalize ()
{
  std::cout <<  __PRETTY_FUNCTION__ << ": processed " << m_eventCount << " events." << std::endl;
  return StatusCode::SUCCESS;
}



