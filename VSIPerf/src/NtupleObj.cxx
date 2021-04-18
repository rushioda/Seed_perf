#include <include/NtupleObj.h>

ClassImp(EventInfoObj)
ClassImp(TruthVtxObj)
ClassImp(TrackObj)
ClassImp(PVObj)
ClassImp(DVObj)
ClassImp(DVeffObj)

TruthVtxObj::TruthVtxObj()
: outP_eta        ( nullptr )
, outP_isReco     ( nullptr )
, outP_phi        ( nullptr )
, outP_pid        ( nullptr )
, outP_pt         ( nullptr )
, parent_eta      ( nullptr )
, parent_phi      ( nullptr )
, parent_pid      ( nullptr )
, parent_prod_phi ( nullptr )
, parent_prod_r   ( nullptr )
, parent_prod_x   ( nullptr )
, parent_prod_y   ( nullptr )
, parent_prod_z   ( nullptr )
, parent_pt       ( nullptr )
, phi             ( nullptr )
, r               ( nullptr )
, z               ( nullptr )
{}


TrackObj::TrackObj()
    : chi2(nullptr), d0(nullptr), z0(nullptr),
      pt(nullptr), eta(nullptr), phi(nullptr),
      errd0(nullptr), errz0(nullptr),
      selected(nullptr)
{}


DVObj::DVObj()
: x                                    ( nullptr )
, y                                    ( nullptr )
, z                                    ( nullptr )
, r                                    ( nullptr )
, phi                                  ( nullptr )
, direction                            ( nullptr )
, chi2                                 ( nullptr )
, covariance                           ( nullptr )
, pt                                   ( nullptr )
, H                                    ( nullptr )
, HT                                   ( nullptr )
, ntrk                                 ( nullptr )
, ntrk_associated                      ( nullptr )
, ntrk_selected                        ( nullptr )
, mass                                 ( nullptr )
, mass_nonAssociated                   ( nullptr )
, charge                               ( nullptr )
, maxOpAng                             ( nullptr )
, minOneTrackRemovedMass               ( nullptr )
, minOpAng                             ( nullptr )
, mind0                                ( nullptr )
, maxd0                                ( nullptr )
, trk_charge                           ( nullptr )
, trk_truthPid                         ( nullptr )
, trk_chi2_toSV                        ( nullptr )
, trk_d0                               ( nullptr )
, trk_d0_wrtSV                         ( nullptr )
, trk_dEdx                             ( nullptr )
, trk_errd0_wrtSV                      ( nullptr )
, trk_errz0_wrtSV                      ( nullptr )
, trk_eta                              ( nullptr )
, trk_eta_wrtSV                        ( nullptr )
, trk_phi                              ( nullptr )
, trk_phi_wrtSV                        ( nullptr )
, trk_pt                               ( nullptr )
, trk_pt_wrtSV                         ( nullptr )
, trk_qOverP                           ( nullptr )
, trk_z0                               ( nullptr )
, trk_z0_wrtSV                         ( nullptr )
, trk_isAssociated                     ( nullptr )
, trk_isSelected                       ( nullptr )
, trk_nIBLOverflowsdEdx                ( nullptr )
, trk_nPixelBarrelLayers               ( nullptr )
, trk_nPixelEndCapLayers               ( nullptr )
, trk_nPixelHits                       ( nullptr )
, trk_nSCTHits                         ( nullptr )
, trk_nSctBarrelLayers                 ( nullptr )
, trk_nSctEndCapLayers                 ( nullptr )
, trk_nTRTHits                         ( nullptr )
, trk_nUsedHitsdEdx                    ( nullptr )
, trk_truthIsPointingToClosestTruthVtx ( nullptr )
, trk_truthIsPointingToReprTruthVtx    ( nullptr )
, trk_final                            ( nullptr )
, trk_hitPattern                       ( nullptr )
, twoTracksCharge                      ( nullptr )
, twoTracksMass                        ( nullptr )
, twoTracksMassRest                    ( nullptr )
, closestTruth_distance                ( nullptr )
, closestTruth_inE                     ( nullptr )
, closestTruth_outE                    ( nullptr )
, closestTruth_parent_eta              ( nullptr )
, closestTruth_parent_phi              ( nullptr )
, closestTruth_parent_pt               ( nullptr )
, closestTruth_phi                     ( nullptr )
, closestTruth_r                       ( nullptr )
, closestTruth_x                       ( nullptr )
, closestTruth_y                       ( nullptr )
, closestTruth_z                       ( nullptr )
, closestTruth_parent                  ( nullptr )
, closestTruth_outP_eta                ( nullptr )
, closestTruth_outP_phi                ( nullptr )
, closestTruth_outP_pt                 ( nullptr )
, closestTruth_outP_charge             ( nullptr )
, closestTruth_outP_pid                ( nullptr )
, closestTruth_outP_isReco             ( nullptr )
, reprTruth_inE                        ( nullptr )
, reprTruth_matchScore                 ( nullptr )
, reprTruth_outE                       ( nullptr )
, reprTruth_parent_eta                 ( nullptr )
, reprTruth_parent_phi                 ( nullptr )
, reprTruth_parent_pt                  ( nullptr )
, reprTruth_phi                        ( nullptr )
, reprTruth_r                          ( nullptr )
, reprTruth_x                          ( nullptr )
, reprTruth_y                          ( nullptr )
, reprTruth_z                          ( nullptr )
, reprTruth_parent                     ( nullptr )
, reprTruth_outP_eta                   ( nullptr )
, reprTruth_outP_phi                   ( nullptr )
, reprTruth_outP_pt                    ( nullptr )
, reprTruth_outP_charge                ( nullptr )
, reprTruth_outP_pid                   ( nullptr )
, reprTruth_outP_isReco                ( nullptr )
{}



DVeffObj::DVeffObj()
: LLP_size                            ( 0 )
, LLP_decay_phi                       ( nullptr )
, LLP_decay_r                         ( nullptr )
, LLP_decay_x                         ( nullptr )
, LLP_decay_y                         ( nullptr )
, LLP_decay_z                         ( nullptr )
, LLP_eta                             ( nullptr )
, LLP_mass                            ( nullptr )
, LLP_childOpenAngle                  ( nullptr )
, LLP_phi                             ( nullptr )
, LLP_pid                             ( nullptr )
, LLP_pt                              ( nullptr )
, descendent_charge                   ( nullptr )
, descendent_pid                      ( nullptr )
, descendent_mass                     ( nullptr )
, descendent_pt                       ( nullptr )
, descendent_eta                      ( nullptr )
, descendent_phi                      ( nullptr )
, descendent_prod_r                   ( nullptr )
, descendent_prod_x                   ( nullptr )
, descendent_prod_y                   ( nullptr )
, descendent_prod_z                   ( nullptr )
, descendent_prod_phi                 ( nullptr )
, descendent_decay_r                  ( nullptr )
, descendent_decay_x                  ( nullptr )
, descendent_decay_y                  ( nullptr )
, descendent_decay_z                  ( nullptr )
, descendent_decay_phi                ( nullptr )
, descendent_reco_size                ( nullptr )
, descendent_reco_sum_eta             ( nullptr )
, descendent_reco_sum_mass            ( nullptr )
, descendent_reco_sum_phi             ( nullptr )
, descendent_reco_sum_pt              ( nullptr )
, descendent_isReconstructed          ( nullptr )
, descendent_isSelected               ( nullptr )
, descendent_truthPosIndex            ( nullptr )
, descendent_selected_size            ( nullptr )
, descendent_size                     ( nullptr )
, descendent_sum_pt                   ( nullptr )
, descendent_sum_eta                  ( nullptr )
, descendent_sum_phi                  ( nullptr )
, descendent_sum_mass                 ( nullptr )
, truthPos_size                       ( nullptr )
, truthPos_r                          ( nullptr )
, truthPos_x                          ( nullptr )
, truthPos_y                          ( nullptr )
, truthPos_z                          ( nullptr )
, truthPos_phi                        ( nullptr )
, truthPos_nReconstructible           ( nullptr )
, truthPos_reconstructibleMass        ( nullptr )
, truthPos_sum_pt                     ( nullptr )
, truthPos_sum_eta                    ( nullptr )
, truthPos_sum_phi                    ( nullptr )
, recoVtx_LLP_index                   ( nullptr )
, recoVtx_truthPos_index              ( nullptr )
, recoVtx_index                       ( nullptr )
, recoVtx_chi2                        ( nullptr )
, recoVtx_chi2_core                   ( nullptr )
, recoVtx_mass                        ( nullptr )
, recoVtx_massFraction                ( nullptr )
, recoVtx_mass_nonAssociated          ( nullptr )
, recoVtx_matchScore                  ( nullptr )
, recoVtx_multiplicityFraction        ( nullptr )
, recoVtx_ntrk                        ( nullptr )
, recoVtx_ntrk_nonAssociated          ( nullptr )
, recoVtx_filtered_mass               ( nullptr )
, recoVtx_filtered_mass_nonAssociated ( nullptr )
, recoVtx_filtered_ntrk               ( nullptr )
, recoVtx_filtered_ntrk_nonAssociated ( nullptr )
, recoVtx_r                           ( nullptr )
, recoVtx_x                           ( nullptr )
, recoVtx_y                           ( nullptr )
, recoVtx_z                           ( nullptr )
, recoVtx_phi                         ( nullptr )
, recoVtx_residual_r                  ( nullptr )
, recoVtx_residual_x                  ( nullptr )
, recoVtx_residual_y                  ( nullptr )
, recoVtx_residual_z                  ( nullptr )
, recoVtx_residual_phi                ( nullptr )
{}


void setBranchAddress( TTree *tree, EventInfoObj& ei ) {
   tree->SetBranchAddress("EventInfo.actualMu",&ei.actualMu);
   tree->SetBranchAddress("EventInfo.averageMu",&ei.averageMu);
   tree->SetBranchAddress("EventInfo.eventNumber",&ei.eventNumber);
   tree->SetBranchAddress("EventInfo.lumiBlock",&ei.lumiBlock);
   tree->SetBranchAddress("EventInfo.runNumber",&ei.runNumber);
   tree->SetBranchAddress("EventInfo.mcChannelNumber",&ei.mcChannelNumber);
   tree->SetBranchAddress("EventInfo.mcWeight",&ei.mcWeight);
}

void setBranchAddress( TTree* tree, TruthVtxObj& truth, const std::string name ) {
    tree->SetBranchAddress( (name + ".vtx_outP_eta" )        .c_str(), &truth.outP_eta        );
    tree->SetBranchAddress( (name + ".vtx_outP_isReco" )     .c_str(), &truth.outP_isReco     );
    tree->SetBranchAddress( (name + ".vtx_outP_phi" )        .c_str(), &truth.outP_phi        );
    tree->SetBranchAddress( (name + ".vtx_outP_pid" )        .c_str(), &truth.outP_pid        );
    tree->SetBranchAddress( (name + ".vtx_outP_pt" )         .c_str(), &truth.outP_pt         );
    tree->SetBranchAddress( (name + ".vtx_parent_eta" )      .c_str(), &truth.parent_eta      );
    tree->SetBranchAddress( (name + ".vtx_parent_phi" )      .c_str(), &truth.parent_phi      );
    tree->SetBranchAddress( (name + ".vtx_parent_pid" )      .c_str(), &truth.parent_pid      );
    tree->SetBranchAddress( (name + ".vtx_parent_prod_phi" ) .c_str(), &truth.parent_prod_phi );
    tree->SetBranchAddress( (name + ".vtx_parent_prod_r" )   .c_str(), &truth.parent_prod_r   );
    tree->SetBranchAddress( (name + ".vtx_parent_prod_x" )   .c_str(), &truth.parent_prod_x   );
    tree->SetBranchAddress( (name + ".vtx_parent_prod_y" )   .c_str(), &truth.parent_prod_y   );
    tree->SetBranchAddress( (name + ".vtx_parent_prod_z" )   .c_str(), &truth.parent_prod_z   );
    tree->SetBranchAddress( (name + ".vtx_parent_pt" )       .c_str(), &truth.parent_pt       );
    tree->SetBranchAddress( (name + ".vtx_phi" )             .c_str(), &truth.phi             );
    tree->SetBranchAddress( (name + ".vtx_r" )               .c_str(), &truth.r               );
    tree->SetBranchAddress( (name + ".vtx_z" )               .c_str(), &truth.z               );
}


void setBranchAddress( TTree* tree, TrackObj& trk, const std::string name ) {
    tree->SetBranchAddress( (name + ".chi2"         ).c_str(), &trk.chi2         );
    tree->SetBranchAddress( (name + ".d0"           ).c_str(), &trk.d0           );
    tree->SetBranchAddress( (name + ".errd0"        ).c_str(), &trk.errd0        );
    tree->SetBranchAddress( (name + ".errz0"        ).c_str(), &trk.errz0        );
    tree->SetBranchAddress( (name + ".eta"          ).c_str(), &trk.eta          );
    tree->SetBranchAddress( (name + ".phi"          ).c_str(), &trk.phi          );
    tree->SetBranchAddress( (name + ".pt"           ).c_str(), &trk.pt           );
    tree->SetBranchAddress( (name + ".sampledSize"  ).c_str(), &trk.sampledSize  );
    tree->SetBranchAddress( (name + ".selected"     ).c_str(), &trk.selected     );
    tree->SetBranchAddress( (name + ".selectedSize" ).c_str(), &trk.selectedSize );
    tree->SetBranchAddress( (name + ".totalSize"    ).c_str(), &trk.totalSize    );
    tree->SetBranchAddress( (name + ".z0"           ).c_str(), &trk.z0           );
}


void setBranchAddress( TTree* tree, PVObj& pv ) {
   tree->SetBranchAddress("PV.nTracksPVassoc",&pv.nTracksPVassoc);
   tree->SetBranchAddress("PV.phi",&pv.phi);
   tree->SetBranchAddress("PV.r",&pv.r);
   tree->SetBranchAddress("PV.x",&pv.x);
   tree->SetBranchAddress("PV.y",&pv.y);
   tree->SetBranchAddress("PV.z",&pv.z);
}


void setBranchAddress( TTree* tree, DVObj& dv ) {
    
   tree->SetBranchAddress("DV.H",                                    &dv.H);
   tree->SetBranchAddress("DV.HT",                                   &dv.HT);
   tree->SetBranchAddress("DV.charge",                               &dv.charge);
   tree->SetBranchAddress("DV.chi2",                                 &dv.chi2);
   tree->SetBranchAddress("DV.closestTruth_distance",                &dv.closestTruth_distance);
   tree->SetBranchAddress("DV.closestTruth_inE",                     &dv.closestTruth_inE);
   tree->SetBranchAddress("DV.closestTruth_outE",                    &dv.closestTruth_outE);
   tree->SetBranchAddress("DV.closestTruth_outP_charge",             &dv.closestTruth_outP_charge);
   tree->SetBranchAddress("DV.closestTruth_outP_eta",                &dv.closestTruth_outP_eta);
   tree->SetBranchAddress("DV.closestTruth_outP_isReco",             &dv.closestTruth_outP_isReco);
   tree->SetBranchAddress("DV.closestTruth_outP_phi",                &dv.closestTruth_outP_phi);
   tree->SetBranchAddress("DV.closestTruth_outP_pid",                &dv.closestTruth_outP_pid);
   tree->SetBranchAddress("DV.closestTruth_outP_pt",                 &dv.closestTruth_outP_pt);
   tree->SetBranchAddress("DV.closestTruth_parent",                  &dv.closestTruth_parent);
   tree->SetBranchAddress("DV.closestTruth_parent_eta",              &dv.closestTruth_parent_eta);
   tree->SetBranchAddress("DV.closestTruth_parent_phi",              &dv.closestTruth_parent_phi);
   tree->SetBranchAddress("DV.closestTruth_parent_pt",               &dv.closestTruth_parent_pt);
   tree->SetBranchAddress("DV.closestTruth_phi",                     &dv.closestTruth_phi);
   tree->SetBranchAddress("DV.closestTruth_r",                       &dv.closestTruth_r);
   tree->SetBranchAddress("DV.closestTruth_x",                       &dv.closestTruth_x);
   tree->SetBranchAddress("DV.closestTruth_y",                       &dv.closestTruth_y);
   tree->SetBranchAddress("DV.closestTruth_z",                       &dv.closestTruth_z);
   tree->SetBranchAddress("DV.covariance",                           &dv.covariance);
   tree->SetBranchAddress("DV.direction",                            &dv.direction);
   tree->SetBranchAddress("DV.mass",                                 &dv.mass);
   tree->SetBranchAddress("DV.mass_nonAssociated",                   &dv.mass_nonAssociated);
   tree->SetBranchAddress("DV.maxOpAng",                             &dv.maxOpAng);
   tree->SetBranchAddress("DV.maxd0",                                &dv.maxd0);
   tree->SetBranchAddress("DV.minOneTrackRemovedMass",               &dv.minOneTrackRemovedMass);
   tree->SetBranchAddress("DV.minOpAng",                             &dv.minOpAng);
   tree->SetBranchAddress("DV.mind0",                                &dv.mind0);
   tree->SetBranchAddress("DV.ntrk",                                 &dv.ntrk);
   tree->SetBranchAddress("DV.ntrk_associated",                      &dv.ntrk_associated);
   tree->SetBranchAddress("DV.ntrk_selected",                        &dv.ntrk_selected);
   tree->SetBranchAddress("DV.phi",                                  &dv.phi);
   tree->SetBranchAddress("DV.pt",                                   &dv.pt);
   tree->SetBranchAddress("DV.r",                                    &dv.r);
   tree->SetBranchAddress("DV.reprTruth_inE",                        &dv.reprTruth_inE);
   tree->SetBranchAddress("DV.reprTruth_matchScore",                 &dv.reprTruth_matchScore);
   tree->SetBranchAddress("DV.reprTruth_outE",                       &dv.reprTruth_outE);
   tree->SetBranchAddress("DV.reprTruth_outP_charge",                &dv.reprTruth_outP_charge);
   tree->SetBranchAddress("DV.reprTruth_outP_eta",                   &dv.reprTruth_outP_eta);
   tree->SetBranchAddress("DV.reprTruth_outP_isReco",                &dv.reprTruth_outP_isReco);
   tree->SetBranchAddress("DV.reprTruth_outP_phi",                   &dv.reprTruth_outP_phi);
   tree->SetBranchAddress("DV.reprTruth_outP_pid",                   &dv.reprTruth_outP_pid);
   tree->SetBranchAddress("DV.reprTruth_outP_pt",                    &dv.reprTruth_outP_pt);
   tree->SetBranchAddress("DV.reprTruth_parent",                     &dv.reprTruth_parent);
   tree->SetBranchAddress("DV.reprTruth_parent_eta",                 &dv.reprTruth_parent_eta);
   tree->SetBranchAddress("DV.reprTruth_parent_phi",                 &dv.reprTruth_parent_phi);
   tree->SetBranchAddress("DV.reprTruth_parent_pt",                  &dv.reprTruth_parent_pt);
   tree->SetBranchAddress("DV.reprTruth_phi",                        &dv.reprTruth_phi);
   tree->SetBranchAddress("DV.reprTruth_r",                          &dv.reprTruth_r);
   tree->SetBranchAddress("DV.reprTruth_x",                          &dv.reprTruth_x);
   tree->SetBranchAddress("DV.reprTruth_y",                          &dv.reprTruth_y);
   tree->SetBranchAddress("DV.reprTruth_z",                          &dv.reprTruth_z);
   tree->SetBranchAddress("DV.trk_charge",                           &dv.trk_charge);
   tree->SetBranchAddress("DV.trk_chi2_toSV",                        &dv.trk_chi2_toSV);
   tree->SetBranchAddress("DV.trk_d0",                               &dv.trk_d0);
   tree->SetBranchAddress("DV.trk_d0_wrtSV",                         &dv.trk_d0_wrtSV);
   tree->SetBranchAddress("DV.trk_dEdx",                             &dv.trk_dEdx);
   tree->SetBranchAddress("DV.trk_errd0_wrtSV",                      &dv.trk_errd0_wrtSV);
   tree->SetBranchAddress("DV.trk_errz0_wrtSV",                      &dv.trk_errz0_wrtSV);
   tree->SetBranchAddress("DV.trk_eta",                              &dv.trk_eta);
   tree->SetBranchAddress("DV.trk_eta_wrtSV",                        &dv.trk_eta_wrtSV);
   tree->SetBranchAddress("DV.trk_final",                            &dv.trk_final);
   tree->SetBranchAddress("DV.trk_hitPattern",                       &dv.trk_hitPattern);
   tree->SetBranchAddress("DV.trk_isAssociated",                     &dv.trk_isAssociated);
   tree->SetBranchAddress("DV.trk_isSelected",                       &dv.trk_isSelected);
   tree->SetBranchAddress("DV.trk_nIBLOverflowsdEdx",                &dv.trk_nIBLOverflowsdEdx);
   tree->SetBranchAddress("DV.trk_nPixelBarrelLayers",               &dv.trk_nPixelBarrelLayers);
   tree->SetBranchAddress("DV.trk_nPixelEndCapLayers",               &dv.trk_nPixelEndCapLayers);
   tree->SetBranchAddress("DV.trk_nPixelHits",                       &dv.trk_nPixelHits);
   tree->SetBranchAddress("DV.trk_nSCTHits",                         &dv.trk_nSCTHits);
   tree->SetBranchAddress("DV.trk_nSctBarrelLayers",                 &dv.trk_nSctBarrelLayers);
   tree->SetBranchAddress("DV.trk_nSctEndCapLayers",                 &dv.trk_nSctEndCapLayers);
   tree->SetBranchAddress("DV.trk_nTRTHits",                         &dv.trk_nTRTHits);
   tree->SetBranchAddress("DV.trk_nUsedHitsdEdx",                    &dv.trk_nUsedHitsdEdx);
   tree->SetBranchAddress("DV.trk_phi",                              &dv.trk_phi);
   tree->SetBranchAddress("DV.trk_phi_wrtSV",                        &dv.trk_phi_wrtSV);
   tree->SetBranchAddress("DV.trk_pt",                               &dv.trk_pt);
   tree->SetBranchAddress("DV.trk_pt_wrtSV",                         &dv.trk_pt_wrtSV);
   tree->SetBranchAddress("DV.trk_qOverP",                           &dv.trk_qOverP);
   tree->SetBranchAddress("DV.trk_truthIsPointingToClosestTruthVtx", &dv.trk_truthIsPointingToClosestTruthVtx);
   tree->SetBranchAddress("DV.trk_truthIsPointingToReprTruthVtx",    &dv.trk_truthIsPointingToReprTruthVtx);
   tree->SetBranchAddress("DV.trk_truthPid",                         &dv.trk_truthPid);
   tree->SetBranchAddress("DV.trk_z0",                               &dv.trk_z0);
   tree->SetBranchAddress("DV.trk_z0_wrtSV",                         &dv.trk_z0_wrtSV);
   tree->SetBranchAddress("DV.twoTracksCharge",                      &dv.twoTracksCharge);
   tree->SetBranchAddress("DV.twoTracksMass",                        &dv.twoTracksMass);
   tree->SetBranchAddress("DV.twoTracksMassRest",                    &dv.twoTracksMassRest);
   tree->SetBranchAddress("DV.x",                                    &dv.x);
   tree->SetBranchAddress("DV.y",                                    &dv.y);
   tree->SetBranchAddress("DV.z",                                    &dv.z);
    
}


void setBranchAddress( TTree* tree, DVeffObj& dveff ) {
   tree->SetBranchAddress("DVeff.LLP_decay_phi",                       &dveff.LLP_decay_phi);
   tree->SetBranchAddress("DVeff.LLP_decay_r",                         &dveff.LLP_decay_r);
   tree->SetBranchAddress("DVeff.LLP_decay_x",                         &dveff.LLP_decay_x);
   tree->SetBranchAddress("DVeff.LLP_decay_y",                         &dveff.LLP_decay_y);
   tree->SetBranchAddress("DVeff.LLP_decay_z",                         &dveff.LLP_decay_z);
   tree->SetBranchAddress("DVeff.LLP_eta",                             &dveff.LLP_eta);
   tree->SetBranchAddress("DVeff.LLP_mass",                            &dveff.LLP_mass);
   tree->SetBranchAddress("DVeff.LLP_childOpenAngle",                  &dveff.LLP_childOpenAngle);
   tree->SetBranchAddress("DVeff.LLP_phi",                             &dveff.LLP_phi);
   tree->SetBranchAddress("DVeff.LLP_pid",                             &dveff.LLP_pid);
   tree->SetBranchAddress("DVeff.LLP_pt",                              &dveff.LLP_pt);
   tree->SetBranchAddress("DVeff.LLP_size",                            &dveff.LLP_size);
   tree->SetBranchAddress("DVeff.descendent_charge",                   &dveff.descendent_charge);
   tree->SetBranchAddress("DVeff.descendent_decay_phi",                &dveff.descendent_decay_phi);
   tree->SetBranchAddress("DVeff.descendent_decay_r",                  &dveff.descendent_decay_r);
   tree->SetBranchAddress("DVeff.descendent_decay_x",                  &dveff.descendent_decay_x);
   tree->SetBranchAddress("DVeff.descendent_decay_y",                  &dveff.descendent_decay_y);
   tree->SetBranchAddress("DVeff.descendent_decay_z",                  &dveff.descendent_decay_z);
   tree->SetBranchAddress("DVeff.descendent_eta",                      &dveff.descendent_eta);
   tree->SetBranchAddress("DVeff.descendent_isReconstructed",          &dveff.descendent_isReconstructed);
   tree->SetBranchAddress("DVeff.descendent_isSelected",               &dveff.descendent_isSelected);
   tree->SetBranchAddress("DVeff.descendent_mass",                     &dveff.descendent_mass);
   tree->SetBranchAddress("DVeff.descendent_phi",                      &dveff.descendent_phi);
   tree->SetBranchAddress("DVeff.descendent_pid",                      &dveff.descendent_pid);
   tree->SetBranchAddress("DVeff.descendent_prod_phi",                 &dveff.descendent_prod_phi);
   tree->SetBranchAddress("DVeff.descendent_prod_r",                   &dveff.descendent_prod_r);
   tree->SetBranchAddress("DVeff.descendent_prod_x",                   &dveff.descendent_prod_x);
   tree->SetBranchAddress("DVeff.descendent_prod_y",                   &dveff.descendent_prod_y);
   tree->SetBranchAddress("DVeff.descendent_prod_z",                   &dveff.descendent_prod_z);
   tree->SetBranchAddress("DVeff.descendent_pt",                       &dveff.descendent_pt);
   tree->SetBranchAddress("DVeff.descendent_reco_size",                &dveff.descendent_reco_size);
   tree->SetBranchAddress("DVeff.descendent_reco_sum_eta",             &dveff.descendent_reco_sum_eta);
   tree->SetBranchAddress("DVeff.descendent_reco_sum_mass",            &dveff.descendent_reco_sum_mass);
   tree->SetBranchAddress("DVeff.descendent_reco_sum_phi",             &dveff.descendent_reco_sum_phi);
   tree->SetBranchAddress("DVeff.descendent_reco_sum_pt",              &dveff.descendent_reco_sum_pt);
   tree->SetBranchAddress("DVeff.descendent_selected_size",            &dveff.descendent_selected_size);
   tree->SetBranchAddress("DVeff.descendent_size",                     &dveff.descendent_size);
   tree->SetBranchAddress("DVeff.descendent_sum_eta",                  &dveff.descendent_sum_eta);
   tree->SetBranchAddress("DVeff.descendent_sum_mass",                 &dveff.descendent_sum_mass);
   tree->SetBranchAddress("DVeff.descendent_sum_phi",                  &dveff.descendent_sum_phi);
   tree->SetBranchAddress("DVeff.descendent_sum_pt",                   &dveff.descendent_sum_pt);
   tree->SetBranchAddress("DVeff.descendent_truthPosIndex",            &dveff.descendent_truthPosIndex);
   tree->SetBranchAddress("DVeff.recoVtx_LLP_index",                   &dveff.recoVtx_LLP_index);
   tree->SetBranchAddress("DVeff.recoVtx_chi2",                        &dveff.recoVtx_chi2);
   tree->SetBranchAddress("DVeff.recoVtx_chi2_core",                   &dveff.recoVtx_chi2_core);
   tree->SetBranchAddress("DVeff.recoVtx_filtered_mass",               &dveff.recoVtx_filtered_mass);
   tree->SetBranchAddress("DVeff.recoVtx_filtered_mass_nonAssociated", &dveff.recoVtx_filtered_mass_nonAssociated);
   tree->SetBranchAddress("DVeff.recoVtx_filtered_ntrk",               &dveff.recoVtx_filtered_ntrk);
   tree->SetBranchAddress("DVeff.recoVtx_filtered_ntrk_nonAssociated", &dveff.recoVtx_filtered_ntrk_nonAssociated);
   tree->SetBranchAddress("DVeff.recoVtx_index",                       &dveff.recoVtx_index);
   tree->SetBranchAddress("DVeff.recoVtx_mass",                        &dveff.recoVtx_mass);
   tree->SetBranchAddress("DVeff.recoVtx_massFraction",                &dveff.recoVtx_massFraction);
   tree->SetBranchAddress("DVeff.recoVtx_mass_nonAssociated",          &dveff.recoVtx_mass_nonAssociated);
   tree->SetBranchAddress("DVeff.recoVtx_matchScore",                  &dveff.recoVtx_matchScore);
   tree->SetBranchAddress("DVeff.recoVtx_multiplicityFraction",        &dveff.recoVtx_multiplicityFraction);
   tree->SetBranchAddress("DVeff.recoVtx_ntrk",                        &dveff.recoVtx_ntrk);
   tree->SetBranchAddress("DVeff.recoVtx_ntrk_nonAssociated",          &dveff.recoVtx_ntrk_nonAssociated);
   tree->SetBranchAddress("DVeff.recoVtx_phi",                         &dveff.recoVtx_phi);
   tree->SetBranchAddress("DVeff.recoVtx_r",                           &dveff.recoVtx_r);
   tree->SetBranchAddress("DVeff.recoVtx_residual_phi",                &dveff.recoVtx_residual_phi);
   tree->SetBranchAddress("DVeff.recoVtx_residual_r",                  &dveff.recoVtx_residual_r);
   tree->SetBranchAddress("DVeff.recoVtx_residual_x",                  &dveff.recoVtx_residual_x);
   tree->SetBranchAddress("DVeff.recoVtx_residual_y",                  &dveff.recoVtx_residual_y);
   tree->SetBranchAddress("DVeff.recoVtx_residual_z",                  &dveff.recoVtx_residual_z);
   tree->SetBranchAddress("DVeff.recoVtx_truthPos_index",              &dveff.recoVtx_truthPos_index);
   tree->SetBranchAddress("DVeff.recoVtx_x",                           &dveff.recoVtx_x);
   tree->SetBranchAddress("DVeff.recoVtx_y",                           &dveff.recoVtx_y);
   tree->SetBranchAddress("DVeff.recoVtx_z",                           &dveff.recoVtx_z);
   tree->SetBranchAddress("DVeff.truthPos_nReconstructible",           &dveff.truthPos_nReconstructible);
   tree->SetBranchAddress("DVeff.truthPos_phi",                        &dveff.truthPos_phi);
   tree->SetBranchAddress("DVeff.truthPos_r",                          &dveff.truthPos_r);
   tree->SetBranchAddress("DVeff.truthPos_reconstructibleMass",        &dveff.truthPos_reconstructibleMass);
   tree->SetBranchAddress("DVeff.truthPos_size",                       &dveff.truthPos_size);
   tree->SetBranchAddress("DVeff.truthPos_sum_eta",                    &dveff.truthPos_sum_eta);
   tree->SetBranchAddress("DVeff.truthPos_sum_phi",                    &dveff.truthPos_sum_phi);
   tree->SetBranchAddress("DVeff.truthPos_sum_pt",                     &dveff.truthPos_sum_pt);
   tree->SetBranchAddress("DVeff.truthPos_x",                          &dveff.truthPos_x);
   tree->SetBranchAddress("DVeff.truthPos_y",                          &dveff.truthPos_y);
   tree->SetBranchAddress("DVeff.truthPos_z",                          &dveff.truthPos_z);
}
