#ifndef VsiAnalysis_VsiTruthProcessor_H
#define VsiAnalysis_VsiTruthProcessor_H

#include "include/ProcessorBase.h"

class VsiTruthProcessor : public ProcessorBase {
 private:
  std::string m_probeTruth;
 public:
  VsiTruthProcessor( std::string name = "Truth" );
  virtual ~VsiTruthProcessor();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
  
  inline void setProbeTruth( std::string name ) { m_probeTruth = name; }
};

#endif /* VsiAnalysis_VsiTruthProcessor_H */
