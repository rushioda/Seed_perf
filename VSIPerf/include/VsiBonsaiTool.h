#ifndef __VsiAnalysis_VsiBonsaiTool__
#define __VsiAnalysis_VsiBonsaiTool__

#include <include/AlgConsts.h>
#include <include/AnyType.h>

#include <AsgTools/MessageCheck.h>

#include <xAODTracking/Vertex.h>
#include <xAODTracking/TrackParticle.h>

#include <map>
#include <tuple>

class TVector3;
class TLorentzVector;

namespace VsiBonsai {
  
  enum class Config {
    trackChi2Cut,
    hitPatternCondition,
    d0_wrtSVCut, z0_wrtSVCut, errd0_wrtSVCut, errz0_wrtSVCut, d0signif_wrtSVCut, z0signif_wrtSVCut, chi2_toSVCut,
    dropAssociated,
    dropNonSelected  
  };
  enum class Error { Unconfigured, TypeError };
  
  using Configurator = std::map<enum Config, anytype>;
  
  void setDefaultConfig( Configurator& );
  void setOutputLevel( MSG::Level level );
  
  using Trimmer = bool (*)( const xAOD::Vertex*, const xAOD::TrackParticle*, const xAOD::Vertex*, const Configurator& );
  
   bool chi2Filter       ( const xAOD::Vertex* /* dv */, const xAOD::TrackParticle* /* dvTrack */ , const xAOD::Vertex* /* pv */, const Configurator& /* config */);
   bool hitPatternFilter ( const xAOD::Vertex* /* dv */, const xAOD::TrackParticle* /* dvTrack */ , const xAOD::Vertex* /* pv */, const Configurator& /* config */);
   bool ipWrtSVFilter    ( const xAOD::Vertex* /* dv */, const xAOD::TrackParticle* /* dvTrack */ , const xAOD::Vertex* /* pv */, const Configurator& /* config */);
   bool dropAssociated   ( const xAOD::Vertex* /* dv */, const xAOD::TrackParticle* /* dvTrack */ , const xAOD::Vertex* /* pv */, const Configurator& /* config */);
   bool dropNonSelected  ( const xAOD::Vertex* /* dv */, const xAOD::TrackParticle* /* dvTrack */ , const xAOD::Vertex* /* pv */, const Configurator& /* config */);
  
  template<enum Config TYPE>
  bool varWrtSVCut( const xAOD::TrackParticle*, const Configurator&, std::string name );
  
  
  ////////////////////////////////////////////////////////////////////////////
  //
  // Tool functions for kinematic properties of the displaced vertex
  //
  ////////////////////////////////////////////////////////////////////////////
  
  
  /**
   *  Returns the sum 4-vector of the constituent tracks using track parameters wrt DV
   */
  TLorentzVector sumP4( const std::vector<const xAOD::TrackParticle*>& );
  TLorentzVector sumP4( const std::vector<TLorentzVector>& );
  
  
  /**
   *  Returns the sum charge of the constituent tracks using track parameters wrt DV
   */
  int sumCharge( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the scalar sum momentum of the constituent tracks using track parameters wrt DV
   */
  double scalarSumP( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the scalar sum pT of the constituent tracks using track parameters wrt DV
   */
  double scalarSumPt( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the cos of the minimimum of opening angles between arbitrary 2-track combinations using track parameters wrt DV
   */
  double minOpAng( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the cos of the maximum of opening angles between arbitrary 2-track combinations using track parameters wrt DV
   */
  double maxOpAng( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the minimum of |d0| (wrt beam spot) among the constituent tracks
   */
  double mind0( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the maximum of |d0| (wrt beam spot) among the constituent tracks
   */
  double maxd0( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the minimum of the invariant mass when arbitrary one track is removed from the system using track parameters wrt DV
   */
  double minOneTrackRemovedMass( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Returns the invariant masses when dividing the system arbitrary two tracks and the rest.
   *  Each try has a return value of tuple< double, double, std::pair<int, int> >
   *  where 1st double is 2-track mass, 2nd double is the rest system's mass,
   *  and the pair<int, int> is the indices of the 2-track mass.
   */
  using TwoTrackMassTuple = std::tuple<double, double, std::pair<const xAOD::TrackParticle*, const xAOD::TrackParticle*> >;
  std::vector< TwoTrackMassTuple >
  allTwoTracksMassCombinations( const std::vector<const xAOD::TrackParticle*>& );
  
  
  /**
   *  Performs kT clustering algorithm on the track collection
   *   - R        : cone radius parameter
   *   - power    : 2.0 = kT, 0.0 = C/A, -2.0: anti-kT
   *   - threshold: minimum jet pT to keep
   */
  std::vector<std::vector<TLorentzVector> > kTalg( const std::vector<const xAOD::TrackParticle*>& /*tracks*/, const double& R = 0.4, const double& power = 2.0, const double& threshold = 5.e3 /*[MeV]*/ );
}


namespace VsiTool {
  
  enum HitPatternCondition { NONE, LOOSE, MEDIUM, TIGHT };

  // vertex area classification.
  // copied from Reconstruction/VKarlVrt/VrtSecInclusive
  
  enum class VertexRegion {
      insideBeamPipe,
    
      insidePixelBarrel0,
      aroundPixelBarrel0,
    
      outsidePixelBarrel0_and_insidePixelBarrel1,
      aroundPixelBarrel1,
    
      outsidePixelBarrel1_and_insidePixelBarrel2,
      aroundPixelBarrel2,
    
      outsidePixelBarrel2_and_insidePixelBarrel3,
      aroundPixelBarrel3,
    
      outsidePixelBarrel3_and_insideSctBarrel0,
      aroundSctBarrel0,
    
      outsideSctBarrel0_and_insideSctBarrel1,
      aroundSctBarrel1,
      
      elsewhere
   };
  
  
  // copied from
  // Tracking/TrkEvent/TrkSummary/TrkTrackSummary/TrackSummary.h
  
  enum DetectorType {
    
    pixelBarrel0 = 0, //!< there are three or four pixel barrel layers (R1/R2)
    pixelBarrel1 = 1,
    pixelBarrel2 = 2,
    pixelBarrel3 = 3,
 
    pixelEndCap0 = 4, //!< three pixel discs (on each side)
    pixelEndCap1 = 5,
    pixelEndCap2 = 6,
 
    sctBarrel0   = 7, //!< four sct barrel layers
    sctBarrel1   = 8,
    sctBarrel2   = 9,
    sctBarrel3   = 10,
 
    sctEndCap0   = 11, //!< and 9 sct discs (on each side)
    sctEndCap1   = 12,
    sctEndCap2   = 13,
    sctEndCap3   = 14,
    sctEndCap4   = 15,
    sctEndCap5   = 16,
    sctEndCap6   = 17,
    sctEndCap7   = 18,
    sctEndCap8   = 19,
 
    trtBarrel    = 20,
    trtEndCap    = 21,
 
    DBM0 = 22,
    DBM1 = 23,
    DBM2 = 24,
 
    numberOfDetectorTypes = 25
 
  };

  const TVector3 dvDirection( const xAOD::Vertex* dv, const xAOD::Vertex* pv );
  const TVector3 trackRelativeDirection( const xAOD::TrackParticle* trk, const xAOD::Vertex* dv, const xAOD::Vertex* pv );
  double dPhiMod( const double& phi1, const double& phi2 );
  enum VertexRegion vertexRegion( const xAOD::Vertex* dv );
  
  template<enum VertexRegion> bool patternMatchInOut( const uint32_t& pattern );
  template<enum VertexRegion> bool patternMatchOuterOnly( const uint32_t& pattern );
  template<enum VertexRegion> bool patternMatchOuterOnlyTight( const uint32_t& pattern );
  
  bool passLoosePattern ( const uint32_t& pattern, const enum VertexRegion&, const TVector3& );
  bool passMediumPattern( const uint32_t& pattern, const enum VertexRegion&, const TVector3& );
  bool passTightPattern ( const uint32_t& pattern, const enum VertexRegion&, const TVector3& );
  
  inline bool hasHit( const uint32_t& pattern, enum DetectorType layer ) { return ( pattern & ( 1<<layer ) ); }
  
  uint8_t nPixelBarrelLayers( const uint32_t& pattern );
  uint8_t nPixelEndCapLayers( const uint32_t& pattern );
  uint8_t nSctBarrelLayers( const uint32_t& pattern );
  uint8_t nSctEndCapLayers( const uint32_t& pattern );
}

#include "VsiBonsaiTool.icc"

#endif /*  __VsiAnalysis_VsiBonsaiTool__ */
