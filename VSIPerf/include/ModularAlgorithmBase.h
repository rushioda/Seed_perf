#ifndef ModularAlgorithmBase_H
#define ModularAlgorithmBase_H

#include <EventLoop/Algorithm.h>

#include <include/TreeManager.h>
#include <include/ProcessorBase.h>

class TH1;

class ModularAlgorithmBase : public EL::Algorithm
{
 public:
  
  int prop_do_selectRegion;
  std::string prop_regionName;
  std::string prop_outputName;
  std::string prop_outputFileName;
  
 protected:
  
  unsigned long m_eventCount;                                    //!
  std::map<std::string, TH1*> m_histos;                          //!
  std::unique_ptr<TreeManager>  m_treeManager;                   //!
  std::vector< std::unique_ptr<ProcessorBase> > m_processors;    //!
  
  void registerVariables();
  virtual void initProcessors() {};

 public:
    
  // this is a standard constructor
  ModularAlgorithmBase();
  virtual ~ModularAlgorithmBase() {};
  
  // these are the functions inherited from Algorithm
  virtual EL::StatusCode initialize ();
  virtual EL::StatusCode execute    ();
  virtual EL::StatusCode finalize   ();
  
  // this is needed to distribute the algorithm to the workers
  ClassDef(ModularAlgorithmBase, 1);
};


#endif
