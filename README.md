# Simple EventLoop algorithm used for VSI performance studies

Setup Instructions:
```
cd SecVtxPUBNote/Code
source setup.sh
mkdir build; cd build
cmake ..
make -j4
source x86_64*/setup.sh
```

# Sample data (long lived stop -> b + l)
* mc16_13TeV.404970.MGPy8EG_A14N23LO_TT_RPVdirectBL_1100_tau_0p1ns.recon.DAOD_RPVLL.e6581_e5984_s3126_r10740_tid15119706_00

to run locally:

```
mkdir run
$ run_vsiAna_modular.py --nevents 1000 --doOverwrite --inputDS mc16_13TeV.404970.MGPy8EG_A14N23LO_TT_RPVdirectBL_1100_tau_0p1ns.recon.DAOD_RPVLL.e6581_e5984_s3126_r10740_tid15119706_00
```


to run on grid (to be tested)

```
$ run_vsiAna_modular.py --doOverwrite --driver grid --gridUser ${USER} --gridTag 20180821.1 --inputDS mc16_13TeV.404970.MGPy8EG_A14N23LO_TT_RPVdirectBL_1100_tau_0p1ns.recon.DAOD_RPVLL.e6581_e5984_s3126_r10740_tid15119706_00
```


Modify `run_vsiAna_modular.py` configs as needed.


# Running macro
`VSIPerf/macros/DVeffAnalysis.C` gives an example of reading the `DVeff` ntuple structure and print out the contents.

```
[macros]$ root -l -b
root [0] .L DVeffAnalysis.C+
root [1] DVeffAnalysis("/afs/cern.ch/user/h/hoide/workdir/public/20180823_VsiAna/stop_bl_1100GeV_1ns_sample.root")
```