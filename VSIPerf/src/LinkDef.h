#include <include/ModularAlgorithmBase.h>
#include <include/VsiAnaExample.h>
#include <include/VsiAnaModular.h>
#include <include/NtupleObj.h>

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

#endif

#ifdef __CINT__

// These classes are for EventLoop
#pragma link C++ class ModularAlgorithmBase+;
#pragma link C++ class VsiAnaExample+;
#pragma link C++ class VsiAnaModular+;

// These classes are for macros
#pragma link C++ class std::vector<std::vector<int>>+;
#pragma link C++ class std::vector<std::vector<float>>+;
#pragma link C++ class std::vector<std::vector<unsigned>>+;
#pragma link C++ class std::vector<std::vector<unsigned char>>+;

#pragma link C++ class EventInfoObj+;
#pragma link C++ class TruthVtxObj+;
#pragma link C++ class TrackObj+;
#pragma link C++ class PVObj+;
#pragma link C++ class DVObj+;
#pragma link C++ class DVeffObj+;
#endif
