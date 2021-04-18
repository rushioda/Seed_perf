#ifndef VsiAnalysis_VsiSecondaryVertexBasicProcessor_H
#define VsiAnalysis_VsiSecondaryVertexBasicProcessor_H

#include "include/ProcessorBase.h"
#include "include/VsiBonsaiTool.h"

#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/TrackParticle.h>
#include <xAODTracking/Vertex.h>
#include <xAODTruth/TruthVertexContainer.h>

class VsiSecondaryVertexBasicProcessor : public ProcessorBase {
 private:
  
  VsiBonsai::Configurator m_bonsaiCfg;
  bool m_do_recordTracks;
  bool m_do_jets;
  bool m_do_closestTruth;
  bool m_do_reprTruth;
  
  void recordTracks( std::vector<const xAOD::TrackParticle*>& );
  void processJetsOnDV( std::vector<const xAOD::TrackParticle*>& );
  void processClosestTruth( const xAOD::Vertex*, std::vector<const xAOD::TrackParticle*>&, const xAOD::TrackParticleContainer*, const xAOD::TruthVertexContainer* );
  void processReprTruth   ( const xAOD::Vertex*, std::vector<const xAOD::TrackParticle*>&, const xAOD::TrackParticleContainer*, const xAOD::TruthVertexContainer* );
  
 public:
  VsiSecondaryVertexBasicProcessor( std::string name = "SecondaryVertex" );
  virtual ~VsiSecondaryVertexBasicProcessor();
  
  virtual void registerVariables( EL::Worker* /*wk*/ ) override;
  virtual EL::StatusCode processDetail( xAOD::TEvent* /*event*/, xAOD::TStore* /*store*/ ) override;
  
  inline VsiBonsai::Configurator& bonsaiCfg() { return m_bonsaiCfg; }
  
  inline void doRecordTracks ( bool flag ) { m_do_recordTracks = flag; }
  inline void doJets         ( bool flag ) { m_do_jets = flag; }
  inline void doClosestTruth ( bool flag ) { m_do_closestTruth = flag; }
  inline void doReprTruth    ( bool flag ) { m_do_closestTruth = flag; }
};

#endif /* VsiAnalysis_VsiSecondaryVertexBasicProcessor_H */
