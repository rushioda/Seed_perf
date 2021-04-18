#include "include/VsiEfficiencyProcessor.h"
#include "include/VsiTruthHelper.h"
#include "include/AlgConsts.h"
#include "include/TruthVertexPos.h"
#include <xAODTruth/TruthParticle.h>
#include <xAODTruth/TruthVertex.h>
#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>

#include "AthContainers/DataVector.h"
#include "AthContainers/ConstDataVector.h"
#include "xAODEgamma/ElectronContainer.h"
#include "xAODMuon/MuonContainer.h"


#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TVector3.h>

#include <numeric>
#include <unordered_map>
#include <set>

//____________________________________________________________________________________________________
class VsiEfficiencyProcessor::Impl {
public:
  std::string probeTruth;
  bool doRecursive;
  float pTthr;
  float truthMatchResidualLimit;
  float trueRadius;
  float distanceCutoff;
  std::unordered_map<const xAOD::TruthParticle*, const xAOD::TrackParticle*> truthTrackHash;
  VsiBonsai::Configurator bonsaiCfg;
  
  VsiEfficiencyProcessor* parent;
  
  Impl(VsiEfficiencyProcessor*);
  ~Impl() {};
  
  void fillTruthProperties( const xAOD::TruthVertex* );
  
};


//____________________________________________________________________________________________________
#pragma message ( "ToDo: some of these parameters are not yet configurable" )
VsiEfficiencyProcessor::Impl::Impl(VsiEfficiencyProcessor* p)
  : doRecursive( true )
  , pTthr( 1.e3 )
  , truthMatchResidualLimit ( 10.0 )
  , trueRadius( 0.0 )
  , distanceCutoff( 10 )
  , parent( p )
{

  VsiBonsai::setDefaultConfig( bonsaiCfg );
  bonsaiCfg[ VsiBonsai::Config::trackChi2Cut ] = 5.0;
  bonsaiCfg[ VsiBonsai::Config::hitPatternCondition ] = VsiTool::HitPatternCondition::NONE;
  bonsaiCfg[ VsiBonsai::Config::dropAssociated ] = false;
  
}





//____________________________________________________________________________________________________
VsiEfficiencyProcessor::VsiEfficiencyProcessor( std::string name )
  : ProcessorBase( name )
  , m_impl( new Impl(this) )
{}



//____________________________________________________________________________________________________
VsiEfficiencyProcessor::~VsiEfficiencyProcessor()
{}


//____________________________________________________________________________________________________
VsiBonsai::Configurator& VsiEfficiencyProcessor::bonsaiCfg() { return m_impl->bonsaiCfg; }

//____________________________________________________________________________________________________
void VsiEfficiencyProcessor::setProbeTruth( const std::string name ) { m_impl->probeTruth = name; }


//____________________________________________________________________________________________________
void VsiEfficiencyProcessor::setDoRecursive( bool flag ) { m_impl->doRecursive = flag; }


//____________________________________________________________________________________________________
void VsiEfficiencyProcessor::registerVariables(EL::Worker* wk) {
  
#pragma message ( "ToDo: think of making these histogram binning configurable" )

  std::vector<double> rbins = { 0.1, 0.3, 0.5, 1, 2, 3, 5, 7, 10, 14, 20, 28, 38, 50, 64, 80, 100, 130, 170, 220, 280, 350, 450, 600 };
  std::vector<double> nbins = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 16, 20, 24, 28, 38, 50, 70, 100, 150, 220 };
  std::vector<double> mbins;
  
  for( int i=0; i<101; i++) {
    mbins.emplace_back( pow(10, -1.0 + i*0.05) );
  }
  
  addHist<TH1F>     ( wk, "hmult_reconstructible",                   ";N_{ch};Entries",        nbins.size()-1, &(nbins[0]));
  addHist<TH1F>     ( wk, "hmult_reconstructed",                     ";N_{ch};Entries",        nbins.size()-1, &(nbins[0]));
  addHist<TH2F>     ( wk, "hmultR_reconstructible",                  ";N_{ch};r [mm];Entries", nbins.size()-1, &(nbins[0]), rbins.size()-1, &(rbins[0]));
  addHist<TH2F>     ( wk, "hmultR_reconstructed",                    ";N_{ch};r [mm];Entries", nbins.size()-1, &(nbins[0]), rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR",                              ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_truthOK",                      ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_reconstructible",              ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_reconstructed",                ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_reconstructedUnique",          ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_truthChargedMultiplicity",     ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_trackMultiplicity",            ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_trackMultiplicity_selected",   ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hDecayPosR_trackMultiplicity_associated", ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH2F>     ( wk, "hDecayPosR_multiplicityFraction",         ";r [mm];Entries",        rbins.size()-1, &(rbins[0]), 40, 0, 2);
  addHist<TH2F>     ( wk, "hDecayPosR_visibleMassFraction",          ";r [mm];Entries",        rbins.size()-1, &(rbins[0]), 40, 0, 2);
  addHist<TH2F>     ( wk, "hDecayPosR_splitted",                     ";r [mm];Entries",        rbins.size()-1, &(rbins[0]), 10, 0, 10);
  addHist<TH2F>     ( wk, "hDecayPosR_d0",                           ";r [mm];Entries",        rbins.size()-1, &(rbins[0]), rbins.size()-1, &(rbins[0]));
  addHist<TH2F>     ( wk, "hDecayPosR_d0_reconstructed",             ";r [mm];Entries",        rbins.size()-1, &(rbins[0]), rbins.size()-1, &(rbins[0]));
  addHist<TProfile> ( wk, "pDecayPosR_truthChargedMultiplicity",     ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TProfile> ( wk, "pDecayPosR_trackMultiplicity",            ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TProfile> ( wk, "pDecayPosR_trackMultiplicity_selected",   ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TProfile> ( wk, "pDecayPosR_trackMultiplicity_associated", ";r [mm];Entries",        rbins.size()-1, &(rbins[0]));
  addHist<TH1F>     ( wk, "hMaxRTruthVertices",                      "max #DeltaR truth vertices",40,0,20);
  addHist<TH1F>     ( wk, "hNvtx_truth",                             ";N_{vtx};Events",        100, 0, 100);
  addHist<TH1F>     ( wk, "hNvtx_reco",                              ";N_{vtx};Events",        100, 0, 100);
  addHist<TH2F>     ( wk, "hPartDphiR",                              ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hPartDphiR_reco",                         ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hPartDphiRPositive",                      ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hPartDphiRPositive_reco",                 ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hPartDphiRNegative",                      ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hPartDphiRNegative_reco",                 ";#Delta#phi wrt. Decay Vector;r [mm];Particles", 50, -TMath::Pi(), TMath::Pi(), rbins.size()-1, &(rbins[0]) );
  addHist<TH2F>     ( wk, "hVisibleMassMap",                         ";Reconstructible Visible Mass [GeV];Reconstructed Visible Mass [GeV]", mbins.size()-1, &(mbins[0]), mbins.size()-1, &(mbins[0]) );
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Ntuple Variables

#pragma message ( "ToDo: validate if the following variable set is really ok to evaluate performances" )
  
  m_vars["LLP_size"]                            = 0UL; // size_t literal
  m_vars["LLP_decay_r"]                         = std::vector<float>{};
  m_vars["LLP_decay_x"]                         = std::vector<float>{};
  m_vars["LLP_decay_y"]                         = std::vector<float>{};
  m_vars["LLP_decay_z"]                         = std::vector<float>{};
  m_vars["LLP_decay_phi"]                       = std::vector<float>{};
  m_vars["LLP_pt"]                              = std::vector<float>{};
  m_vars["LLP_eta"]                             = std::vector<float>{};
  m_vars["LLP_phi"]                             = std::vector<float>{};
  m_vars["LLP_mass"]                            = std::vector<float>{};
  m_vars["LLP_childOpenAngle"]                  = std::vector<float>{};
  m_vars["LLP_pid"]                             = std::vector<int>{};
  
  m_vars["truthPos_size"]                       = std::vector< size_t >{};
  m_vars["truthPos_r"]                          = std::vector< std::vector<float> >{};
  m_vars["truthPos_x"]                          = std::vector< std::vector<float> >{};
  m_vars["truthPos_y"]                          = std::vector< std::vector<float> >{};
  m_vars["truthPos_z"]                          = std::vector< std::vector<float> >{};
  m_vars["truthPos_phi"]                        = std::vector< std::vector<float> >{};
  m_vars["truthPos_nReconstructible"]           = std::vector< std::vector<size_t> >{};
  m_vars["truthPos_reconstructibleMass"]        = std::vector< std::vector<float> >{};
  m_vars["truthPos_sum_pt"]                     = std::vector< std::vector<float> >{};
  m_vars["truthPos_sum_eta"]                    = std::vector< std::vector<float> >{};
  m_vars["truthPos_sum_phi"]                    = std::vector< std::vector<float> >{};
  
  m_vars["descendent_size"]                     = std::vector< size_t >{};
  m_vars["descendent_reco_size"]                = std::vector< size_t >{};
  m_vars["descendent_selected_size"]            = std::vector< size_t >{};
  m_vars["descendent_sum_pt"]                   = std::vector< float >{};
  m_vars["descendent_sum_eta"]                  = std::vector< float >{};
  m_vars["descendent_sum_phi"]                  = std::vector< float >{};
  m_vars["descendent_sum_mass"]                 = std::vector< float >{};
  m_vars["descendent_reco_sum_pt"]              = std::vector< float >{};
  m_vars["descendent_reco_sum_eta"]             = std::vector< float >{};
  m_vars["descendent_reco_sum_phi"]             = std::vector< float >{};
  m_vars["descendent_reco_sum_mass"]            = std::vector< float >{};
  m_vars["descendent_truthPosIndex"]            = std::vector< std::vector<size_t> >{};
  m_vars["descendent_prod_r"]                   = std::vector< std::vector<float> >{};
  m_vars["descendent_prod_x"]                   = std::vector< std::vector<float> >{};
  m_vars["descendent_prod_y"]                   = std::vector< std::vector<float> >{};
  m_vars["descendent_prod_z"]                   = std::vector< std::vector<float> >{};
  m_vars["descendent_prod_phi"]                 = std::vector< std::vector<float> >{};
  m_vars["descendent_decay_r"]                  = std::vector< std::vector<float> >{};
  m_vars["descendent_decay_x"]                  = std::vector< std::vector<float> >{};
  m_vars["descendent_decay_y"]                  = std::vector< std::vector<float> >{};
  m_vars["descendent_decay_z"]                  = std::vector< std::vector<float> >{};
  m_vars["descendent_decay_phi"]                = std::vector< std::vector<float> >{};
  m_vars["descendent_pid"]                      = std::vector< std::vector<int> >{};
  m_vars["descendent_pt"]                       = std::vector< std::vector<float> >{};
  m_vars["descendent_eta"]                      = std::vector< std::vector<float> >{};
  m_vars["descendent_phi"]                      = std::vector< std::vector<float> >{};
  m_vars["descendent_mass"]                     = std::vector< std::vector<float> >{};
  m_vars["descendent_charge"]                   = std::vector< std::vector<float> >{};
  m_vars["descendent_isReconstructed"]          = std::vector< std::vector<char> >{};
  m_vars["descendent_isAssociated"]             = std::vector< std::vector<char> >{};
  m_vars["descendent_isSelected"]               = std::vector< std::vector<char> >{};
  m_vars["descendent_d0"]                       = std::vector< std::vector<float> >{};
  m_vars["descendent_z0"]                       = std::vector< std::vector<float> >{};
  m_vars["descendent_refReco_r"]                = std::vector< std::vector<float> >{};
  m_vars["descendent_refReco_z"]                = std::vector< std::vector<float> >{};
  m_vars["descendent_refReco_phi"]              = std::vector< std::vector<float> >{};
  m_vars["descendent_refReco_dist"]             = std::vector< std::vector<float> >{};
  m_vars["descendent_passHitPattern"]           = std::vector< std::vector<char> >{};
  
  m_vars["recoVtx_index"]                       = std::vector<size_t>{};
  m_vars["recoVtx_LLP_index"]                   = std::vector<size_t>{};
  m_vars["recoVtx_truthPos_index"]              = std::vector<size_t>{};
  m_vars["recoVtx_matchScore"]                  = std::vector<float>{};
  m_vars["recoVtx_r"]                           = std::vector<float>{};
  m_vars["recoVtx_x"]                           = std::vector<float>{};
  m_vars["recoVtx_y"]                           = std::vector<float>{};
  m_vars["recoVtx_z"]                           = std::vector<float>{};
  m_vars["recoVtx_phi"]                         = std::vector<float>{};
  m_vars["recoVtx_residual_r"]                  = std::vector<float>{};
  m_vars["recoVtx_residual_x"]                  = std::vector<float>{};
  m_vars["recoVtx_residual_y"]                  = std::vector<float>{};
  m_vars["recoVtx_residual_z"]                  = std::vector<float>{};
  m_vars["recoVtx_residual_phi"]                = std::vector<float>{};
  m_vars["recoVtx_chi2"]                        = std::vector<float>{};
  m_vars["recoVtx_chi2_core"]                   = std::vector<float>{};
  m_vars["recoVtx_mass"]                        = std::vector<float>{};
  m_vars["recoVtx_mass_nonAssociated"]          = std::vector<float>{};
  m_vars["recoVtx_ntrk"]                        = std::vector<size_t>{};
  m_vars["recoVtx_ntrk_nonAssociated"]          = std::vector<size_t>{};
  m_vars["recoVtx_massFraction"]                = std::vector<float>{};
  m_vars["recoVtx_multiplicityFraction"]        = std::vector<float>{};
  m_vars["recoVtx_filtered_mass"]               = std::vector<float>{};
  m_vars["recoVtx_filtered_mass_nonAssociated"] = std::vector<float>{};
  m_vars["recoVtx_filtered_ntrk"]               = std::vector<size_t>{};
  m_vars["recoVtx_filtered_ntrk_nonAssociated"] = std::vector<size_t>{};

}



//____________________________________________________________________________________________________
EL::StatusCode VsiEfficiencyProcessor::processDetail( xAOD::TEvent* event, xAOD::TStore* /*store*/ ) {
  
  if( !isMC() )  return EL::StatusCode::SUCCESS;
  if( !s_thePV ) return EL::StatusCode::SUCCESS;
  
  const xAOD::TruthParticleContainer *truthParticles   ( nullptr );
  const xAOD::TruthVertexContainer   *truthVertices    ( nullptr );
  const xAOD::TrackParticleContainer *recoTracks       ( nullptr );
  const xAOD::VertexContainer        *vsiVertices      ( nullptr );
  

  STRONG_CHECK_SC( event->retrieve(truthVertices,    "TruthVertices")                     );
  STRONG_CHECK_SC( event->retrieve(truthParticles,   "TruthParticles")                    );

  const xAOD::ElectronContainer *electrons    ( nullptr );
  const xAOD::MuonContainer* muons            ( nullptr );
  ConstDataVector<xAOD::TrackParticleContainer> recoTrks(SG::VIEW_ELEMENTS);


  bool m_doLeptonMode = false;
  if( !m_doLeptonMode ){
    STRONG_CHECK_SC( event->retrieve(recoTracks,       "InDetTrackParticles")               );
    STRONG_CHECK_SC( event->retrieve(vsiVertices,      "VrtSecInclusive_SecondaryVertices") );
  }
  else{

    STRONG_CHECK_SC( event->retrieve( muons    , "Muons"                                     ) );
    STRONG_CHECK_SC( event->retrieve( electrons, "Electrons"                                 ) );
    STRONG_CHECK_SC( event->retrieve(vsiVertices, "VrtSecInclusive_SecondaryVertices_Leptons") );

    for( const auto& electron : *electrons ) {
      if( 0 == electron->nTrackParticles() ) continue;
      const auto* trk = electron->trackParticle(0);
      if( !trk ) continue;
      recoTrks.push_back(trk);
    }
    for( const auto& muon : *muons ) {
      const auto* trk = muon->trackParticle( xAOD::Muon::InnerDetectorTrackParticle );
      if( !trk ) continue;
      recoTrks.push_back(trk);
    }
    recoTracks = recoTrks.asDataVector();
  } // Lepton mode

  //////////////////////////////////////////////////////////////////////////////////////////////////
  // VsiBonsai trimmers
  
  std::vector<VsiBonsai::Trimmer> trimmers {
    VsiBonsai::dropAssociated,
    VsiBonsai::chi2Filter,
    VsiBonsai::ipWrtSVFilter,
    VsiBonsai::hitPatternFilter
  };
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Ntuple Variables, can become accesible with prefix "r_" (e.g. r_LLP_decay_r)
  
  REFVAR( LLP_size,                             size_t                             );
  REFVAR( LLP_decay_r,                          std::vector<float>                 );
  REFVAR( LLP_decay_x,                          std::vector<float>                 );
  REFVAR( LLP_decay_y,                          std::vector<float>                 );
  REFVAR( LLP_decay_z,                          std::vector<float>                 );
  REFVAR( LLP_decay_phi,                        std::vector<float>                 );
  REFVAR( LLP_pt,                               std::vector<float>                 );
  REFVAR( LLP_eta,                              std::vector<float>                 );
  REFVAR( LLP_phi,                              std::vector<float>                 );
  REFVAR( LLP_mass,                             std::vector<float>                 );
  REFVAR( LLP_childOpenAngle,                   std::vector<float>                 );
  REFVAR( LLP_pid,                              std::vector<int>                   );
  
  REFVAR( truthPos_size,                        std::vector< size_t >              );
  REFVAR( truthPos_r,                           std::vector< std::vector<float> >  );
  REFVAR( truthPos_x,                           std::vector< std::vector<float> >  );
  REFVAR( truthPos_y,                           std::vector< std::vector<float> >  );
  REFVAR( truthPos_z,                           std::vector< std::vector<float> >  );
  REFVAR( truthPos_phi,                         std::vector< std::vector<float> >  );
  REFVAR( truthPos_nReconstructible,            std::vector< std::vector<size_t> > );
  REFVAR( truthPos_reconstructibleMass,         std::vector< std::vector<float> >  );
  REFVAR( truthPos_sum_pt,                      std::vector< std::vector<float> >  );
  REFVAR( truthPos_sum_eta,                     std::vector< std::vector<float> >  );
  REFVAR( truthPos_sum_phi,                     std::vector< std::vector<float> >  );
  
  REFVAR( descendent_size,                      std::vector< size_t >              );
  REFVAR( descendent_reco_size,                 std::vector< size_t >              );
  REFVAR( descendent_selected_size,             std::vector< size_t >              );
  REFVAR( descendent_sum_pt,                    std::vector< float >               );
  REFVAR( descendent_sum_eta,                   std::vector< float >               );
  REFVAR( descendent_sum_phi,                   std::vector< float >               );
  REFVAR( descendent_sum_mass,                  std::vector< float >               );
  REFVAR( descendent_reco_sum_pt,               std::vector< float >               );
  REFVAR( descendent_reco_sum_eta,              std::vector< float >               );
  REFVAR( descendent_reco_sum_phi,              std::vector< float >               );
  REFVAR( descendent_reco_sum_mass,             std::vector< float >               );
  REFVAR( descendent_truthPosIndex,             std::vector< std::vector<size_t> > );
  REFVAR( descendent_prod_r,                    std::vector< std::vector<float> >  );
  REFVAR( descendent_prod_x,                    std::vector< std::vector<float> >  );
  REFVAR( descendent_prod_y,                    std::vector< std::vector<float> >  );
  REFVAR( descendent_prod_z,                    std::vector< std::vector<float> >  );
  REFVAR( descendent_prod_phi,                  std::vector< std::vector<float> >  );
  REFVAR( descendent_decay_r,                   std::vector< std::vector<float> >  );
  REFVAR( descendent_decay_x,                   std::vector< std::vector<float> >  );
  REFVAR( descendent_decay_y,                   std::vector< std::vector<float> >  );
  REFVAR( descendent_decay_z,                   std::vector< std::vector<float> >  );
  REFVAR( descendent_decay_phi,                 std::vector< std::vector<float> >  );
  REFVAR( descendent_pid,                       std::vector< std::vector<int> >    );
  REFVAR( descendent_pt,                        std::vector< std::vector<float> >  );
  REFVAR( descendent_eta,                       std::vector< std::vector<float> >  );
  REFVAR( descendent_phi,                       std::vector< std::vector<float> >  );
  REFVAR( descendent_mass,                      std::vector< std::vector<float> >  );
  REFVAR( descendent_charge,                    std::vector< std::vector<float> >  );
  REFVAR( descendent_isReconstructed,           std::vector< std::vector<char> >   );
  REFVAR( descendent_isAssociated,              std::vector< std::vector<char> >   );
  REFVAR( descendent_isSelected,                std::vector< std::vector<char> >   );
  REFVAR( descendent_d0,                        std::vector< std::vector<float> >  );
  REFVAR( descendent_z0,                        std::vector< std::vector<float> >  );
  REFVAR( descendent_refReco_r,                 std::vector< std::vector<float> >  );
  REFVAR( descendent_refReco_z,                 std::vector< std::vector<float> >  );
  REFVAR( descendent_refReco_phi,               std::vector< std::vector<float> >  );
  REFVAR( descendent_refReco_dist,              std::vector< std::vector<float> >  );
  REFVAR( descendent_passHitPattern,            std::vector< std::vector<char> >  );
  
  REFVAR( recoVtx_index,                        std::vector<size_t>                );
  REFVAR( recoVtx_LLP_index,                    std::vector<size_t>                );
  REFVAR( recoVtx_truthPos_index,               std::vector<size_t>                );
  REFVAR( recoVtx_matchScore,                   std::vector<float>                 );
  REFVAR( recoVtx_r,                            std::vector<float>                 );
  REFVAR( recoVtx_x,                            std::vector<float>                 );
  REFVAR( recoVtx_y,                            std::vector<float>                 );
  REFVAR( recoVtx_z,                            std::vector<float>                 );
  REFVAR( recoVtx_phi,                          std::vector<float>                 );
  REFVAR( recoVtx_residual_r,                   std::vector<float>                 );
  REFVAR( recoVtx_residual_x,                   std::vector<float>                 );
  REFVAR( recoVtx_residual_y,                   std::vector<float>                 );
  REFVAR( recoVtx_residual_z,                   std::vector<float>                 );
  REFVAR( recoVtx_residual_phi,                 std::vector<float>                 );
  REFVAR( recoVtx_chi2,                         std::vector<float>                 );
  REFVAR( recoVtx_chi2_core,                    std::vector<float>                 );
  REFVAR( recoVtx_mass,                         std::vector<float>                 );
  REFVAR( recoVtx_mass_nonAssociated,           std::vector<float>                 );
  REFVAR( recoVtx_ntrk,                         std::vector<size_t>                );
  REFVAR( recoVtx_ntrk_nonAssociated,           std::vector<size_t>                );
  REFVAR( recoVtx_massFraction,                 std::vector<float>                 );
  REFVAR( recoVtx_multiplicityFraction,         std::vector<float>                 );
  REFVAR( recoVtx_filtered_mass,                std::vector<float>                 );
  REFVAR( recoVtx_filtered_mass_nonAssociated,  std::vector<float>                 );
  REFVAR( recoVtx_filtered_ntrk,                std::vector<size_t>                );
  REFVAR( recoVtx_filtered_ntrk_nonAssociated,  std::vector<size_t>                );
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  m_impl->trueRadius = 0.0;
  


  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Create truth - track hash table first
  
  m_impl->truthTrackHash.clear();
  for( const auto& trk : *recoTracks ) { 
    auto* truth = VsiTruthHelper::getTruthParticle( trk );
    if( truth ) {
      m_impl->truthTrackHash[truth] = trk;
    }
  }
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Filter truth vertices
  
  std::vector<const xAOD::TruthVertex*> signalTruthVertices;
  
  for( const auto *truthVertex : *truthVertices ) {
    
    if( !VsiTruthHelper::pidFuncs[ m_impl->probeTruth ]( truthVertex ) ) continue;
    
    signalTruthVertices.emplace_back( truthVertex );
    
  }
  
  // DEBUGW( "signalTruthVertices.size() = " << signalTruthVertices.size() );
  
  fillHist<TH1F>( "hNvtx_truth", signalTruthVertices.size() );
  
  
  //////////////////////////////////////////////////////////////////////////////////////////////////
  // Truth vertex loop
  
  unsigned nvtx_reco = 0;
  
  // DEBUG();
  
  //r_LLP_size = signalTruthVertices.size();
  
  r_LLP_size = 0;
 
  for( size_t llpIndex = 0; llpIndex < signalTruthVertices.size(); llpIndex++ ) {
    
    const auto* signalTruthVertex = signalTruthVertices.at( llpIndex );

    // Only consider truth vertices 
    // in the fiducal volume
    if( !VsiTruthHelper::vertexInFiducialVolume(signalTruthVertex) ) continue;
    r_LLP_size++;

    
    // DEBUG();
    // DEBUGW( "truth (r,z) = (" << signalTruthVertex->perp() << ", " << signalTruthVertex->z() << "), "
    //         "pid = "  << signalTruthVertex->incomingParticle(0)->pdgId() << ", "
    //         "mass = " << signalTruthVertex->incomingParticle(0)->m() << " GeV" );
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Fill the LLP properties to the ntuple
    //
    
    r_LLP_decay_r   .emplace_back( signalTruthVertex->perp() );
    r_LLP_decay_x   .emplace_back( signalTruthVertex->x() );
    r_LLP_decay_y   .emplace_back( signalTruthVertex->y() );
    r_LLP_decay_z   .emplace_back( signalTruthVertex->z() );
    r_LLP_decay_phi .emplace_back( signalTruthVertex->phi() );
    r_LLP_pt        .emplace_back( signalTruthVertex->incomingParticle(0)->pt() );
    r_LLP_eta       .emplace_back( signalTruthVertex->incomingParticle(0)->eta() );
    r_LLP_phi       .emplace_back( signalTruthVertex->incomingParticle(0)->phi() );
    r_LLP_mass      .emplace_back( signalTruthVertex->incomingParticle(0)->m() );
    r_LLP_pid       .emplace_back( signalTruthVertex->incomingParticle(0)->pdgId() );

    float childOpenAngle = -1.0;

    // First try for HNL
    if( abs(signalTruthVertex->incomingParticle(0)->pdgId())==50 ){
      const xAOD::TruthParticle* lep1 = 0;
      const xAOD::TruthParticle* lep2 = 0;
      for( unsigned ip = 0; ip < signalTruthVertex->nOutgoingParticles(); ip++ ) {
        auto* p = signalTruthVertex->outgoingParticle(ip);
        if( !p ) continue;
        if( abs( p->pdgId() ) == 13 || abs( p->pdgId() ) == 11 ) { // checking for muons or electrons
          if( !lep1 ){
	    lep1 = p;
            continue;
          }
          if( !lep2 ) lep2 = p;
        }
      }
      if( lep1 && lep2 ){
        childOpenAngle = lep1->p4().DeltaR(lep2->p4());  
      }
    } // HNL
    // Otherwise take the maximum separation between any two tracks
    else{
      for( unsigned ip = 0; ip < signalTruthVertex->nOutgoingParticles(); ip++ ) {
        auto* p = signalTruthVertex->outgoingParticle(ip);
        for( unsigned ip1 = 0; ip1 < signalTruthVertex->nOutgoingParticles(); ip1++ ) {
          auto* p1 = signalTruthVertex->outgoingParticle(ip1);
          if( p==p1 ) continue;
          float dR = p1->p4().DeltaR(p->p4());
          if( dR>childOpenAngle ) childOpenAngle=dR;
        }
      }
    }
    
    r_LLP_childOpenAngle.emplace_back( childOpenAngle );
    
#define BACKELEM( name, init ) r_##name.emplace_back( init ); auto& b_##name = r_##name.back();
    
    // Fill the container to each of the following variables
    // back lemen is accessible with b_ (e.g. b_truthPos_size )
    
    BACKELEM( truthPos_size                       , 0UL );
    BACKELEM( truthPos_r                          , std::vector<float> {}  );
    BACKELEM( truthPos_x                          , std::vector<float> {}  );
    BACKELEM( truthPos_y                          , std::vector<float> {}  );
    BACKELEM( truthPos_z                          , std::vector<float> {}  );
    BACKELEM( truthPos_phi                        , std::vector<float> {}  );
    BACKELEM( truthPos_nReconstructible           , std::vector<size_t> {} );
    BACKELEM( truthPos_reconstructibleMass        , std::vector<float> {}  );
    BACKELEM( truthPos_sum_pt                     , std::vector<float> {}  );
    BACKELEM( truthPos_sum_eta                    , std::vector<float> {}  );
    BACKELEM( truthPos_sum_phi                    , std::vector<float> {}  );
  
    BACKELEM( descendent_size                     , 0UL );
    BACKELEM( descendent_reco_size                , 0UL );
    BACKELEM( descendent_selected_size            , 0UL );
    BACKELEM( descendent_sum_pt                   , 0.0F );
    BACKELEM( descendent_sum_eta                  , 0.0F );
    BACKELEM( descendent_sum_phi                  , 0.0F );
    BACKELEM( descendent_sum_mass                 , 0.0F );
    BACKELEM( descendent_reco_sum_pt              , 0.0F );
    BACKELEM( descendent_reco_sum_eta             , 0.0F );
    BACKELEM( descendent_reco_sum_phi             , 0.0F );
    BACKELEM( descendent_reco_sum_mass            , 0.0F );
    BACKELEM( descendent_truthPosIndex            , std::vector<size_t> {}  );
    BACKELEM( descendent_prod_r                   , std::vector<float> {}   );
    BACKELEM( descendent_prod_x                   , std::vector<float> {}   );
    BACKELEM( descendent_prod_y                   , std::vector<float> {}   );
    BACKELEM( descendent_prod_z                   , std::vector<float> {}   );
    BACKELEM( descendent_prod_phi                 , std::vector<float> {}   );
    BACKELEM( descendent_decay_r                  , std::vector<float> {}   );
    BACKELEM( descendent_decay_x                  , std::vector<float> {}   );
    BACKELEM( descendent_decay_y                  , std::vector<float> {}   );
    BACKELEM( descendent_decay_z                  , std::vector<float> {}   );
    BACKELEM( descendent_decay_phi                , std::vector<float> {}   );
    BACKELEM( descendent_pid                      , std::vector<int> {}     );
    BACKELEM( descendent_pt                       , std::vector<float> {}   );
    BACKELEM( descendent_eta                      , std::vector<float> {}   );
    BACKELEM( descendent_phi                      , std::vector<float> {}   );
    BACKELEM( descendent_mass                     , std::vector<float> {}   );
    BACKELEM( descendent_charge                   , std::vector<float> {}   );
    BACKELEM( descendent_isReconstructed          , std::vector<char> {}    );
    BACKELEM( descendent_isAssociated             , std::vector<char> {}    );
    BACKELEM( descendent_isSelected               , std::vector<char> {}    );
    BACKELEM( descendent_d0                       , std::vector<float> {}    );
    BACKELEM( descendent_z0                       , std::vector<float> {}    );
    BACKELEM( descendent_refReco_r                , std::vector<float> {}    );
    BACKELEM( descendent_refReco_z                , std::vector<float> {}    );
    BACKELEM( descendent_refReco_phi              , std::vector<float> {}    );
    BACKELEM( descendent_refReco_dist             , std::vector<float> {}    );
    BACKELEM( descendent_passHitPattern           , std::vector<char> {}    );
#undef BACKELEM  
    
  
    TVector3 pos( signalTruthVertex->x(), signalTruthVertex->y(), signalTruthVertex->z() );
    TVector3 pos_PV( s_thePV->x(), s_thePV->y(), s_thePV->z() );
    
    m_impl->trueRadius = (pos - pos_PV).Perp();
    
    fillHist<TH1F>( "hDecayPosR", m_impl->trueRadius );
    
    m_impl->fillTruthProperties( signalTruthVertex );
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Count how many reconstructible outgoing particles are present for this signal
    //
    std::set<const xAOD::TruthParticle*> reconstructibleParticles;
    
    VsiTruthHelper::countReconstructibleDescendentParticles( signalTruthVertex, reconstructibleParticles,
                                                             m_impl->doRecursive, m_impl->distanceCutoff, m_impl->pTthr );
    
    const auto& nReconstructibleParticles = reconstructibleParticles.size();
    // DEBUGW( "nReconstructible = " << nReconstructibleParticles );
    
    std::vector<TruthVertexPos> truthPhysicalPositions;
    
    for( const auto* truth : reconstructibleParticles ) {
      if( !truth->hasProdVtx() ) continue;
      if( !truth->prodVtx()    ) continue;

#if 0
      // Hide 2020-06-17: seems this part was modified
      if( truthPhysicalPositions.size()==0 ){
        truthPhysicalPositions.emplace_back( TruthVertexPos(truth) );
        continue;
      }      
      truthPhysicalPositions[0].emplace( truth );
#endif

      bool flag { false };
      for( auto& p : truthPhysicalPositions ) {
        if( p.isCompatibleWith( truth ) ) {
          p.emplace( truth );
          flag = true;
          break;
        }
      }
      if( !flag ) {
        truthPhysicalPositions.emplace_back( TruthVertexPos(truth) );
      }
    }
    
    double max_separation=-1.0;
    for( auto& p1 : truthPhysicalPositions ){
      for( auto& p2 : truthPhysicalPositions ){
        double d = (p1.position()-p2.position()).Mag();
        if(d>0 && d>max_separation ) max_separation = d;
      }
    }

    fillHist<TH1F>    ( "hMaxRTruthVertices", max_separation );
    
    for( auto& tpos : truthPhysicalPositions ) {
      
      b_truthPos_size++;
      
      b_truthPos_r                   .emplace_back( tpos.position().Perp() );
      b_truthPos_x                   .emplace_back( tpos.position().x() );
      b_truthPos_y                   .emplace_back( tpos.position().y() );
      b_truthPos_z                   .emplace_back( tpos.position().z() );
      b_truthPos_phi                 .emplace_back( tpos.position().Phi() );
      b_truthPos_nReconstructible    .emplace_back( tpos.size_truths() );
      
      auto sumP4 = tpos.sumP4_truths();
      
      b_truthPos_reconstructibleMass .emplace_back( sumP4.M()   );
      b_truthPos_sum_pt              .emplace_back( sumP4.Pt()  );
      b_truthPos_sum_eta             .emplace_back( sumP4.Eta() );
      b_truthPos_sum_phi             .emplace_back( sumP4.Phi() );

    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Associate the truth particles to the truth vertex positions
    // Also fill the truth vertex positions properties to the ntuple
    //
    
    for( auto& cvtx : truthPhysicalPositions ) {
      
      cvtx.storeTracks( m_impl->truthTrackHash );
      
      // DEBUGW( "  candidate truth vertex pos (r,z) = (" << cvtx.position().Perp() << ", " << cvtx.position().z() << "), truth mult = " << cvtx.size_truths() );
    }
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Fill the reconstructible perticle properties to the ntuple
    //
    
    std::set<const xAOD::TruthParticle*> reconstructedParticles;
    std::set<const xAOD::TruthParticle*> reconstructedParticles_selected;
    
    
    auto reconstructibleSumP4 = std::accumulate( reconstructibleParticles.begin(), reconstructibleParticles.end(),
                                                 TLorentzVector{}, []( TLorentzVector p4, const xAOD::TruthParticle* p ) { p4 += p->p4(); return p4; } );
    
    b_descendent_sum_pt   = reconstructibleSumP4.Pt();
    b_descendent_sum_eta  = reconstructibleSumP4.Eta();
    b_descendent_sum_phi  = reconstructibleSumP4.Phi();
    b_descendent_sum_mass = reconstructibleSumP4.M();
    
    
    for( const auto* truth : reconstructibleParticles ) {
      
      b_descendent_size++;
      
      
      // TruthPos index
      auto truthPosIndex = AlgConsts::invalidUnsigned;
      
      for( size_t index = 0; index < truthPhysicalPositions.size(); ++index ) {
        const auto& cvtx = truthPhysicalPositions.at(index);
        if( cvtx.isCompatibleWith( truth ) ) {
          truthPosIndex = index;
          break;
        }
      }
      
      b_descendent_truthPosIndex.emplace_back( truthPosIndex );
          
      
      const auto* prodVtx = truth->prodVtx();
      const auto* decayVtx = truth->decayVtx();
      
      b_descendent_prod_r          .emplace_back( prodVtx  ? prodVtx->perp()  : AlgConsts::invalidFloat );
      b_descendent_prod_x          .emplace_back( prodVtx  ? prodVtx->x()     : AlgConsts::invalidFloat );
      b_descendent_prod_y          .emplace_back( prodVtx  ? prodVtx->y()     : AlgConsts::invalidFloat );
      b_descendent_prod_z          .emplace_back( prodVtx  ? prodVtx->z()     : AlgConsts::invalidFloat );
      b_descendent_prod_phi        .emplace_back( prodVtx  ? prodVtx->phi()   : AlgConsts::invalidFloat );
      b_descendent_decay_r         .emplace_back( decayVtx ? decayVtx->perp() : AlgConsts::invalidFloat );
      b_descendent_decay_x         .emplace_back( decayVtx ? decayVtx->x()    : AlgConsts::invalidFloat );
      b_descendent_decay_y         .emplace_back( decayVtx ? decayVtx->y()    : AlgConsts::invalidFloat );
      b_descendent_decay_z         .emplace_back( decayVtx ? decayVtx->z()    : AlgConsts::invalidFloat );
      b_descendent_decay_phi       .emplace_back( decayVtx ? decayVtx->phi()  : AlgConsts::invalidFloat );
      b_descendent_pid             .emplace_back( truth->pdgId()  );
      b_descendent_pt              .emplace_back( truth->pt()     );
      b_descendent_eta             .emplace_back( truth->eta()    );
      b_descendent_phi             .emplace_back( truth->phi()    );
      b_descendent_mass            .emplace_back( truth->m()      );
      b_descendent_charge          .emplace_back( truth->charge() );
      
      bool isReconstructed = m_impl->truthTrackHash.find(truth) != m_impl->truthTrackHash.end();
      b_descendent_isReconstructed .emplace_back( isReconstructed );
      b_descendent_isAssociated    .emplace_back( false );
      
      b_descendent_d0          .emplace_back( AlgConsts::invalidFloat );
      b_descendent_z0          .emplace_back( AlgConsts::invalidFloat );
      b_descendent_refReco_r   .emplace_back( AlgConsts::invalidFloat );
      b_descendent_refReco_z   .emplace_back( AlgConsts::invalidFloat );
      b_descendent_refReco_phi .emplace_back( AlgConsts::invalidFloat );
      b_descendent_refReco_dist.emplace_back( AlgConsts::invalidFloat );
      b_descendent_passHitPattern.emplace_back( -1 );
          
        
      if( isReconstructed ) {
        
        reconstructedParticles.emplace( truth );
        
        const auto* trk = m_impl->truthTrackHash.at( truth );
        
        auto max_dist = AlgConsts::maxValue;
        
        // Select the closest vertex wrt the production point
        for( size_t index = 0; index < vsiVertices->size(); index++) {
        
          const auto* recoVtx = vsiVertices->at( index );
          
          const auto& region = VsiTool::vertexRegion( recoVtx );
          bool isPass = VsiTool::passLoosePattern( trk->hitPattern(), region, TVector3(1,0,0) );
          
          b_descendent_passHitPattern.back() = isPass;
          

        
          TVector3 pos( recoVtx->x(), recoVtx->y(), recoVtx->z() );
          
          auto dist = (pos - prodVtx->v4().Vect() ).Mag2();
          
          for( int itrk=0; itrk < recoVtx->nTrackParticles(); itrk++ ) {
            if( trk == recoVtx->trackParticle(itrk) ) {
              b_descendent_isAssociated.back() = true;
              break;
            }
          }
          
          if( dist < max_dist ) {
            b_descendent_d0          .back() = trk->d0();
            b_descendent_z0          .back() = trk->z0();
            
            b_descendent_refReco_r   .back() = pos.Perp();
            b_descendent_refReco_z   .back() = pos.z();
            b_descendent_refReco_phi .back() = pos.Phi();
            b_descendent_refReco_dist.back() = dist;
            
            max_dist = dist;
          }
          
        }
        
      }
      
      bool isSelected { false };
      if( isReconstructed ) {
        const auto* trk = m_impl->truthTrackHash.at( truth );
        if( trk->isAvailable<char>("is_selected") ) {
          if( trk->auxdataConst<char>("is_selected") ) {
            isSelected = true;
          }
        }
      }
      
      b_descendent_isSelected      .emplace_back( isSelected );
      
      if( isSelected ) {
        
        reconstructedParticles_selected.emplace( truth );
        
      }
      
    }
    
    
    b_descendent_reco_size     = reconstructedParticles.size();
    b_descendent_selected_size = reconstructedParticles_selected.size();
    
    auto visibleSumP4 = std::accumulate( reconstructedParticles.begin(), reconstructedParticles.end(),
                                         TLorentzVector{}, []( TLorentzVector p4, const xAOD::TruthParticle* p ) { p4 += p->p4(); return p4; } );
    
    b_descendent_reco_sum_pt   = visibleSumP4.Pt();
    b_descendent_reco_sum_eta  = visibleSumP4.Eta();
    b_descendent_reco_sum_phi  = visibleSumP4.Phi();
    b_descendent_reco_sum_mass = visibleSumP4.M();
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    fillHist<TH1F>    ( "hDecayPosR_truthOK", m_impl->trueRadius );
    fillHist<TH1F>    ( "hDecayPosR_truthChargedMultiplicity", m_impl->trueRadius, nReconstructibleParticles );
    fillHist<TProfile>( "pDecayPosR_truthChargedMultiplicity", m_impl->trueRadius, nReconstructibleParticles );
    
    if( nReconstructibleParticles < 2 ) continue;
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Now this Signal vertex is potentially reconstructible.
    //
    
    fillHist<TH1F>( "hDecayPosR_reconstructible", m_impl->trueRadius );
    fillHist<TH1F>( "hmult_reconstructible", nReconstructibleParticles );
    fillHist<TH2F>( "hmultR_reconstructible", nReconstructibleParticles, m_impl->trueRadius );
    
    
    const auto nReconstructedTracks_total    = reconstructedParticles.size();
    const auto nReconstructedTracks_selected = reconstructedParticles_selected.size();
    
    // DEBUGW( "  nReconstructed (total) = " << nReconstructedTracks_total << ", mass = " << visibleSumP4.M()*1.e-3 << " GeV" );
    // DEBUGW( "  nReconstructed (selected) = " << nReconstructedTracks_selected );
    
    if( nReconstructedTracks_selected < 2 ) continue;
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Now this Signal vertex has at least 2 selected tracks and the vertex is possible to be reconstructed
    //
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // search the corresponding reco vertex
    //
    std::map<const xAOD::Vertex*, TVector3> recoVertices;
    for( size_t index = 0; index < vsiVertices->size(); index++) {
      
      const auto* recoVtx = vsiVertices->at( index );
      
      // check to which truth vertex position is populated
      std::map<size_t, double> votes;
      
      for( size_t index = 0; index < truthPhysicalPositions.size(); ++index ) {
        
        auto& cvtx = truthPhysicalPositions.at(index);
        
        for( size_t itrk = 0; itrk < recoVtx->nTrackParticles(); ++itrk ) {
          const auto* trk  = recoVtx->trackParticle(itrk);
          if( cvtx.find( trk ) ) {
            votes[index] += trk->pt();
          }
        }
      }
      
      double scalarSumPt { 0.0 };
      
      for( size_t itrk = 0; itrk < recoVtx->nTrackParticles(); ++itrk ) {
        const auto* trk = recoVtx->trackParticle(itrk);
        if( !trk ) continue;
        scalarSumPt += trk->pt();
      }
      
      
      TVector3 pos( recoVtx->x(), recoVtx->y(), recoVtx->z() );
      
      if( 0 == votes.size() ) continue;
      
      for( auto& vote : votes ) {
        const auto& pos = truthPhysicalPositions.at(vote.first).position();
        // DEBUGW( "  ==> vote truth (r,z) = (" << pos.Perp() << ", " << pos.z() << "), sumPt = " << vote.second*1.e-3 << " GeV" );
      }
      
      auto candidate = std::max_element( votes.begin(), votes.end(),
                                         [&]( auto& v1, auto& v2) { return v1.second < v2.second; } );
      
      const auto& cand_pos = truthPhysicalPositions.at( (*candidate).first ).position();
      auto residual = pos - cand_pos;
      
      // DEBUGW( "  candidate truth (r,z) = (" << cand_pos.Perp() << ", " << cand_pos.z() << ", " << cand_pos.Phi() << ")" );
      
      double matchScore = (*candidate).second / scalarSumPt;

      if( matchScore>=0.5 /*residual.Mag() < m_impl->truthMatchResidualLimit*/ ) {
        
        // DEBUGW( "reco (r,z,phi) = (" << recoVtx->position().perp() << ", " << recoVtx->z() << ", " << recoVtx->position().phi() << "), residual = " << residual.Mag() << ", "
        //         "mass = " << recoVtx->auxdataConst<float>("mass")*1.e-3 << " GeV" );

        ///////////////////////////////////////////////////////////////////////////////////////
        // Properties after trimming
        
        // Filter tracks based on VsiBonsai configuration
        std::vector<const xAOD::TrackParticle*> allTracks;
        std::vector<const xAOD::TrackParticle*> filteredTracks;
        std::vector<const xAOD::TrackParticle*> filteredTracks_nonAssociated;
        std::vector<const xAOD::TrackParticle*> droppedTracks;
        std::vector<const xAOD::TrackParticle*> nonAssociatedTracks;
        
        for( size_t itrk=0; itrk<recoVtx->nTrackParticles(); itrk++) {
          
          const auto* trk = recoVtx->trackParticle( itrk );
          
          allTracks.emplace_back( trk );
          
          bool flag { true };
          
          for( auto trimmer : trimmers ) {
            
            if( !trimmer( recoVtx, trk, s_thePV, m_impl->bonsaiCfg ) ) { flag = false; break; }
            
          }
          
          flag ? filteredTracks.emplace_back( trk ) : droppedTracks.emplace_back( trk );
          
          // Non-associated tracks
          if( VsiBonsai::dropAssociated( recoVtx, trk, s_thePV, m_impl->bonsaiCfg ) ) {
            
            nonAssociatedTracks.emplace_back( trk );
            
            if( flag ) filteredTracks_nonAssociated.emplace_back( trk );
            
          }
          
          
        }
        

#define EMPLACE_BACK( name, val ) r_##name.emplace_back( val );
        
        EMPLACE_BACK( recoVtx_index                       , index  );
        EMPLACE_BACK( recoVtx_LLP_index                   , llpIndex  );
        EMPLACE_BACK( recoVtx_truthPos_index              , (*candidate).first  );
        EMPLACE_BACK( recoVtx_matchScore                  , matchScore );
        EMPLACE_BACK( recoVtx_r                           , recoVtx->position().perp() );
        EMPLACE_BACK( recoVtx_x                           , recoVtx->x() );
        EMPLACE_BACK( recoVtx_y                           , recoVtx->y() );
        EMPLACE_BACK( recoVtx_z                           , recoVtx->z() );
        EMPLACE_BACK( recoVtx_phi                         , recoVtx->position().phi() );
        EMPLACE_BACK( recoVtx_residual_r                  , residual.Perp() );
        EMPLACE_BACK( recoVtx_residual_x                  , residual.x() );
        EMPLACE_BACK( recoVtx_residual_y                  , residual.y() );
        EMPLACE_BACK( recoVtx_residual_z                  , residual.z() );
        EMPLACE_BACK( recoVtx_residual_phi                , residual.Phi() );
        EMPLACE_BACK( recoVtx_chi2                        , recoVtx->chiSquared() / recoVtx->numberDoF() );
        EMPLACE_BACK( recoVtx_chi2_core                   , recoVtx->auxdataConst<float>("chi2_core") / recoVtx->auxdataConst<float>("ndof_core") );
        EMPLACE_BACK( recoVtx_mass                        , VsiBonsai::sumP4( allTracks ).M() );
        EMPLACE_BACK( recoVtx_ntrk                        , allTracks.size() );
        EMPLACE_BACK( recoVtx_massFraction                , VsiBonsai::sumP4( allTracks ).M() / visibleSumP4.M() );
        EMPLACE_BACK( recoVtx_multiplicityFraction        , static_cast<float>( allTracks.size() ) / static_cast<float>( reconstructedParticles.size() ) );
        EMPLACE_BACK( recoVtx_mass_nonAssociated          , VsiBonsai::sumP4( nonAssociatedTracks ).M() );
        EMPLACE_BACK( recoVtx_ntrk_nonAssociated          , nonAssociatedTracks.size()  );
        EMPLACE_BACK( recoVtx_filtered_mass               , VsiBonsai::sumP4( filteredTracks ).M() );
        EMPLACE_BACK( recoVtx_filtered_mass_nonAssociated , VsiBonsai::sumP4( filteredTracks_nonAssociated ).M() );
        EMPLACE_BACK( recoVtx_filtered_ntrk               , filteredTracks.size()  );
        EMPLACE_BACK( recoVtx_filtered_ntrk_nonAssociated , filteredTracks_nonAssociated.size()  );
        
#undef EMPLACE_BACK
        
        
        recoVertices[recoVtx] = residual;
        nvtx_reco++;
      }
    }
    
    // DEBUGW( "nMatchedRecoVtx = " << recoVertices.size() );
    
    if( 0 == recoVertices.size() ) continue;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Now the corresponding reco vertices were found.
    //
    
    fillHist<TH1F>( "hDecayPosR_reconstructed", m_impl->trueRadius );
    fillHist<TH1F>( "hmult_reconstructed"     , nReconstructedTracks_selected );
    fillHist<TH2F>( "hmultR_reconstructed"    , nReconstructedTracks_selected, m_impl->trueRadius );
    
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    // Loop over the matched reco vertices and fill their properties
    //
    
    for( auto& pair: recoVertices ) {
      const auto* recoVtx = pair.first;
      
      double multiplicityFraction = static_cast<double>(recoVtx->nTrackParticles()) / static_cast<double>(nReconstructedTracks_total);
      
      std::vector<const xAOD::TrackParticle*> trks;
      std::vector<const xAOD::TrackParticle*> trks_nonAssociated;
      
      for( size_t i=0; i<recoVtx->nTrackParticles(); i++) {
        auto* trk = recoVtx->trackParticle(i);
        if( trk->pt() < m_impl->pTthr ) continue;
        trks.emplace_back( trk );
        
        fillHist<TH2F>( "hDecayPosR_d0_reconstructed", m_impl->trueRadius, fabs(trk->d0()) );
        
        if( !trk->isAvailable<char>("is_associated") ) {
          trks_nonAssociated.emplace_back( trk );
          continue;
        }
        
        if( !trk->auxdataConst<char>("is_associated") ) {
          trks_nonAssociated.emplace_back( trk );
        }
      }
      
      const auto visibleSumP4Reco = VsiBonsai::sumP4( trks );
      
      fillHist<TH2F>( "hVisibleMassMap", visibleSumP4.M()*1.e-3, visibleSumP4Reco.M()*1.e-3 );
      
      double visibleMassFraction = visibleSumP4Reco.M() / visibleSumP4.M();
      
      fillHist<TH2F>( "hDecayPosR_multiplicityFraction", m_impl->trueRadius, multiplicityFraction );
      fillHist<TH2F>( "hDecayPosR_visibleMassFraction",  m_impl->trueRadius, visibleMassFraction  );
      
      fillHist<TH1F>( "hDecayPosR_trackMultiplicity",                m_impl->trueRadius, nReconstructedTracks_total );
      fillHist<TH1F>( "hDecayPosR_trackMultiplicity_selected",       m_impl->trueRadius, trks_nonAssociated.size() );
      fillHist<TH1F>( "hDecayPosR_trackMultiplicity_associated",     m_impl->trueRadius, trks.size() );
      
      fillHist<TProfile>( "pDecayPosR_trackMultiplicity",            m_impl->trueRadius, nReconstructedTracks_total );
      fillHist<TProfile>( "pDecayPosR_trackMultiplicity_selected",   m_impl->trueRadius, trks_nonAssociated.size() );
      fillHist<TProfile>( "pDecayPosR_trackMultiplicity_associated", m_impl->trueRadius, trks.size() );
    
    }
    
    if( recoVertices.size() > 0 ) {
      fillHist<TH2F>( "hDecayPosR_splitted", m_impl->trueRadius, recoVertices.size() );
    }
    
    if( recoVertices.size() == 1 ) {
      fillHist<TH1F>( "hDecayPosR_reconstructedUnique", m_impl->trueRadius );
    }
      
  }
  
  fillHist<TH1F>( "hNvtx_reco", nvtx_reco );
  
  // DEBUG();
  
  return EL::StatusCode::SUCCESS;
}




//____________________________________________________________________________________________________
void VsiEfficiencyProcessor::Impl::fillTruthProperties( const xAOD::TruthVertex* signalTruthVertex ) {
  
  auto getDphi = []( const double& phi1, const double& phi2 ) {
    auto dphi = phi1 - phi2;
    while( dphi > TMath::Pi() ) dphi -= TMath::TwoPi();
    while( dphi < -TMath::Pi() ) dphi += TMath::TwoPi();
    return dphi;
  };
  
#pragma message( "ID range hard-coded" )
  auto isOutside = []( TVector3& v ) { return ( v.Perp() > 563. || fabs( v.z() ) > 2720. ); };
  
  
  for( size_t ipart = 0; ipart <  signalTruthVertex->nOutgoingParticles(); ipart ++ ) {
    const auto* particle = signalTruthVertex->outgoingParticle( ipart );
    if( !particle ) continue;
    if( particle->pt() < pTthr ) continue;
    
    // Check if this particle has decayPos.
    // if the decayPos is outside the SCT volume and the particle is charged, fill properties
    // if the decayPos is not present and the particle is charged, fill properties
    
    bool flag { false };
    
    if( particle->isCharged() ) {
      if( particle->hasDecayVtx() ) {
      
        TVector3 decayPos( particle->decayVtx()->x(), particle->decayVtx()->y(), particle->decayVtx()->z() );
        
        if( isOutside( decayPos ) ) flag = true;
      } else {
        flag = true;
      }
    }
    
    
    if( flag ) {
      const auto& dphi = getDphi( particle->p4().Phi(), signalTruthVertex->v4().Phi() );
    
      parent->fillHist<TH2F>( "hPartDphiR", dphi, signalTruthVertex->perp() );
      if( particle->charge() > 0 ) {
        parent->fillHist<TH2F>( "hPartDphiRPositive", dphi, signalTruthVertex->perp() );
      }
      if( particle->charge() < 0 ) {
        parent->fillHist<TH2F>( "hPartDphiRNegative", dphi, signalTruthVertex->perp() );
      }
    
      if( truthTrackHash.find( particle ) == truthTrackHash.end() ) continue;
      const auto* trk = truthTrackHash.at( particle );
      if( !trk ) continue;
    
      parent->fillHist<TH2F>( "hPartDphiR_reco", dphi, signalTruthVertex->perp() );
      if( particle->charge() > 0 ) {
        parent->fillHist<TH2F>( "hPartDphiRPositive_reco", dphi, signalTruthVertex->perp() );
      }
      if( particle->charge() < 0 ) {
        parent->fillHist<TH2F>( "hPartDphiRNegative_reco", dphi, signalTruthVertex->perp() );
      }
    }
    
    if( particle->hasDecayVtx() ) {
      
      TVector3 decayPos( particle->decayVtx()->x(), particle->decayVtx()->y(), particle->decayVtx()->z() );
      TVector3 prodPos ( particle->prodVtx()->x(),  particle->prodVtx()->y(),  particle->prodVtx()->z()  );
      
      const auto distance = (decayPos - prodPos).Mag();
      
      if( isOutside( decayPos ) )     continue;
      if( distance > distanceCutoff ) continue;
        
      if( doRecursive ) fillTruthProperties( particle->decayVtx() );
    }
    
  }
  
}


