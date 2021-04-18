#ifndef NtupleObj_H
#define NtupleObj_H

#include <TObject.h>
#include <TTree.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <vector>


//____________________________________________________________________________________________________
class EventInfoObj : public TObject {
public:
  EventInfoObj() {}
  virtual ~EventInfoObj() {}
  
   Float_t         actualMu;
   Float_t         averageMu;
   ULong64_t       eventNumber;
   UInt_t          lumiBlock;
   UInt_t          runNumber;
   UInt_t          mcChannelNumber;
   Float_t         mcWeight;
   
   ClassDef( EventInfoObj, 0 );
};




//____________________________________________________________________________________________________
class TruthVtxObj {
public:
  TruthVtxObj();
  virtual ~TruthVtxObj() {}
  
   std::vector<std::vector<float> >*         outP_eta;
   std::vector<std::vector<unsigned char> >* outP_isReco;
   std::vector<std::vector<float> >*         outP_phi;
   std::vector<std::vector<int> >*           outP_pid;
   std::vector<std::vector<float> >*         outP_pt;
   std::vector<float>*   parent_eta;
   std::vector<float>*   parent_phi;
   std::vector<int>*     parent_pid;
   std::vector<float>*   parent_prod_phi;
   std::vector<float>*   parent_prod_r;
   std::vector<float>*   parent_prod_x;
   std::vector<float>*   parent_prod_y;
   std::vector<float>*   parent_prod_z;
   std::vector<float>*   parent_pt;
   std::vector<float>*   phi;
   std::vector<float>*   r;
   std::vector<float>*   z;
   
   ClassDef( TruthVtxObj, 0 );
 };



//____________________________________________________________________________________________________
class TrackObj {
public:
    TrackObj();
    virtual~TrackObj() {}
    std::vector<float>*  chi2;
    std::vector<float>*  d0;
    std::vector<float>*  z0;
    std::vector<float>*  pt;
    std::vector<float>*  eta;
    std::vector<float>*  phi;
    std::vector<float>*  errd0;
    std::vector<float>*  errz0;
    std::vector<unsigned int>* selected;
    UInt_t          sampledSize;
    UInt_t          selectedSize;
    UInt_t          totalSize;
    
    ClassDef( TrackObj, 0 );
};





//____________________________________________________________________________________________________
class PVObj : public TObject {
public:
  PVObj() {}
  virtual ~PVObj() {}
   UInt_t          nTracksPVassoc;
   Float_t         phi;
   Float_t         r;
   Float_t         x;
   Float_t         y;
   Float_t         z;
   
   ClassDef( PVObj, 0 );
};


//____________________________________________________________________________________________________
class DVObj {
public:
    DVObj();
    virtual ~DVObj() {};
    
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
    
    ClassDef( DVObj, 0 );
};



class DVeffObj {
public:
  DVeffObj();
  virtual ~DVeffObj() {}
  
  ULong64_t                                 LLP_size;
  std::vector<float>                       *LLP_decay_phi;
  std::vector<float>                       *LLP_decay_r;
  std::vector<float>                       *LLP_decay_x;
  std::vector<float>                       *LLP_decay_y;
  std::vector<float>                       *LLP_decay_z;
  std::vector<float>                       *LLP_eta;
  std::vector<float>                       *LLP_mass;
  std::vector<float>                       *LLP_phi;
  std::vector<int>                         *LLP_pid;
  std::vector<float>                       *LLP_pt;
  std::vector<float>                       *LLP_childOpenAngle;
  
  
  
  std::vector<std::vector<float> >         *descendent_charge;
  std::vector<std::vector<int> >           *descendent_pid;
  std::vector<std::vector<float> >         *descendent_mass;
  std::vector<std::vector<float> >         *descendent_pt;
  std::vector<std::vector<float> >         *descendent_eta;
  std::vector<std::vector<float> >         *descendent_phi;
  
  std::vector<std::vector<float> >         *descendent_prod_r;
  std::vector<std::vector<float> >         *descendent_prod_x;
  std::vector<std::vector<float> >         *descendent_prod_y;
  std::vector<std::vector<float> >         *descendent_prod_z;
  std::vector<std::vector<float> >         *descendent_prod_phi;
  
  std::vector<std::vector<float> >         *descendent_decay_r;
  std::vector<std::vector<float> >         *descendent_decay_x;
  std::vector<std::vector<float> >         *descendent_decay_y;
  std::vector<std::vector<float> >         *descendent_decay_z;
  std::vector<std::vector<float> >         *descendent_decay_phi;
  
  std::vector<unsigned long>               *descendent_reco_size;
  std::vector<float>                       *descendent_reco_sum_eta;
  std::vector<float>                       *descendent_reco_sum_mass;
  std::vector<float>                       *descendent_reco_sum_phi;
  std::vector<float>                       *descendent_reco_sum_pt;
  
  std::vector<std::vector<char> >          *descendent_isReconstructed;
  std::vector<std::vector<char> >          *descendent_isSelected;
  std::vector<std::vector<unsigned long> > *descendent_truthPosIndex;
  
  std::vector<unsigned long>               *descendent_selected_size;
  std::vector<unsigned long>               *descendent_size;
  std::vector<float>                       *descendent_sum_pt;
  std::vector<float>                       *descendent_sum_eta;
  std::vector<float>                       *descendent_sum_phi;
  std::vector<float>                       *descendent_sum_mass;
  
  
  std::vector<unsigned long>               *truthPos_size;
  std::vector<std::vector<float> >         *truthPos_r;
  std::vector<std::vector<float> >         *truthPos_x;
  std::vector<std::vector<float> >         *truthPos_y;
  std::vector<std::vector<float> >         *truthPos_z;
  std::vector<std::vector<float> >         *truthPos_phi;
  std::vector<std::vector<unsigned long> > *truthPos_nReconstructible;
  std::vector<std::vector<float> >         *truthPos_reconstructibleMass;
  std::vector<std::vector<float> >         *truthPos_sum_pt;
  std::vector<std::vector<float> >         *truthPos_sum_eta;
  std::vector<std::vector<float> >         *truthPos_sum_phi;
  
  
  
  std::vector<unsigned long>               *recoVtx_LLP_index;
  std::vector<unsigned long>               *recoVtx_truthPos_index;
  std::vector<unsigned long>               *recoVtx_index;
  std::vector<float>                       *recoVtx_chi2;
  std::vector<float>                       *recoVtx_chi2_core;
  std::vector<float>                       *recoVtx_mass;
  std::vector<float>                       *recoVtx_massFraction;
  std::vector<float>                       *recoVtx_mass_nonAssociated;
  std::vector<float>                       *recoVtx_matchScore;
  std::vector<float>                       *recoVtx_multiplicityFraction;
  std::vector<unsigned long>               *recoVtx_ntrk;
  std::vector<unsigned long>               *recoVtx_ntrk_nonAssociated;
  std::vector<float>                       *recoVtx_filtered_mass;
  std::vector<float>                       *recoVtx_filtered_mass_nonAssociated;
  std::vector<unsigned long>               *recoVtx_filtered_ntrk;
  std::vector<unsigned long>               *recoVtx_filtered_ntrk_nonAssociated;
  std::vector<float>                       *recoVtx_r;
  std::vector<float>                       *recoVtx_x;
  std::vector<float>                       *recoVtx_y;
  std::vector<float>                       *recoVtx_z;
  std::vector<float>                       *recoVtx_phi;
  std::vector<float>                       *recoVtx_residual_r;
  std::vector<float>                       *recoVtx_residual_x;
  std::vector<float>                       *recoVtx_residual_y;
  std::vector<float>                       *recoVtx_residual_z;
  std::vector<float>                       *recoVtx_residual_phi;
  
  ClassDef( DVeffObj, 0 );
};


//____________________________________________________________________________________________________
void setBranchAddress( TTree *tree, EventInfoObj& ei );
void setBranchAddress( TTree* tree, TruthVtxObj& truth, const std::string name );
void setBranchAddress( TTree* tree, TrackObj& trk, const std::string name );
void setBranchAddress( TTree* tree, PVObj& pv );
void setBranchAddress( TTree* tree, DVObj& dv );
void setBranchAddress( TTree* tree, DVeffObj& dveff );

#endif
