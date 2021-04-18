#ifndef VsiAnalysis_VsiTrackProcessor_H
#define VsiAnalysis_VsiTrackProcessor_H

#include "include/ProcessorBase.h"

class VsiTrackProcessor : public ProcessorBase {
 private:
  bool m_do_sample;
  bool m_writeSelectedOnly;
  double m_trackStorePrescale;
  
 public:
  VsiTrackProcessor( std::string name = "Track" );
  virtual ~VsiTrackProcessor();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
  inline void doSample( bool flag ) { m_do_sample = flag; }
  inline void doWriteSelectedOnly( bool flag ) { m_writeSelectedOnly = flag; }
  inline void setPrescale( double prescale ) { m_trackStorePrescale = prescale; }
};

#endif /* VsiAnalysis_VsiTrackProcessor_H */
