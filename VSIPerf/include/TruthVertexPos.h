#ifndef __TruthVertexPos_H__
#define __TruthVertexPos_H__

#include <xAODTruth/TruthParticle.h>
#include <xAODTracking/TrackParticle.h>

#include <TVector3.h>
#include <TLorentzVector.h>

#include <memory>
#include <set>
#include <unordered_map>

class TruthVertexPos {
private:
  
  class Impl;
  std::unique_ptr<Impl> m_impl;
  
public:
  TruthVertexPos();
  TruthVertexPos( const TruthVertexPos& );
  TruthVertexPos( const TVector3& /*pos*/ );
  TruthVertexPos( const xAOD::TruthParticle* /*truth*/ );
  
  virtual ~TruthVertexPos();
  
  void setPoistion( const TVector3& pos );
  void emplace( const xAOD::TruthParticle* truth );
  void emplace( const xAOD::TrackParticle* trk );
  
  void storeTracks( const std::unordered_map<const xAOD::TruthParticle*, const xAOD::TrackParticle*>& hash );
  
  TVector3 position()  const;
  size_t size_truths() const;
  size_t size_tracks() const;
  TLorentzVector sumP4_truths() const;
  
  bool isCompatibleWith( const TVector3& pos,              const double thr = 1.0 ) const;
  bool isCompatibleWith( const xAOD::TruthParticle* truth, const double thr = 1.0 ) const;
  
  bool find( const xAOD::TrackParticle* trk ) const;
  
};


#endif
