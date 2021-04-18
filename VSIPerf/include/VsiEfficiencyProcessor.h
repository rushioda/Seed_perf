#ifndef VsiAnalysis_VsiEfficiencyProcessor_H
#define VsiAnalysis_VsiEfficiencyProcessor_H

#include "include/ProcessorBase.h"
#include "include/VsiBonsaiTool.h"

class VsiEfficiencyProcessor : public ProcessorBase {
 private:
  
  class Impl;
  std::unique_ptr<Impl> m_impl;
  
 public:
  
  VsiEfficiencyProcessor( std::string name = "DVEff" );
  
  virtual ~VsiEfficiencyProcessor();
  
  void registerVariables( EL::Worker* /*wk*/ ) override;
  
  EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
  
  VsiBonsai::Configurator& bonsaiCfg();
  void setProbeTruth( const std::string name );
  void setDoRecursive( bool flag );
  
};

#endif /* VsiAnalysis_VsiEfficiencyProcessor_H */
