#ifndef TruthVtxObj_H
#define TruthVtxObj_H

#pragma link C++ class std::vector<std::vector<int>>+;
#pragma link C++ class std::vector<std::vector<float>>+;
#pragma link C++ class std::vector<std::vector<unsigned>>+;
#pragma link C++ class std::vector<std::vector<unsigned char>>+;

#include <TTree.h>
#include <vector>

class TruthVtxObj {
public:
    TruthVtxObj();
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
 };


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

#endif
