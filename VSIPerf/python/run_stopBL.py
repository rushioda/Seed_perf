#!/usr/bin/env python

import ROOT, logging, collections, commonOptions
# Workaround to fix threadlock issues with GUI
ROOT.PyConfig.StartGuiThread = False

parser = commonOptions.parseCommonOptions()

#you can add additional options here if you want
#parser.add_option('--verbosity', help   = "Run all algs at the selected verbosity.",choices=("info", "warning","error", "debug", "verbose"), default="error")

(options, args) = parser.parse_args()

job = commonOptions.initializeRunScript(options, args)


######################################################################
##


logging.info("creating algorithms")

outputFilename = "VsiAnaModular"
output = ROOT.EL.OutputStream(outputFilename);

#here we add the algorithms we want to run over
import collections
algsToRun = collections.OrderedDict()

algsToRun["ntupSvc"] = ROOT.EL.NTupleSvc( outputFilename )

######################################################################
from AnaAlgorithm.AnaAlgorithmConfig import AnaAlgorithmConfig
vsialg   = ROOT.VsiAnaModular()

## General configurations
vsialg.setMsgLevel( 3 )

## Output tree name
vsialg.prop_outputName           = "vsitree"

## Output file name (use the same one as above)
vsialg.prop_outputFileName       = outputFilename


## Which release to be used (recommended to be r21)
vsialg.prop_mode                 = ROOT.VsiAnaModular.r21



##################################################
## Processor algorithm switches

## Record EventInfo information (usually true)
vsialg.prop_do_eventInfo         = True

## Record Truth vertex information (usually true for MC, no harm for data by settting True)
vsialg.prop_do_truth             = True
vsialg.prop_probeTruth           = "Stop1" ## other possibilities: "InvisibleFromHiggs" "RHadron" "HadInt" etc.


## Record ID track information by randomly prescaling (optional)
vsialg.prop_do_IDTracksPrescaled = True
vsialg.prop_trackStorePrescale   = 0.01


## Record ID track information of the selected tracks by VSI (optional)
vsialg.prop_do_IDTracksSelected  = True


## Record the primary vertices (recommended to be true)
vsialg.prop_do_primaryVertices   = True


## Record the DV information (recommended to be true)
vsialg.prop_do_vsiVertices       = True
vsialg.prop_containerName        = "SecondaryVertices" ## changed to others when looking at different containers
vsialg.prop_chi2Cut              = 9999.
vsialg.prop_hitPatternReq        = 0
vsialg.prop_doDropAssociated     = False

## use the following options as needed
vsialg.prop_d0_wrtSVCut          = 0.8
vsialg.prop_z0_wrtSVCut          = 1.2
vsialg.prop_errd0_wrtSVCut       = 999999
vsialg.prop_errz0_wrtSVCut       = 999999
vsialg.prop_d0signif_wrtSVCut    = 5.0
vsialg.prop_z0signif_wrtSVCut    = 5.0
vsialg.prop_chi2_toSVCut         = 5.0

## Record the DV information (recommended to be false)
vsialg.prop_do_efficiency        = True
vsialg.prop_eff_doTruthRecursive = True # Recursively associate descendent as well

## Dump the event displays (optional)
vsialg.prop_do_display           = False
vsialg.prop_displayScope         = 130  # Event display draw range [mm]


## Add algorithm
algsToRun["vsialg"]  = vsialg

##
######################################################################

job.outputAdd(output)

commonOptions.addAlgsFromDict(job , algsToRun , options.verbosity)

commonOptions.overwriteSubmitDir(options.submitDir , options.doOverwrite)
commonOptions.submitJob         ( job , options.driver , options.submitDir , options.gridUser , options.gridTag, options.nevents)

