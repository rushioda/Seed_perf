#include <include/TruthVertexPos.h>

#include <xAODTruth/TruthVertex.h>

class TruthVertexPos::Impl {
public:
  TVector3 position;
  std::set<const xAOD::TruthParticle*> truths;
  std::set<const xAOD::TrackParticle*> tracks;
  Impl() {}
  ~Impl() {}
};


//____________________________________________________________________________________________________
TruthVertexPos::TruthVertexPos() : m_impl( new TruthVertexPos::Impl ) {}


//____________________________________________________________________________________________________
TruthVertexPos::TruthVertexPos( const TruthVertexPos& p ) : m_impl( new TruthVertexPos::Impl ) {
  m_impl->position = p.m_impl->position;
  m_impl->truths   = p.m_impl->truths;
  m_impl->tracks   = p.m_impl->tracks;
}


//____________________________________________________________________________________________________
TruthVertexPos::TruthVertexPos( const TVector3& pos ) : m_impl( new TruthVertexPos::Impl ) 
{
  m_impl->position = pos;
}


//____________________________________________________________________________________________________
TruthVertexPos::TruthVertexPos( const xAOD::TruthParticle* truth ) : m_impl( new TruthVertexPos::Impl ) 
{
  m_impl->position = truth->prodVtx()->v4().Vect();
  m_impl->truths.emplace( truth );
}


//____________________________________________________________________________________________________
TruthVertexPos::~TruthVertexPos() {}


//____________________________________________________________________________________________________
void TruthVertexPos::storeTracks( const std::unordered_map<const xAOD::TruthParticle*, const xAOD::TrackParticle*>& hash ) {
  for( const auto* truth : m_impl->truths ) {
    if( hash.find( truth ) != hash.end() ) {
      m_impl->tracks.emplace( hash.at( truth ) );
    }
  }
}


//____________________________________________________________________________________________________
bool TruthVertexPos::find( const xAOD::TrackParticle* trk ) const {
  return ( std::find( m_impl->tracks.begin(), m_impl->tracks.end(), trk ) != m_impl->tracks.end() );
}


//____________________________________________________________________________________________________
void TruthVertexPos::setPoistion( const TVector3& pos ) { m_impl->position = pos; }


//____________________________________________________________________________________________________
void TruthVertexPos::emplace( const xAOD::TruthParticle* truth ) { m_impl->truths.emplace(truth); }


//____________________________________________________________________________________________________
void TruthVertexPos::emplace( const xAOD::TrackParticle* trk )   { m_impl->tracks.emplace(trk); }
  

//____________________________________________________________________________________________________
TVector3 TruthVertexPos::position()  const { return m_impl->position; }


//____________________________________________________________________________________________________
size_t TruthVertexPos::size_truths() const { return m_impl->truths.size(); }


//____________________________________________________________________________________________________
size_t TruthVertexPos::size_tracks() const { return m_impl->tracks.size(); }


//____________________________________________________________________________________________________
TLorentzVector TruthVertexPos::sumP4_truths() const {
  TLorentzVector sumP4;
  for( const auto* truth : m_impl->truths ) { sumP4 += truth->p4(); }
  return sumP4;
}

//____________________________________________________________________________________________________
bool TruthVertexPos::isCompatibleWith( const TVector3& pos, const double thr ) const { return (m_impl->position - pos).Mag() < thr; }


//____________________________________________________________________________________________________
bool TruthVertexPos::isCompatibleWith( const xAOD::TruthParticle* truth, const double thr ) const { return (m_impl->position - truth->prodVtx()->v4().Vect() ).Mag() < thr; }
  
