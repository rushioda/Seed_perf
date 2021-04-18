#ifndef ProcessorBase_H
#define ProcessorBase_H

#include "include/TreeManager.h"
#include "include/AnyType.h"
#include "include/AlgConsts.h"
#include "include/strongErrorCheck.h"

#include <EventLoop/Algorithm.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <xAODTracking/Vertex.h>

#include <map>
#include <string>
#include <limits>

class ProcessorBase {
 protected:
  
  static bool s_isMC;
  static xAOD::Vertex* s_thePV;
  
  std::string m_name;
  std::map<std::string, anytype>& m_vars;
  std::map<std::string, TH1*> m_histos;
  
 public:
  ProcessorBase( std::string name );
  virtual ~ProcessorBase();
  
  inline static bool isMC() { return s_isMC; }
  inline static void setMC( bool flag ) { s_isMC = flag; }
  inline static xAOD::Vertex* getPV() { return s_thePV; }
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) = 0;
  EL::StatusCode process( xAOD::TEvent* /*event*/, xAOD::TStore* store = nullptr );
  void processCommon( xAOD::TEvent* /*event*/, xAOD::TStore* store = nullptr );
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) = 0;
  
  
  template<class HISTO, typename ...REST>
  void addHist( EL::Worker* wk, const std::string name, const std::string title, REST ...rest ) {
    auto* h = new HISTO(name.c_str(), title.c_str(), rest... );
    m_histos[name] = h;
    wk->addOutput( h );
  }
  
  template<class HISTO, typename ...REST>
  void fillHist( const std::string name, REST ...rest ) {
    if( m_histos.find(name) == m_histos.end() ) {
      std::string err = Form( "%s: %s not found in the histogram list!!", __PRETTY_FUNCTION__, name.c_str() );
      std::cerr << err << std::endl;
      throw std::runtime_error( err );
    }
    ( dynamic_cast<HISTO*>( m_histos[name] ) )->Fill( rest... );
  }
  
};

#define DEBUG() std::cout << __PRETTY_FUNCTION__ << ": L" << __LINE__ << std::endl;
#define DEBUGW( x ) std::cout << __PRETTY_FUNCTION__ << ": L" << __LINE__ << ":: " << x << std::endl;

#define REFVAR( NAME, TYPE )                             \
  if( typeid(TYPE) != m_vars.at(#NAME).type() ) {        \
    throw std::runtime_error( #NAME " type mismatch" );    \
  }                                                      \
  auto& r_##NAME = m_vars.at(#NAME).getVar<TYPE>()       \

#define AUXDYN( obj, type, varname ) ( obj->isAvailable<type>(varname)? obj->auxdataConst<type>(varname) : std::numeric_limits<type>::quiet_NaN() )

#endif /* ProcessorBase_H */
