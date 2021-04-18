#ifndef TrackObj_H
#define TrackObj_H

#pragma link C++ class std::vector<std::vector<int>>+;
#pragma link C++ class std::vector<std::vector<float>>+;
#pragma link C++ class std::vector<std::vector<unsigned>>+;
#pragma link C++ class std::vector<std::vector<unsigned char>>+;

#include <TTree.h>
#include <vector>

class TrackObj {
public:
    TrackObj();
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
};

TrackObj::TrackObj()
    : chi2(nullptr), d0(nullptr), z0(nullptr),
      pt(nullptr), eta(nullptr), phi(nullptr),
      errd0(nullptr), errz0(nullptr),
      selected(nullptr)
{}


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

#endif
