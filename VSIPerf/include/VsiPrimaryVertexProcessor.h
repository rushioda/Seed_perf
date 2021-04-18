#ifndef VsiAnalysis_VsiPrimaryVertexProcessor_H
#define VsiAnalysis_VsiPrimaryVertexProcessor_H

#include "include/ProcessorBase.h"

class VsiPrimaryVertexProcessor : public ProcessorBase {
 public:
  VsiPrimaryVertexProcessor( std::string name = "PrimaryVertex" );
  virtual ~VsiPrimaryVertexProcessor();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
};

#endif /* VsiAnalysis_VsiPrimaryVertexProcessor_H */
