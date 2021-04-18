#ifndef __VsiAnalysis_VsiTruthHelper__
#define __VsiAnalysis_VsiTruthHelper__

#include <xAODTruth/TruthVertex.h>
#include <xAODTruth/TruthParticle.h>
#include <xAODTruth/TruthVertex.h>
#include <xAODTracking/TrackParticle.h>
#include <xAODTracking/TrackParticleContainer.h>
#include <xAODEventInfo/EventInfo.h>

#include <set>
#include <map>
#include <unordered_map>

namespace VsiTruthHelper {
  
  bool selectRhadron            (const xAOD::TruthVertex*);
  bool selectN1                 (const xAOD::TruthVertex*);
  bool selectHiggs              (const xAOD::TruthVertex*);
  bool selectStop1              (const xAOD::TruthVertex*);
  bool selectInvisibleFromHiggs (const xAOD::TruthVertex*);
  bool selectBmeson             (const xAOD::TruthVertex*);
  bool selectDarkPhoton         (const xAOD::TruthVertex*);
  bool selectHNL                (const xAOD::TruthVertex*);
  bool selectKshort             (const xAOD::TruthVertex*);
  bool selectHadInt             (const xAOD::TruthVertex*);
  bool selectHNL                (const xAOD::TruthVertex*);
  
  extern std::map<std::string, bool (*)(const xAOD::TruthVertex*)> pidFuncs;
  
  const xAOD::TruthParticle* getTruthParticle       ( const xAOD::TrackParticle* trk );
  const xAOD::TruthParticle* getParentTruthParticle ( const xAOD::TrackParticle* trk );
  const xAOD::TruthVertex*   getProdVtx             ( const xAOD::TrackParticle* trk );
  
  bool isReconstructed( const xAOD::TruthParticle*, const xAOD::TrackParticleContainer* );

  bool vertexInFiducialVolume(const xAOD::TruthVertex* signalTruthVertex);
  
  void countReconstructibleDescendentParticles(const xAOD::TruthVertex* signalTruthVertex,
                                               std::set<const xAOD::TruthParticle*>& set,
                                               const bool doRecursive,
                                               const double distanceCutoff,
                                               const double pTthr);

  void recursivePrint( const xAOD::TruthVertex* v, std::unordered_map<const xAOD::TruthParticle*, const xAOD::TrackParticle*>& );
}




#endif
