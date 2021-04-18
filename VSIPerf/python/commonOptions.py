import os
import sys
import logging
import shutil
import ROOT
from copy import copy
from copy import deepcopy

from optparse import OptionParser

from datetime import date

import collections


def parseCommonOptions() :
    parser = OptionParser()
    parser.add_option("--submitDir", help   = "dir to store the output", default="submit_dir")
    parser.add_option("--inputDS", help     = "You can pass either the directory locally, the file containing the list of grid datasets, or directly the name of a grid dataset. ", default="/afs/cern.ch/work/l/larry/public/lvlv_datasets/")

    parser.add_option("--gridUser", help    = "gridUser"  , default= '')
    parser.add_option("--gridTag", help     = "gridTag"   , default= '')

    parser.add_option("--driver", help      = "select where to run", choices=("direct", "prooflite", "lsf", "grid", "condor"), default="direct")
    parser.add_option('--doOverwrite', help = "Overwrite submit dir if it already exists",action="store_true", default=False)
    parser.add_option('--nevents', help     = "Run n events ", default = -1 )
    parser.add_option('--skipevents', help     = "skip n events ", default = 0 )
    parser.add_option('--verbosity', help   = "Run all algs at the selected verbosity.",choices=("info", "warning","error", "debug", "verbose"), default="error")
    parser.add_option('--doSystematics', help = "Do systematics.  This will take *MUCH* longer!",action="store_true", default=False)
    return parser

import atexit
@atexit.register
def quiet_exit():
    ROOT.gSystem.Exit(0)

def setVerbosity ( alg , levelString ) :
    level = None
    if levelString == "info"    : level = ROOT.MSG.INFO
    if levelString == "warning" : level = ROOT.MSG.WARNING
    if levelString == "error"   : level = ROOT.MSG.ERROR
    if levelString == "debug"   : level = ROOT.MSG.DEBUG
    if levelString == "verbose" : level = ROOT.MSG.VERBOSE

    if not level :
        logging.info("you set an illegal verbosity! Exiting.")
        commonOptions.quiet_exit()
    alg.setMsgLevel(level)

def fillSampleHandler ( sh_all, inp) :
    """You can pass either the directory locally, the file containing the list of grid datasets, or directly the name of a grid dataset. """
    if os.path.isfile(inp) :
        if ".root" in inp[-9:]:
            tmpFileName = inp.split("/")[-1]
            tmpDirName  = "/".join(inp.split("/")[:-1])
            ROOT.SH.ScanDir().sampleDepth(0).samplePattern(tmpFileName).scan(sh_all,tmpDirName)
        else:
            # This is if you're handing a file full of dataset names
            with open(inp) as f:
                for ds in f :
                    #this removes the scope and trailing whitespace
                    cleanedDs = (ds.split(":")[-1]).rstrip()
                    ROOT.SH.addGrid(sh_all, cleanedDs )
    elif os.path.isdir(inp) :
        mylist = ROOT.SH.DiskListLocal(inp)
        ROOT.SH.scanDir(sh_all,mylist, "*")
        iSampleDepth = 0
        while sh_all.size()==0 and iSampleDepth<3:
            ROOT.SH.ScanDir().sampleDepth(iSampleDepth).scan(sh_all,inp)
            iSampleDepth += 1
    elif "root://" in inp:
        server, path = inp.replace('root://','').split('//')
        sh_list = ROOT.SH.DiskListXRD(server, os.path.join('/', path), True)
        ROOT.SH.ScanDir().scan(sh_all, sh_list)
    else :
        ROOT.SH.scanRucio(sh_all, inp)
        sh_all.setMetaString("nc_grid_filter", "*");
    if not sh_all.size() :
        functionName = lambda : sys._getframe(1).f_code.co_name
        logging.warning("failed to find any samples in " + functionName())

def addAlgsFromDict( job,  algsToRun , verbosity = "error", verbosityBlackList = [] ) :
    for name,alg in algsToRun.iteritems() :
        setNameFunc = getattr(alg, "SetName", None)
        if setNameFunc != None:
            if name not in verbosityBlackList:
                setVerbosity(alg , verbosity)
            else:
                logging.info("Silencing %s to error level verbosity. See optional argument to addAlgsFromDict() to change at user level"%name)
                setVerbosity(alg, "error")
            logging.info("adding " + name + " to algs" )
            alg.SetName(name)#this is needed to see the alg names with athena messaging
        job.algsAdd(alg)

def overwriteSubmitDir (submitDir, doOverwrite) :
    if os.path.isdir(submitDir) :
        logging.info(submitDir + " already exists.")
        if doOverwrite :
            logging.info( "Overwriting previous submitDir")
            shutil.rmtree(submitDir)
        else :
            logging.info( "Exiting.  If you want to overwrite the previous submitDir, use --doOverwrite")
            quiet_exit()

def submitJob (job , driverName , submitDir, gridUser = "" , gridTag = "", nEvents=-1 ) :
    if (gridUser or gridTag) and driverName != "grid" :
        logging.error("You have specified a gridUser or gridTag but are not using the grid driver.  Exiting without submitting.")
        quiet_exit()

    logging.info("Running " + str(nEvents) + " events")
    job.options().setDouble (ROOT.EL.Job.optMaxEvents, float(nEvents));

    logging.info("creating driver")
    driver = None
    if (driverName == "direct"):
        logging.info( "direct driver")
        driver = ROOT.EL.DirectDriver()
        job.options().setDouble (ROOT.EL.Job.optCacheSize, 100*1024*1024);
        job.options().setDouble (ROOT.EL.Job.optCacheLearnEntries, 1000);
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif driverName == "lsf" :
        driver = ROOT.EL.LSFDriver()
        # ROOT.SH.scanNEvents(sh);
        # sh.setMetaDouble(ROOT.EL.Job.optEventsPerWorker, 50000);
        job.options().setString(ROOT.EL.Job.optSubmitFlags, "-q " + "1nh");
        driver.submit(job, submitDir)
    elif (driverName == "prooflite"):
        logging.info( "prooflite")
        driver = ROOT.EL.ProofDriver()
        logging.info("submit job")
        driver.submit(job, submitDir)
    elif (driverName == "grid"):
        logging.info( "grid driver")
        if not gridUser : gridUser = os.environ.get("USER")
        if not gridTag  : gridTag  = date.today().strftime("%m%d%y")
        driver = ROOT.EL.PrunDriver()
        gridOutputDatasetName = "user.%s.%%in:name[1]%%.%%in:name[2]%%.%%in:name[3]%%.%s"%(gridUser,gridTag)
        driver.options().setString("nc_outputSampleName", gridOutputDatasetName  );
        driver.options().setString(ROOT.EL.Job.optGridNGBPerJob,  "MAX");
        #driver.options().setString(ROOT.EL.Job.optGridNFilesPerJob,  "1");
        driver.options().setDouble(ROOT.EL.Job.optGridMergeOutput, 1);

        if  nEvents > 0:
            logging.warning( "Grid driver doesn't support ---nevents option. Assuming you want a test job so I'll submit a 1-file job." )
            driver.options().setDouble(ROOT.EL.Job.optGridNFiles, 1);

        logging.info("submit job")
        driver.submitOnly(job, submitDir)
    elif (driverName == "condor"):
        driver = ROOT.EL.CondorDriver()
        driver.options().setBool(ROOT.EL.Job.optBatchSharedFileSystem, False)
        driver.options().setString(ROOT.EL.Job.optCondorConf, "stream_output = true")
        driver.submitOnly(job, submitDir)
    else :
        logging.info( "you gave an illegal driver name.  Not submitting.")


def getSystList(dataSource = 1) :
    if( dataSource == 0 ) : return []#for data, just run once with no systematics

    susyTools = ROOT.ST.SUSYObjDef_xAOD("getlist")
    susyTools.setDataSource(dataSource)
    susyTools.setProperty("ConfigFile", "SUSYTools/SUSYTools_Default.conf")

    PRWLumiCalcFiles = ROOT.std.vector('string')(0)
    PRWLumiCalcFiles.push_back("$ROOTCOREBIN/data/FactoryTools/ilumicalc_histograms_None_276262-304494_OflLumi-13TeV-005.root")

    PRWConfigFiles = ROOT.std.vector('string')(0)
    PRWConfigFiles.push_back("/cvmfs/atlas.cern.ch/repo/sw/database/GroupData/dev/SUSYTools/merged_prw_mc15c.root")

    susyTools.setProperty("PRWConfigFiles", PRWConfigFiles )
    susyTools.setProperty("PRWLumiCalcFiles", PRWLumiCalcFiles  )
    logging.info("initializing SUSYTools")

    susyTools.initialize()

    # registry = ROOT.CP.SystematicRegistry.getInstance()
    # recommendedSystematics = registry.recommendedSystematics()

    recommendedSystematics = susyTools.getSystInfoList()

    logging.debug("Full list of recommended systematics is (%d):"%recommendedSystematics.size())
    for systInfo in recommendedSystematics:
        logging.debug(systInfo.systset.name() )

    return recommendedSystematics


def move_element(odict, thekey, newpos):
    odict[thekey] = odict.pop(thekey)
    i = 0
    for key, value in odict.items():
        if key != thekey and i >= newpos:
            odict[key] = odict.pop(key)
        i += 1
    return odict


def initializeRunScript(options, args):

    ROOT.gROOT.Macro( '$ROOTCOREDIR/scripts/load_packages.C' )

    logging.basicConfig(level=logging.INFO)
    # create a new sample handler to describe the data files we use
    logging.info("creating new sample handler")
    sh_all = ROOT.SH.SampleHandler()

    fillSampleHandler(sh_all, options.inputDS)

    sh_all.setMetaString ("nc_tree", "CollectionTree");

    # this is the basic description of our job
    logging.info("creating new job")

    job = ROOT.EL.Job()
    job.sampleHandler(sh_all)
    job.useXAOD()
    job.options().setString( ROOT.EL.Job.optXaodAccessMode,ROOT.EL.Job.optXaodAccessMode_athena );

    return job
