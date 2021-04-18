#ifndef VsiAnalysis_VsiEventDisplay_H
#define VsiAnalysis_VsiEventDisplay_H

#include "include/ProcessorBase.h"
#include "TCanvas.h"

class VsiEventDisplay : public ProcessorBase {
 private:
  
  class Impl;
  std::unique_ptr<Impl> m_impl; //!
  
 public:
  
  enum class Client { kVsiAnaExample };
  
  VsiEventDisplay( const std::string /*name*/ );
  virtual ~VsiEventDisplay();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
  
  void initialize();
  
  void setDoPrint( bool /* flag */ ) const;
  void setDoBatch( bool /* flag */ ) const;
  void setMaxEvents( const unsigned /* nmax */ ) const;
  
  void setTruthName( const std::string /* name */ ) const;
  
  void setScope( const double /*scope*/ ) const;
};

#endif /* VsiAnalysis_VsiEventDisplay_H */
