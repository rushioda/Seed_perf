#ifndef EventInfoObj_H
#define EventInfoObj_H

#include <TTree.h>

class EventInfoObj : public TObject {
public:
   Float_t         actualMu;
   Float_t         averageMu;
   ULong64_t       eventNumber;
   UInt_t          lumiBlock;
   UInt_t          runNumber;
   
   ClassDef( EventInfoObj, 1 );
};

void setBranchAddress( TTree *tree, EventInfoObj& ei ) {
   tree->SetBranchAddress("EventInfo.actualMu",&ei.actualMu);
   tree->SetBranchAddress("EventInfo.averageMu",&ei.averageMu);
   tree->SetBranchAddress("EventInfo.eventNumber",&ei.eventNumber);
   tree->SetBranchAddress("EventInfo.lumiBlock",&ei.lumiBlock);
   tree->SetBranchAddress("EventInfo.runNumber",&ei.runNumber);
}

#endif
