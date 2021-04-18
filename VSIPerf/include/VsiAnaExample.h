#ifndef VsiAnalysis_VsiAnaExample_H
#define VsiAnalysis_VsiAnaExample_H

#include <include/VsiBonsaiTool.h>
#include <include/TreeManager.h>
#include <include/AnyType.h>

#include <EventLoop/Algorithm.h>

#include <xAODEventInfo/EventInfo.h>
#include <xAODTruth/TruthParticleContainer.h>
#include <xAODTruth/TruthVertexContainer.h>
#include <xAODTracking/TrackParticleContainer.h>
#include <xAODTracking/VertexContainer.h>

#include <xAODTracking/TrackParticle.h>
#include <xAODTracking/Vertex.h>

#include <vector>
#include <map>
#include <string.h>
#include <TH1.h>
#include <iostream>
#include <fstream>


class VsiAnaExample_impl;

class VsiAnaExample : public EL::Algorithm
{
 private:
  
  // variables that don't get filled at submission time should be
  // protected from being send from the submission node to the worker
  // node (done by the //!)
  
  VsiAnaExample_impl *m_impl; //!
  
  void do_truth( const xAOD::TruthVertexContainer* );
  void do_tracks( const xAOD::TrackParticleContainer*, const xAOD::TrackParticleContainer*, const xAOD::VertexContainer*, unsigned&, unsigned& );
  void do_vsiVertices( const xAOD::VertexContainer*, const xAOD::Vertex* );
  
  void printVertex( const xAOD::Vertex* );
  
 public:
  // put your configuration variables here as public variables.
  // that way they can be set directly from CINT and python.
  
  enum class Release { r20p7, r21 };
  
  std::string prop_probeTruth;      
  std::string prop_containerName;   
  std::string prop_outputName;      
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
  int         prop_doDisplay;
  int         prop_displayDoBatch;
  int         prop_displayMaxEvents;
    
  // this is a standard constructor
  VsiAnaExample();
  ~VsiAnaExample(){};

  // these are the functions inherited from Algorithm
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute ();
  virtual EL::StatusCode finalize ();

  // this is needed to distribute the algorithm to the workers
  ClassDef(VsiAnaExample, 1);
};

#endif
