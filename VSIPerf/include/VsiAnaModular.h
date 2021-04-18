#ifndef VsiAnalysis_VsiAnaModular_H
#define VsiAnalysis_VsiAnaModular_H

#include <include/ModularAlgorithmBase.h>

class VsiAnaModular : public ModularAlgorithmBase
{
 private:
  
  virtual void initProcessors();
  
 public:
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
  
  enum class Release { r20p7, r21 };
  
  int         prop_do_eventInfo;
  int         prop_do_truth;
  int         prop_do_IDTracksPrescaled;
  int         prop_do_IDTracksSelected;
  int         prop_do_primaryVertices;
  int         prop_do_vsiVertices;
  int         prop_do_efficiency;
  int         prop_do_display;
  
  std::string prop_probeTruth;
  std::string prop_containerName;
  Release     prop_release;
  double      prop_chi2Cut;              
  int         prop_hitPatternReq;        
  int         prop_doDropAssociated;     
  int         prop_fillTracks;           
  double      prop_trackStorePrescale;   
  double      prop_d0_wrtSVCut;          
  double      prop_z0_wrtSVCut;          
  double      prop_errd0_wrtSVCut;       
  double      prop_errz0_wrtSVCut;       
  double      prop_d0signif_wrtSVCut;    
  double      prop_z0signif_wrtSVCut;    
  double      prop_chi2_toSVCut;         
  double      prop_displayScope;         
  int         prop_eff_doTruthRecursive;
    
  // this is a standard constructor
  VsiAnaModular();
  ~VsiAnaModular(){};

  // this is needed to distribute the algorithm to the workers
  ClassDef(VsiAnaModular, 1);
};


#endif
