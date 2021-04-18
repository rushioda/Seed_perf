#include "include/VsiSecondaryVertexBasicProcessor.h"
#include "include/VsiTruthHelper.h"

#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/TruthVertexContainer.h>

VsiSecondaryVertexBasicProcessor::VsiSecondaryVertexBasicProcessor( std::string name )
  : ProcessorBase( name )
  , m_bonsaiCfg( VsiBonsai::Configurator{} )
  , m_do_recordTracks( true )
  , m_do_jets( false )
  , m_do_closestTruth( false )
  , m_do_reprTruth( false )
{
  
  VsiBonsai::setDefaultConfig( m_bonsaiCfg );
  
  m_bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ] = 5.0;
  m_bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = VsiTool::HitPatternCondition::NONE;
  m_bonsaiCfg[ VsiBonsai::Config::dropAssociated ] = true;
  
}



VsiSecondaryVertexBasicProcessor::~VsiSecondaryVertexBasicProcessor()
{}


void VsiSecondaryVertexBasicProcessor::registerVariables(EL::Worker* /*wk*/) {
  
  m_vars["x"]                      = std::vector<float> {};
  m_vars["y"]                      = std::vector<float> {};
  m_vars["z"]                      = std::vector<float> {};
  m_vars["r"]                      = std::vector<float> {};
  m_vars["phi"]                    = std::vector<float> {};
  m_vars["covariance"]             = std::vector< std::vector<float> > {};
  m_vars["chi2"]                   = std::vector<float> {};
  m_vars["mass"]                   = std::vector<float> {};
  m_vars["mass_nonAssociated"]     = std::vector<float> {};
  m_vars["direction"]              = std::vector<float> {};
  m_vars["charge"]                 = std::vector<int>   {};
  m_vars["pt"]                     = std::vector<float> {};
  m_vars["H"]                      = std::vector<float> {};
  m_vars["HT"]                     = std::vector<float> {};
  m_vars["minOpAng"]               = std::vector<float> {};
  m_vars["maxOpAng"]               = std::vector<float> {};
  m_vars["mind0"]                  = std::vector<float> {};
  m_vars["maxd0"]                  = std::vector<float> {};
  m_vars["minOneTrackRemovedMass"] = std::vector<float> {};
  m_vars["twoTracksMass"]          = std::vector< std::vector<float> > {};
  m_vars["twoTracksCharge"]        = std::vector< std::vector<int> > {};
  m_vars["twoTracksMassRest"]      = std::vector< std::vector<float> > {};
  m_vars["ntrk"]                   = std::vector<uint32_t> {};
  m_vars["ntrk_selected"]          = std::vector<uint32_t> {};
  m_vars["ntrk_associated"]        = std::vector<uint32_t> {};
  
  if( m_do_recordTracks ) {
    
    m_vars["trk_d0"]                 = std::vector<std::vector<float>> {};
    m_vars["trk_z0"]                 = std::vector<std::vector<float>> {};
    m_vars["trk_qOverP"]             = std::vector<std::vector<float>> {};
    m_vars["trk_pt"]                 = std::vector<std::vector<float>> {};
    m_vars["trk_eta"]                = std::vector<std::vector<float>> {};
    m_vars["trk_phi"]                = std::vector<std::vector<float>> {};
  
    m_vars["trk_d0_wrtSV"]           = std::vector<std::vector<float>> {};
    m_vars["trk_z0_wrtSV"]           = std::vector<std::vector<float>> {};
    m_vars["trk_pt_wrtSV"]           = std::vector<std::vector<float>> {};
    m_vars["trk_eta_wrtSV"]          = std::vector<std::vector<float>> {};
    m_vars["trk_phi_wrtSV"]          = std::vector<std::vector<float>> {};
    m_vars["trk_errd0_wrtSV"]        = std::vector<std::vector<float>> {};
    m_vars["trk_errz0_wrtSV"]        = std::vector<std::vector<float>> {};
    m_vars["trk_chi2_toSV"]          = std::vector<std::vector<float>> {};
  
    m_vars["trk_final"]              = std::vector<std::vector<uint32_t>> {};
    m_vars["trk_charge"]             = std::vector<std::vector<int>> {};
    m_vars["trk_isSelected"]         = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_isAssociated"]       = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nPixelHits"]         = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nSCTHits"]           = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nTRTHits"]           = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nPixelBarrelLayers"] = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nPixelEndCapLayers"] = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nSctBarrelLayers"]   = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nSctEndCapLayers"]   = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_hitPattern"]         = std::vector<std::vector<uint32_t>> {};
    m_vars["trk_dEdx"]               = std::vector<std::vector<float>> {};
    m_vars["trk_nUsedHitsdEdx"]      = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_nIBLOverflowsdEdx"]  = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_truthPid"]           = std::vector<std::vector<int>> {};
    m_vars["trk_truthIsPointingToClosestTruthVtx"] = std::vector<std::vector<uint8_t>> {};
    m_vars["trk_truthIsPointingToReprTruthVtx"]    = std::vector<std::vector<uint8_t>> {};
    
  }
  
  if( m_do_jets ) {
    
    m_vars["njet"]                   = std::vector<size_t> {};
    m_vars["jet_pt"]                 = std::vector<std::vector<float>> {};
    m_vars["jet_eta"]                = std::vector<std::vector<float>> {};
    m_vars["jet_phi"]                = std::vector<std::vector<float>> {};
    m_vars["jet_mass"]               = std::vector<std::vector<float>> {};
    m_vars["jet_ntrk"]               = std::vector<std::vector<size_t>> {};
    
  }
  
  if( m_do_closestTruth ) {
    
    m_vars["closestTruth_x"]            = std::vector<float> {};
    m_vars["closestTruth_y"]            = std::vector<float> {};
    m_vars["closestTruth_z"]            = std::vector<float> {};
    m_vars["closestTruth_r"]            = std::vector<float> {};
    m_vars["closestTruth_phi"]          = std::vector<float> {};
    m_vars["closestTruth_inE"]          = std::vector<float> {};
    m_vars["closestTruth_outE"]         = std::vector<float> {};
    m_vars["closestTruth_distance"]     = std::vector<float> {};
    m_vars["closestTruth_parent"]       = std::vector<int> {};
    m_vars["closestTruth_parent_pt"]    = std::vector<float> {};
    m_vars["closestTruth_parent_eta"]   = std::vector<float> {};
    m_vars["closestTruth_parent_phi"]   = std::vector<float> {};
    m_vars["closestTruth_outP_pt"]      = std::vector< std::vector<float> > {};
    m_vars["closestTruth_outP_eta"]     = std::vector< std::vector<float> > {};
    m_vars["closestTruth_outP_phi"]     = std::vector< std::vector<float> > {};
    m_vars["closestTruth_outP_charge"]  = std::vector< std::vector<int> > {};
    m_vars["closestTruth_outP_pid"]     = std::vector< std::vector<int> > {};
    m_vars["closestTruth_outP_isReco"]  = std::vector< std::vector<uint8_t> > {};
    
  }
  
  if( m_do_reprTruth ) {
    
    m_vars["reprTruth_x"]            = std::vector<float> {};
    m_vars["reprTruth_y"]            = std::vector<float> {};
    m_vars["reprTruth_z"]            = std::vector<float> {};
    m_vars["reprTruth_r"]            = std::vector<float> {};
    m_vars["reprTruth_phi"]          = std::vector<float> {};
    m_vars["reprTruth_inE"]          = std::vector<float> {};
    m_vars["reprTruth_outE"]         = std::vector<float> {};
    m_vars["reprTruth_matchScore"]   = std::vector<float> {};
    m_vars["reprTruth_parent"]       = std::vector<int> {};
    m_vars["reprTruth_parent_pt"]    = std::vector<float> {};
    m_vars["reprTruth_parent_eta"]   = std::vector<float> {};
    m_vars["reprTruth_parent_phi"]   = std::vector<float> {};
    m_vars["reprTruth_outP_pt"]      = std::vector< std::vector<float> > {};
    m_vars["reprTruth_outP_eta"]     = std::vector< std::vector<float> > {};
    m_vars["reprTruth_outP_phi"]     = std::vector< std::vector<float> > {};
    m_vars["reprTruth_outP_charge"]  = std::vector< std::vector<int> > {};
    m_vars["reprTruth_outP_pid"]     = std::vector< std::vector<int> > {};
    m_vars["reprTruth_outP_isReco"]  = std::vector< std::vector<uint8_t> > {};
    
  }
  
}



EL::StatusCode VsiSecondaryVertexBasicProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
#define BACK_ELEM( var ) r_##var.resize( r_##var.size()+1 ); auto& var = r_##var.back();
  
#define DEBUG_LINE() std::cout << __PRETTY_FUNCTION__ << ": " << __LINE__ << std::endl;
  
  const xAOD::VertexContainer *vsiVertices       ( nullptr );
  auto sc = event->retrieve(vsiVertices, "VrtSecInclusive_SecondaryVertices");
  if( sc.isFailure() ) {
    throw std::runtime_error("VsiSecondaryVertexBasicProcessor: not finding xAOD::VertexContainer VrtSecInclusive_SecondaryVertices");
  }
  
  const xAOD::TrackParticleContainer   *recoTracks    ( nullptr );
  auto statusCode = event->retrieve(recoTracks,    "InDetTrackParticles");
  if( statusCode.isFailure() ) {
    throw std::runtime_error("VsiSecondaryVertexBasicProcessor: not finding xAOD::TrackParticleContainer InDetTrackParticles");
  }
    
  const xAOD::TruthVertexContainer   *truthVertices    ( nullptr );
  if( isMC() ) {
    auto statusCode = event->retrieve(truthVertices,    "TruthVertices");
    if( statusCode.isFailure() ) {
      throw std::runtime_error("VsiSecondaryVertexBasicProcessor: not finding xAOD::TruthVertexContainer TruthVertices");
    }
  }
    
    
  REFVAR( x,                      std::vector<float> );
  REFVAR( y,                      std::vector<float> );
  REFVAR( z,                      std::vector<float> );
  REFVAR( r,                      std::vector<float> );
  REFVAR( phi,                    std::vector<float> );
  REFVAR( covariance,             std::vector< std::vector<float> > );
  REFVAR( chi2,                   std::vector<float> );
  REFVAR( mass,                   std::vector<float> );
  REFVAR( mass_nonAssociated,     std::vector<float> );
  REFVAR( direction,              std::vector<float> );
  REFVAR( charge,                 std::vector<int>   );
  REFVAR( pt,                     std::vector<float> );
  REFVAR( H,                      std::vector<float> );
  REFVAR( HT,                     std::vector<float> );
  REFVAR( minOpAng,               std::vector<float> );
  REFVAR( maxOpAng,               std::vector<float> );
  REFVAR( mind0,                  std::vector<float> );
  REFVAR( maxd0,                  std::vector<float> );
  REFVAR( minOneTrackRemovedMass, std::vector<float> );
  REFVAR( twoTracksMass,          std::vector< std::vector<float> > );
  REFVAR( twoTracksCharge,        std::vector< std::vector<int> > );
  REFVAR( twoTracksMassRest,      std::vector< std::vector<float> > );
  REFVAR( ntrk,                   std::vector<uint32_t> );
  REFVAR( ntrk_selected,          std::vector<uint32_t> );
  REFVAR( ntrk_associated,        std::vector<uint32_t> );
  
  std::vector<VsiBonsai::Trimmer> trimmers {
    VsiBonsai::chi2Filter,
    VsiBonsai::ipWrtSVFilter,
    VsiBonsai::hitPatternFilter
  };
  
  
  for( auto *vertex : *vsiVertices ) {
    
    // Filter tracks based on VsiBonsai configuration
    std::vector<const xAOD::TrackParticle*> filteredTracks;
    std::vector<const xAOD::TrackParticle*> droppedTracks;
    
    for( size_t itrk=0; itrk<vertex->nTrackParticles(); itrk++) {
      
      auto* trk = vertex->trackParticle( itrk );
      if( !trk ) continue;
      
      bool flag = true;
        
      for( auto trimmer : trimmers ) {
            
        if( !trimmer( vertex, trk, getPV(), m_bonsaiCfg ) ) {
          flag = false;
          break;
        }
      }
        
      if( flag ) {
          
        filteredTracks.emplace_back( trk );
          
      } else {
          
        droppedTracks.emplace_back( trk );
          
      }
      
    }
    
    if( filteredTracks.size() < 2 ) continue;
    
    
    const TLorentzVector& sumP4 = VsiBonsai::sumP4( filteredTracks );
    const double scalarSumP     = VsiBonsai::scalarSumP( filteredTracks );
    const double scalarSumPt    = VsiBonsai::scalarSumPt( filteredTracks );
    const int sumCharge         = VsiBonsai::sumCharge( filteredTracks );
    
    const TVector3 pos( vertex->x(), vertex->y(), vertex->z() );
    
    auto direction = sumP4.Vect().Dot( pos ) / sumP4.Vect().Mag() / pos.Mag();
    auto pt = sumP4.Pt();
    
    auto mass = sumP4.M();
    
    float mass_nonAssociated { AlgConsts::invalidFloat };
    if( vertex->isAvailable<float>("mass_selectedTracks") ) {
      mass_nonAssociated = vertex->auxdataConst<float>("mass_selectedTracks");
    }
    
    
    auto twoTrackMassTuples = VsiBonsai::allTwoTracksMassCombinations( filteredTracks );
    
    std::vector<float> twoTracksMass;
    std::vector<float> twoTracksMassRest;
    std::vector<int>  twoTracksCharge;
    for( auto& tuple : twoTrackMassTuples ) {
      
      float mass = std::get<0>( tuple );
      float massRest = std::get<1>( tuple );
      
      const auto* trk1 = std::get<2>(tuple).first;
      const auto* trk2 = std::get<2>(tuple).second;
      int charge = trk1->charge() + trk2->charge();
      
      twoTracksMass.emplace_back( mass );
      twoTracksMassRest.emplace_back( massRest );
      twoTracksCharge.emplace_back( charge );
    }
    
    
    r_x                      .emplace_back( vertex->x() );
    r_y                      .emplace_back( vertex->y() );
    r_z                      .emplace_back( vertex->z() );
    r_r                      .emplace_back( vertex->position().perp() );
    r_phi                    .emplace_back( vertex->position().phi() );
    r_covariance             .emplace_back( vertex->covariance() );
    r_chi2                   .emplace_back( vertex->chiSquared()/vertex->numberDoF() );
    r_ntrk                   .emplace_back( filteredTracks.size() );
    r_charge                 .emplace_back( sumCharge );
    r_mass                   .emplace_back( mass );
    r_pt                     .emplace_back( pt );
    r_H                      .emplace_back( scalarSumP );
    r_HT                     .emplace_back( scalarSumPt );
    r_direction              .emplace_back( direction );
    r_mass_nonAssociated     .emplace_back( mass_nonAssociated );
    r_minOpAng               .emplace_back( VsiBonsai::minOpAng( filteredTracks ) );
    r_maxOpAng               .emplace_back( VsiBonsai::maxOpAng( filteredTracks ) );
    r_mind0                  .emplace_back( VsiBonsai::mind0   ( filteredTracks ) );
    r_maxd0                  .emplace_back( VsiBonsai::maxd0   ( filteredTracks ) );
    r_minOneTrackRemovedMass .emplace_back( VsiBonsai::minOneTrackRemovedMass( filteredTracks ) );    
    r_twoTracksMass          .emplace_back( twoTracksMass );
    r_twoTracksMassRest      .emplace_back( twoTracksMassRest );
    r_twoTracksCharge        .emplace_back( twoTracksCharge );
    
    
    size_t ntrk_selected   { 0 };
    size_t ntrk_associated { 0 };
    
    for( const auto* trk : filteredTracks ) {
      if( trk->isAvailable<char>("is_associated") ) {
        ntrk_associated++;
      } else {
        ntrk_selected++;
      }
    }
    
    r_ntrk_selected  .emplace_back( ntrk_selected   );
    r_ntrk_associated.emplace_back( ntrk_associated );
    
    
    if( 0 == vertex->nTrackParticles() ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: vertex ntrk = 0!" << std::endl;
    
    // Tracks associated to the vertex
    if( m_do_recordTracks ) {
      
      recordTracks( filteredTracks );
      
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // DV-Jets (experimental)
    //
    
    if( m_do_jets ) {
      
      processJetsOnDV( filteredTracks );
      
    }
    
    
    if( !isMC() ) continue;
    
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth matching (distance based)
    //
    
    if( m_do_closestTruth ) {
      
      processClosestTruth( vertex, filteredTracks, recoTracks, truthVertices );
      
    }
      
    
    ////////////////////////////////////////////////////////////////////////////////////////
    //
    // Truth matching (track-truth-matching based)
    //
    
    if( m_do_reprTruth ) {
      
      processReprTruth( vertex, filteredTracks, recoTracks, truthVertices );
      
    }
    
  }
  
  return EL::StatusCode::SUCCESS;
  
}


void VsiSecondaryVertexBasicProcessor::recordTracks( std::vector<const xAOD::TrackParticle*>& filteredTracks ) {
  
  REFVAR( trk_d0,                 std::vector< std::vector<float> > );
  REFVAR( trk_z0,                 std::vector< std::vector<float> > );
  REFVAR( trk_qOverP,             std::vector< std::vector<float> > );
  REFVAR( trk_pt,                 std::vector< std::vector<float> > );
  REFVAR( trk_eta,                std::vector< std::vector<float> > );
  REFVAR( trk_phi,                std::vector< std::vector<float> > );
  REFVAR( trk_d0_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( trk_z0_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( trk_pt_wrtSV,           std::vector< std::vector<float> > );
  REFVAR( trk_eta_wrtSV,          std::vector< std::vector<float> > );
  REFVAR( trk_phi_wrtSV,          std::vector< std::vector<float> > );
  REFVAR( trk_errd0_wrtSV,        std::vector< std::vector<float> > );
  REFVAR( trk_errz0_wrtSV,        std::vector< std::vector<float> > );
  REFVAR( trk_chi2_toSV  ,        std::vector< std::vector<float> > );
  REFVAR( trk_final,              std::vector< std::vector<uint32_t> > );
  REFVAR( trk_charge,             std::vector< std::vector<int> > );
  REFVAR( trk_isSelected,         std::vector< std::vector<uint8_t> > );
  REFVAR( trk_isAssociated,       std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nPixelHits,         std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nSCTHits,           std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nTRTHits,           std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nPixelBarrelLayers, std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nPixelEndCapLayers, std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nSctBarrelLayers,   std::vector< std::vector<uint8_t> > );
  REFVAR( trk_nSctEndCapLayers,   std::vector< std::vector<uint8_t> > );
  REFVAR( trk_hitPattern,         std::vector< std::vector<uint32_t> > );
  REFVAR( trk_dEdx,               std::vector< std::vector<float> >   );
  REFVAR( trk_nUsedHitsdEdx,      std::vector< std::vector<uint8_t> >   );
  REFVAR( trk_nIBLOverflowsdEdx,  std::vector< std::vector<uint8_t> >   );
  REFVAR( trk_truthPid,           std::vector< std::vector<int> >   );
  
  BACK_ELEM( trk_d0 );
  BACK_ELEM( trk_z0 );
  BACK_ELEM( trk_pt );
  BACK_ELEM( trk_eta );
  BACK_ELEM( trk_phi );
  BACK_ELEM( trk_qOverP );
  BACK_ELEM( trk_d0_wrtSV );
  BACK_ELEM( trk_z0_wrtSV );
  BACK_ELEM( trk_pt_wrtSV );
  BACK_ELEM( trk_eta_wrtSV );
  BACK_ELEM( trk_phi_wrtSV );
  BACK_ELEM( trk_errd0_wrtSV );
  BACK_ELEM( trk_errz0_wrtSV );
  BACK_ELEM( trk_chi2_toSV );
  BACK_ELEM( trk_final );
  BACK_ELEM( trk_charge );
  BACK_ELEM( trk_isSelected );
  BACK_ELEM( trk_isAssociated );
  BACK_ELEM( trk_nPixelHits );
  BACK_ELEM( trk_nSCTHits );
  BACK_ELEM( trk_nTRTHits );
  BACK_ELEM( trk_nPixelBarrelLayers );
  BACK_ELEM( trk_nPixelEndCapLayers );
  BACK_ELEM( trk_nSctBarrelLayers );
  BACK_ELEM( trk_nSctEndCapLayers );
  BACK_ELEM( trk_hitPattern );
  BACK_ELEM( trk_dEdx );
  BACK_ELEM( trk_nUsedHitsdEdx );
  BACK_ELEM( trk_nIBLOverflowsdEdx );
  BACK_ELEM( trk_truthPid );

#if debug
  ATH_MSG_DEBUG( Form("  PostFiltered vertex: type = %d   |   (r, z, phi) = (%7.2f, %7.2f, %5.2f)   |   num_trks = %3lu   |   chi2/ndof = %5.1f  | mass = %.3e",
                      vertex->vertexType(),
                      vertex->position().perp(), vertex->z(), vertex->position().phi(),
                      vertex->nTrackParticles(),
                      vertex->chiSquared()/vertex->numberDoF(),
                      AUXDYN( vertex, float, "mass" )  )
                 << ( vertex->isAvailable<char>("isFake")? Form(" | isFake = %u", vertex->auxdataConst<char>("isFake") ) : "" )
                 );
#endif
  
  for( auto* trk : filteredTracks ) {
      
    if( !trk ) continue;
      
#if debug
      
    if( trk->isAvailable<float>("d0_wrtSV_Leptons") ) {
        
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                          "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV(leptons) = (%8.1e, %8.1e)  | *Fake/PU*",
                          trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                          trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                          trk->d0(), trk->z0(),
                          AUXDYN( trk, float, "d0_wrtSV_Leptons" ),
                          AUXDYN( trk, float, "z0_wrtSV_Leptons" ) )
                     );
        
    } else if( trk->isAvailable<float>("d0_wrtSV") ) {
        
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                          "   (d0, z0) = (%8.1e, %8.1e), (d0, z0)@SV = (%8.1e, %8.1e)  | *Fake/PU*",
                          trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                          trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                          trk->d0(), trk->z0(),
                          AUXDYN( trk, float, "d0_wrtSV" ),
                          AUXDYN( trk, float, "z0_wrtSV" ) )
                     );
        
    } else {
      ATH_MSG_DEBUG( Form("   + trk: index[%4lu]: (pT, eta, phi, charge) = (%7.3f GeV, %5.2f, %4.1f, %2.0f)   |"
                          "   (d0, z0) = (%8.1e, %8.1e)  | *Fake/PU*",
                          trk->isAvailable<unsigned long>("trk_id")? trk->auxdataConst<unsigned long>("trk_id") : trk->index(),
                          trk->pt()/1.e3, trk->eta(), trk->phi(), trk->charge(),
                          trk->d0(),
                          trk->z0() )
                     );
        
    }
        
#endif
        
    uint8_t nPixelHits        { 0 };
    uint8_t nSCTHits          { 0 };
    uint8_t nTRTHits          { 0 };
    float   pixeldEdx         { 0.0 };
    uint8_t nUsedHitsdEdx     { trk->numberOfUsedHitsdEdx() };
    uint8_t nIBLOverflowsdEdx { trk->numberOfIBLOverflowsdEdx() };
      
    trk->summaryValue( nPixelHits,        xAOD::numberOfPixelHits );
    trk->summaryValue( nSCTHits,          xAOD::numberOfSCTHits );
    trk->summaryValue( nTRTHits,          xAOD::numberOfTRTHits );
    trk->summaryValue( pixeldEdx,         xAOD::pixeldEdx );
      
    uint8_t nPixelBarrelLayers = VsiTool::nPixelBarrelLayers( trk->hitPattern() );
    uint8_t nPixelEndCapLayers = VsiTool::nPixelEndCapLayers( trk->hitPattern() );
    uint8_t nSctBarrelLayers   = VsiTool::nSctBarrelLayers  ( trk->hitPattern() );
    uint8_t nSctEndCapLayers   = VsiTool::nSctEndCapLayers  ( trk->hitPattern() );
      
    trk_charge       .emplace_back( static_cast<int>( trk->charge() ) );
    trk_d0       .emplace_back( trk->d0() );
    trk_z0       .emplace_back( trk->z0() );
    trk_qOverP       .emplace_back( trk->qOverP()  );
    trk_pt           .emplace_back( trk->pt() );
    trk_eta          .emplace_back( trk->eta() );
    trk_phi          .emplace_back( trk->phi() );
      
    std::vector<std::string> vars { "d0_wrtSV", "z0_wrtSV", "pt_wrtSV", "eta_wrtSV", "phi_wrtSV", "errd0_wrtSV", "errz0_wrtSV", "chi2_toSV"  };
      
    bool flag { true };
    for( auto& var : vars ) {
      if( ! trk->isAvailable<float>( var.c_str() ) ) flag = false;
    }

    if( flag ) {
      trk_d0_wrtSV     .emplace_back( AUXDYN( trk, float, "d0_wrtSV"    ) );
      trk_z0_wrtSV     .emplace_back( AUXDYN( trk, float, "z0_wrtSV"    ) );
      trk_pt_wrtSV     .emplace_back( AUXDYN( trk, float, "pt_wrtSV"    ) );
      trk_eta_wrtSV    .emplace_back( AUXDYN( trk, float, "eta_wrtSV"   ) );
      trk_phi_wrtSV    .emplace_back( AUXDYN( trk, float, "phi_wrtSV"   ) );
      trk_errd0_wrtSV  .emplace_back( AUXDYN( trk, float, "errd0_wrtSV" ) );
      trk_errz0_wrtSV  .emplace_back( AUXDYN( trk, float, "errz0_wrtSV" ) );
      trk_chi2_toSV    .emplace_back( AUXDYN( trk, float, "chi2_toSV"   ) );
    } else {
      trk_d0_wrtSV     .emplace_back( AlgConsts::invalidFloat );
      trk_z0_wrtSV     .emplace_back( AlgConsts::invalidFloat );
      trk_pt_wrtSV     .emplace_back( AlgConsts::invalidFloat );
      trk_eta_wrtSV    .emplace_back( AlgConsts::invalidFloat );
      trk_phi_wrtSV    .emplace_back( AlgConsts::invalidFloat );
      trk_errd0_wrtSV  .emplace_back( AlgConsts::invalidFloat );
      trk_errz0_wrtSV  .emplace_back( AlgConsts::invalidFloat );
      trk_chi2_toSV    .emplace_back( AlgConsts::invalidFloat );
    }
      
      
    typedef ElementLink<xAOD::TruthParticleContainer> truthLink;
    const xAOD::TruthParticle *truth { nullptr };
    if( trk->isAvailable< truthLink >("truthParticleLink") ) {
      try {
        const truthLink& link = trk->auxdataConst< truthLink >("truthParticleLink");
        truth = *link;
      } catch(...) {}
    }
      
    if( trk->isAvailable<char>("is_svtrk_final") ) {
      trk_final        .emplace_back( trk->auxdataConst<char>("is_svtrk_final") );
    } else {
      trk_final        .emplace_back( 0 );
    }
    if( trk->isAvailable<char>("is_selected") ) {
      trk_isSelected .emplace_back( trk->auxdataConst<char>("is_selected") );
    } else {
      trk_isSelected .emplace_back( 0 );
    }
    if( trk->isAvailable<char>("is_associated") ) {
      trk_isAssociated .emplace_back( trk->auxdataConst<char>("is_associated") );
    } else {
      trk_isAssociated .emplace_back( 0 );
    }
    trk_nPixelHits         .emplace_back( nPixelHits );
    trk_nSCTHits           .emplace_back( nSCTHits );
    trk_nTRTHits           .emplace_back( nTRTHits );
    trk_nPixelBarrelLayers .emplace_back( nPixelBarrelLayers );
    trk_nPixelEndCapLayers .emplace_back( nPixelEndCapLayers );
    trk_nSctBarrelLayers   .emplace_back( nSctBarrelLayers );
    trk_nSctEndCapLayers   .emplace_back( nSctEndCapLayers );
    trk_hitPattern         .emplace_back( trk->hitPattern() );
    trk_dEdx               .emplace_back( pixeldEdx );
    trk_nUsedHitsdEdx      .emplace_back( nUsedHitsdEdx );
    trk_nIBLOverflowsdEdx  .emplace_back( nIBLOverflowsdEdx );
    trk_truthPid           .emplace_back( truth? truth->pdgId() : AlgConsts::invalidInt );
        
  }
    
    
  if( 0 == trk_pt.size()  ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: track pt size = 0!" << std::endl;
  if( 0 == trk_eta.size() ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: track pt size = 0!" << std::endl;
  if( 0 == trk_phi.size() ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: track pt size = 0!" << std::endl;
  if( 0 == trk_d0.size()  ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: track pt size = 0!" << std::endl;
  if( 0 == trk_z0.size()  ) std::cerr << __PRETTY_FUNCTION__ << ": WARNING: track pt size = 0!" << std::endl;
    
}



void VsiSecondaryVertexBasicProcessor::processJetsOnDV( std::vector<const xAOD::TrackParticle*>& filteredTracks ) {
  
    REFVAR( njet,                   std::vector< size_t > );
    REFVAR( jet_pt,                 std::vector< std::vector<float> > );
    REFVAR( jet_eta,                std::vector< std::vector<float> > );
    REFVAR( jet_phi,                std::vector< std::vector<float> > );
    REFVAR( jet_mass,               std::vector< std::vector<float> > );
    REFVAR( jet_ntrk,               std::vector< std::vector<size_t> > );
  
    // parameters: cone R, power (antikt: -2), pT threshold [MeV]
    auto jets = VsiBonsai::kTalg( filteredTracks, 0.4, -2.0, 3.e3 );
    
    //ATH_MSG_DEBUG( " --> found " << jets.size() << " jets" );
    
    std::vector<float> jet_pt;
    std::vector<float> jet_eta;
    std::vector<float> jet_phi;
    std::vector<float> jet_mass;
    std::vector<size_t> jet_ntrk;
    
    for( auto& jet : jets ) {
      auto sumP4 = VsiBonsai::sumP4( jet );
      jet_pt   .emplace_back( sumP4.Pt()  );
      jet_eta  .emplace_back( sumP4.Eta() );
      jet_phi  .emplace_back( sumP4.Phi() );
      jet_mass .emplace_back( sumP4.M()   );
      jet_ntrk .emplace_back( jet.size()  );
      
      //ATH_MSG_DEBUG( "   Jet: trk size = " << jet.size() << ", pt = " << sumP4.Pt() << ", eta = " << sumP4.Eta() << ", phi = " << sumP4.Phi() );
    }
    //ATH_MSG_DEBUG( "------------------------------------" );
    
    r_njet     .emplace_back( jets.size() );
    r_jet_pt   .emplace_back( jet_pt   );
    r_jet_eta  .emplace_back( jet_eta  );
    r_jet_phi  .emplace_back( jet_phi  );
    r_jet_mass .emplace_back( jet_mass );
    r_jet_ntrk .emplace_back( jet_ntrk );
}



void VsiSecondaryVertexBasicProcessor::processClosestTruth( const xAOD::Vertex* vertex,
                                                            std::vector<const xAOD::TrackParticle*>& filteredTracks,
                                                            const xAOD::TrackParticleContainer* recoTracks,
                                                            const xAOD::TruthVertexContainer* truthVertices )
{
  
  REFVAR( closestTruth_x,               std::vector<float> );
  REFVAR( closestTruth_y,               std::vector<float> );
  REFVAR( closestTruth_z,               std::vector<float> );
  REFVAR( closestTruth_r,               std::vector<float> );
  REFVAR( closestTruth_phi,             std::vector<float> );
  REFVAR( closestTruth_inE,             std::vector<float> );
  REFVAR( closestTruth_outE,            std::vector<float> );
  REFVAR( closestTruth_distance,        std::vector<float> );
  REFVAR( closestTruth_parent,          std::vector<int>   );
  REFVAR( closestTruth_parent_pt,       std::vector<float>   );
  REFVAR( closestTruth_parent_eta,      std::vector<float>   );
  REFVAR( closestTruth_parent_phi,      std::vector<float>   );
  REFVAR( closestTruth_outP_pt,         std::vector< std::vector<float> >   );
  REFVAR( closestTruth_outP_eta,        std::vector< std::vector<float> >   );
  REFVAR( closestTruth_outP_phi,        std::vector< std::vector<float> >   );
  REFVAR( closestTruth_outP_charge,     std::vector< std::vector<int> >     );
  REFVAR( closestTruth_outP_pid,        std::vector< std::vector<int> >     );
  REFVAR( closestTruth_outP_isReco,     std::vector< std::vector<uint8_t> > );
  
  BACK_ELEM( closestTruth_outP_pt );
  BACK_ELEM( closestTruth_outP_eta );
  BACK_ELEM( closestTruth_outP_phi );
  BACK_ELEM( closestTruth_outP_charge );
  BACK_ELEM( closestTruth_outP_pid );
  BACK_ELEM( closestTruth_outP_isReco );
  
  REFVAR( trk_truthIsPointingToClosestTruthVtx, std::vector< std::vector<uint8_t> >     );
  BACK_ELEM( trk_truthIsPointingToClosestTruthVtx );
  
  // loop over all truth vertices, and pick-up the closest truth vertex
  const xAOD::TruthVertex* closestTruthVertex { nullptr };
    
  double recoTruthDist { AlgConsts::maxValue };
    
  for( const auto *truthVertex : *truthVertices ) {
    TVector3 reco_pos( vertex->x(), vertex->y(), vertex->z() );
    TVector3 truth_pos( truthVertex->x(), truthVertex->y(), truthVertex->z() );
    double dist = ( reco_pos - truth_pos ).Mag();
    if( recoTruthDist > dist ) {
      recoTruthDist = dist;
      closestTruthVertex = truthVertex;
    }
  }
    
    
  if( closestTruthVertex ) {
      
    TLorentzVector p4sum_in;
    TLorentzVector p4sum_out;
    for( unsigned ip = 0; ip < closestTruthVertex->nIncomingParticles(); ip++ ) {
      auto* particle = closestTruthVertex->incomingParticle(ip);
      if( !particle ) continue;
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_in += p4;
    }
    for( unsigned ip = 0; ip < closestTruthVertex->nOutgoingParticles(); ip++ ) {
      auto* particle = closestTruthVertex->outgoingParticle(ip);
      if( !particle ) continue;
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_out += p4;
        
      closestTruth_outP_pt      .emplace_back( particle->pt()    );
      closestTruth_outP_eta     .emplace_back( particle->eta()   );
      closestTruth_outP_phi     .emplace_back( particle->phi()   );
      closestTruth_outP_charge  .emplace_back( static_cast<int>( particle->charge() ) );
      closestTruth_outP_pid     .emplace_back( particle->pdgId() );
      closestTruth_outP_isReco  .emplace_back( VsiTruthHelper::isReconstructed( particle, recoTracks ) );
        
    }
      
    const auto* parent = closestTruthVertex->incomingParticle(0);
      
    r_closestTruth_x           .emplace_back( closestTruthVertex->x()    );
    r_closestTruth_y           .emplace_back( closestTruthVertex->y()    );
    r_closestTruth_z           .emplace_back( closestTruthVertex->z()    );
    r_closestTruth_r           .emplace_back( closestTruthVertex->perp() );
    r_closestTruth_phi         .emplace_back( closestTruthVertex->phi()  );
    r_closestTruth_inE         .emplace_back( p4sum_in.E()  );
    r_closestTruth_outE        .emplace_back( p4sum_out.E() );
    r_closestTruth_distance    .emplace_back( recoTruthDist );
    r_closestTruth_parent      .emplace_back( parent? parent->pdgId() : AlgConsts::invalidInt   );
    r_closestTruth_parent_pt   .emplace_back( parent? parent->pt()    : AlgConsts::invalidFloat );
    r_closestTruth_parent_eta  .emplace_back( parent? parent->eta()   : AlgConsts::invalidFloat );
    r_closestTruth_parent_phi  .emplace_back( parent? parent->phi()   : AlgConsts::invalidFloat );
      
    for( auto* trk : filteredTracks ) {
      const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
      trk_truthIsPointingToClosestTruthVtx.emplace_back( truthProdVtx? (closestTruthVertex == truthProdVtx ) : false );
    }
      
  } else {
      
    trk_truthIsPointingToClosestTruthVtx.resize( filteredTracks.size(), false );
      
  }
    
}



void VsiSecondaryVertexBasicProcessor::processReprTruth( const xAOD::Vertex* /*vertex*/,
                                                         std::vector<const xAOD::TrackParticle*>& filteredTracks,
                                                         const xAOD::TrackParticleContainer* recoTracks,
                                                         const xAOD::TruthVertexContainer* /*truthVertices*/ )
{
  
  REFVAR( reprTruth_x,               std::vector<float> );
  REFVAR( reprTruth_y,               std::vector<float> );
  REFVAR( reprTruth_z,               std::vector<float> );
  REFVAR( reprTruth_r,               std::vector<float> );
  REFVAR( reprTruth_phi,             std::vector<float> );
  REFVAR( reprTruth_inE,             std::vector<float> );
  REFVAR( reprTruth_outE,            std::vector<float> );
  REFVAR( reprTruth_matchScore,      std::vector<float> );
  REFVAR( reprTruth_parent,          std::vector<int>   );
  REFVAR( reprTruth_parent_pt,       std::vector<float>   );
  REFVAR( reprTruth_parent_eta,      std::vector<float>   );
  REFVAR( reprTruth_parent_phi,      std::vector<float>   );
  REFVAR( reprTruth_outP_pt,         std::vector< std::vector<float> >   );
  REFVAR( reprTruth_outP_eta,        std::vector< std::vector<float> >   );
  REFVAR( reprTruth_outP_phi,        std::vector< std::vector<float> >   );
  REFVAR( reprTruth_outP_charge,     std::vector< std::vector<int> >     );
  REFVAR( reprTruth_outP_pid,        std::vector< std::vector<int> >     );
  REFVAR( reprTruth_outP_isReco,     std::vector< std::vector<uint8_t> > );
  
  
  BACK_ELEM( reprTruth_outP_pt );
  BACK_ELEM( reprTruth_outP_eta );
  BACK_ELEM( reprTruth_outP_phi );
  BACK_ELEM( reprTruth_outP_charge );
  BACK_ELEM( reprTruth_outP_pid );
  BACK_ELEM( reprTruth_outP_isReco );
  
  
  REFVAR( trk_truthIsPointingToReprTruthVtx,    std::vector< std::vector<uint8_t> >     );
  BACK_ELEM( trk_truthIsPointingToReprTruthVtx );
    
  // List-up all truth vertices linked from tracks
  std::map<const xAOD::TruthVertex*, float> linkedTruthVertices;
  
  const auto& scalarSumPt = VsiBonsai::scalarSumPt( filteredTracks );
    
  for( auto* trk : filteredTracks ) {
      
    const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
      
    if( !truthProdVtx ) continue;
      
    linkedTruthVertices[truthProdVtx] += fabs( trk->pt() ) / scalarSumPt;
  }
    
  if( 0 == linkedTruthVertices.size() ) {
    r_reprTruth_x         .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_y         .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_z         .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_r         .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_phi       .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_inE       .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_outE      .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_matchScore.emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_parent    .emplace_back( AlgConsts::invalidInt   );
    r_reprTruth_parent_pt .emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_parent_eta.emplace_back( AlgConsts::invalidFloat );
    r_reprTruth_parent_phi.emplace_back( AlgConsts::invalidFloat );
      
    trk_truthIsPointingToReprTruthVtx.resize( filteredTracks.size(), false );
  }
    
  else {
      
    // Pick up the highest score truth vertex as the representative
    // Calculate the pt-weighted score
    auto maxScoreTruth = std::max_element( linkedTruthVertices.begin(), linkedTruthVertices.end(),
                                           []( const std::pair<const xAOD::TruthVertex*, float>& p1, const std::pair<const xAOD::TruthVertex*, float>& p2 ) {
                                             return (p1.second < p2.second); } );
      
    const auto* reprTruthVtx = (*maxScoreTruth).first;
    const auto& score = (*maxScoreTruth).second;
      
    TLorentzVector p4sum_in;
    TLorentzVector p4sum_out;
    for( unsigned ip = 0; ip < reprTruthVtx->nIncomingParticles(); ip++ ) {
      auto* particle = reprTruthVtx->incomingParticle(ip);
      if( !particle ) continue;
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_in += p4;
    }
    for( unsigned ip = 0; ip < reprTruthVtx->nOutgoingParticles(); ip++ ) {
      auto* particle = reprTruthVtx->outgoingParticle(ip);
      if( !particle ) continue;
      TLorentzVector p4; p4.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m() );
      p4sum_out += p4;
        
      reprTruth_outP_pt      .emplace_back( particle->pt()    );
      reprTruth_outP_eta     .emplace_back( particle->eta()   );
      reprTruth_outP_phi     .emplace_back( particle->phi()   );
      reprTruth_outP_charge  .emplace_back( static_cast<int>( particle->charge() ) );
      reprTruth_outP_pid     .emplace_back( particle->pdgId() );
      reprTruth_outP_isReco  .emplace_back( VsiTruthHelper::isReconstructed( particle, recoTracks ) );
        
      // ToDo: check if this particle is reconstructed
        
    }
      
    const auto* parent = reprTruthVtx->incomingParticle(0);
      
    r_reprTruth_x           .emplace_back( reprTruthVtx->x()    );
    r_reprTruth_y           .emplace_back( reprTruthVtx->y()    );
    r_reprTruth_z           .emplace_back( reprTruthVtx->z()    );
    r_reprTruth_r           .emplace_back( reprTruthVtx->perp() );
    r_reprTruth_phi         .emplace_back( reprTruthVtx->phi()  );
    r_reprTruth_inE         .emplace_back( p4sum_in.E()  );
    r_reprTruth_outE        .emplace_back( p4sum_out.E() );
    r_reprTruth_matchScore  .emplace_back( score         );
    r_reprTruth_parent      .emplace_back( parent? parent->pdgId() : AlgConsts::invalidInt   );
    r_reprTruth_parent_pt   .emplace_back( parent? parent->pt()    : AlgConsts::invalidFloat );
    r_reprTruth_parent_eta  .emplace_back( parent? parent->eta()   : AlgConsts::invalidFloat );
    r_reprTruth_parent_phi  .emplace_back( parent? parent->phi()   : AlgConsts::invalidFloat );
      
    for( auto* trk : filteredTracks ) {
      const auto* truthProdVtx = VsiTruthHelper::getProdVtx( trk );
      trk_truthIsPointingToReprTruthVtx.emplace_back( truthProdVtx? ( reprTruthVtx == truthProdVtx ) : false );
    }
      
  }
    
}
