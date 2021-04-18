#ifndef VsiAnalysis_VsiEventInfoProcessor_H
#define VsiAnalysis_VsiEventInfoProcessor_H

#include "include/ProcessorBase.h"

class VsiEventInfoProcessor : public ProcessorBase {
 public:
  VsiEventInfoProcessor( std::string name = "EventInfo" );
  virtual ~VsiEventInfoProcessor();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
};

#endif /* VsiAnalysis_VsiEventInfoProcessor_H */
