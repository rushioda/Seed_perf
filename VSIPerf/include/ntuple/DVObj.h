#ifndef DVOBJ_H
#define DVOBJ_H

#pragma link C++ class std::vector<std::vector<int>>+;
#pragma link C++ class std::vector<std::vector<float>>+;
#pragma link C++ class std::vector<std::vector<unsigned>>+;
#pragma link C++ class std::vector<std::vector<unsigned char>>+;

#include <TTree.h>
#include <vector>

class DVObj {
public:
    DVObj();
    
    std::vector< float>*   x;
    std::vector< float>*   y;
    std::vector< float>*   z;
    std::vector< float>*   r;
    std::vector< float>*   phi;
    std::vector< float>*   direction;
    std::vector< float>*   chi2;
    std::vector< std::vector< float> >* covariance;
    std::vector< float>*   pt;
    std::vector< float>*   H;
    std::vector< float>*   HT;
    std::vector< unsigned int>* ntrk;
    std::vector< unsigned int>* ntrk_associated;
    std::vector< unsigned int>* ntrk_selected;
    std::vector< float>*   mass;
    std::vector< float>*   mass_nonAssociated;
    std::vector< int>*     charge;
    std::vector< float>*   maxOpAng;
    std::vector< float>*   minOneTrackRemovedMass;
    std::vector< float>*   minOpAng;
    std::vector< float>*   mind0;
    std::vector< float>*   maxd0;
    
    std::vector< std::vector< int> >*           trk_charge;
    std::vector< std::vector< int> >*           trk_truthPid;
    std::vector< std::vector< float> >*         trk_chi2_toSV;
    std::vector< std::vector< float> >*         trk_d0;
    std::vector< std::vector< float> >*         trk_d0_wrtSV;
    std::vector< std::vector< float> >*         trk_dEdx;
    std::vector< std::vector< float> >*         trk_errd0_wrtSV;
    std::vector< std::vector< float> >*         trk_errz0_wrtSV;
    std::vector< std::vector< float> >*         trk_eta;
    std::vector< std::vector< float> >*         trk_eta_wrtSV;
    std::vector< std::vector< float> >*         trk_phi;
    std::vector< std::vector< float> >*         trk_phi_wrtSV;
    std::vector< std::vector< float> >*         trk_pt;
    std::vector< std::vector< float> >*         trk_pt_wrtSV;
    std::vector< std::vector< float> >*         trk_qOverP;
    std::vector< std::vector< float> >*         trk_z0;
    std::vector< std::vector< float> >*         trk_z0_wrtSV;
    std::vector< std::vector< unsigned char> >* trk_isAssociated;
    std::vector< std::vector< unsigned char> >* trk_isSelected;
    std::vector< std::vector< unsigned char> >* trk_nIBLOverflowsdEdx;
    std::vector< std::vector< unsigned char> >* trk_nPixelBarrelLayers;
    std::vector< std::vector< unsigned char> >* trk_nPixelEndCapLayers;
    std::vector< std::vector< unsigned char> >* trk_nPixelHits;
    std::vector< std::vector< unsigned char> >* trk_nSCTHits;
    std::vector< std::vector< unsigned char> >* trk_nSctBarrelLayers;
    std::vector< std::vector< unsigned char> >* trk_nSctEndCapLayers;
    std::vector< std::vector< unsigned char> >* trk_nTRTHits;
    std::vector< std::vector< unsigned char> >* trk_nUsedHitsdEdx;
    std::vector< std::vector< unsigned char> >* trk_truthIsPointingToClosestTruthVtx;
    std::vector< std::vector< unsigned char> >* trk_truthIsPointingToReprTruthVtx;
    std::vector< std::vector< unsigned int> >*  trk_final;
    std::vector< std::vector< unsigned int> >*  trk_hitPattern;
    
    std::vector< std::vector< int> >*   twoTracksCharge;
    std::vector< std::vector< float> >* twoTracksMass;
    std::vector< std::vector< float> >* twoTracksMassRest;
    
    std::vector< float>*                        closestTruth_distance;
    std::vector< float>*                        closestTruth_inE;
    std::vector< float>*                        closestTruth_outE;
    std::vector< float>*                        closestTruth_parent_eta;
    std::vector< float>*                        closestTruth_parent_phi;
    std::vector< float>*                        closestTruth_parent_pt;
    std::vector< float>*                        closestTruth_phi;
    std::vector< float>*                        closestTruth_r;
    std::vector< float>*                        closestTruth_x;
    std::vector< float>*                        closestTruth_y;
    std::vector< float>*                        closestTruth_z;
    std::vector< int>*                          closestTruth_parent;
    std::vector< std::vector< float> >*         closestTruth_outP_eta;
    std::vector< std::vector< float> >*         closestTruth_outP_phi;
    std::vector< std::vector< float> >*         closestTruth_outP_pt;
    std::vector< std::vector< int> >*           closestTruth_outP_charge;
    std::vector< std::vector< int> >*           closestTruth_outP_pid;
    std::vector< std::vector< unsigned char> >* closestTruth_outP_isReco;
    
    std::vector< float>*                        reprTruth_inE;
    std::vector< float>*                        reprTruth_matchScore;
    std::vector< float>*                        reprTruth_outE;
    std::vector< float>*                        reprTruth_parent_eta;
    std::vector< float>*                        reprTruth_parent_phi;
    std::vector< float>*                        reprTruth_parent_pt;
    std::vector< float>*                        reprTruth_phi;
    std::vector< float>*                        reprTruth_r;
    std::vector< float>*                        reprTruth_x;
    std::vector< float>*                        reprTruth_y;
    std::vector< float>*                        reprTruth_z;
    std::vector< int>*                          reprTruth_parent;
    std::vector< std::vector< float> >*         reprTruth_outP_eta;
    std::vector< std::vector< float> >*         reprTruth_outP_phi;
    std::vector< std::vector< float> >*         reprTruth_outP_pt;
    std::vector< std::vector< int> >*           reprTruth_outP_charge;
    std::vector< std::vector< int> >*           reprTruth_outP_pid;
    std::vector< std::vector< unsigned char> >* reprTruth_outP_isReco;
    
};


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


#endif
