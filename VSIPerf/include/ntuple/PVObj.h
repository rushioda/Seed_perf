#ifndef PVObj_H
#define PVObj_H

#include <TTree.h>

class PVObj {
public:
   UInt_t          nTracksPVassoc;
   Float_t         phi;
   Float_t         r;
   Float_t         x;
   Float_t         y;
   Float_t         z;
};

void setBranchAddress( TTree* tree, PVObj& pv ) {
   tree->SetBranchAddress("PV.nTracksPVassoc",&pv.nTracksPVassoc);
   tree->SetBranchAddress("PV.phi",&pv.phi);
   tree->SetBranchAddress("PV.r",&pv.r);
   tree->SetBranchAddress("PV.x",&pv.x);
   tree->SetBranchAddress("PV.y",&pv.y);
   tree->SetBranchAddress("PV.z",&pv.z);
}

#endif
