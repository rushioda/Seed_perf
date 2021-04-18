################################################################################
# Package: VSIPerf
################################################################################

cmake_minimum_required(VERSION 3.11)

# Declare the package name:
atlas_subdir(VSIPerf)

# Extra dependencies based on the build environment:
set( extra_deps )

if( XAOD_STANDALONE )
   set( extra_deps PhysicsAnalysis/D3PDTools/EventLoop
      PRIVATE Control/xAODRootAccessInterfaces Control/xAODRootAccess )
endif()

set( extra_libs )

if( XAOD_STANDALONE )
   set( extra_public_libs EventLoop EventLoopGrid )
   set( extra_private_libs xAODRootAccessInterfaces xAODRootAccess )
endif()

set(analysis_deps)

# Declare the package's dependencies:
atlas_depends_on_subdirs(
   PUBLIC
   PhysicsAnalysis/D3PDTools/SampleHandler 
   PhysicsAnalysis/D3PDTools/EventLoopAlgs
   PhysicsAnalysis/AnalysisCommon/CPAnalysisExamples
   Control/AthToolSupport/AsgTools
   Event/xAOD/xAODCore
   Event/xAOD/xAODEgamma
   Event/xAOD/xAODEventInfo
   Event/xAOD/xAODJet
   Event/xAOD/xAODMissingET
   Event/xAOD/xAODTrigMissingET
   Event/xAOD/xAODMuon
   Event/xAOD/xAODTau
   Event/xAOD/xAODTracking
   Event/xAOD/xAODTruth
   Event/xAOD/xAODMetaData
   ${extra_deps} )

# External dependencies:
find_package( Boost )
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO Graf Gpad )
find_package( GTest )

# ROOT Dictionary
atlas_add_root_dictionary( VSIPerfLib VSIPerfCintDict 
                           ROOT_HEADERS include/*.h src/LinkDef.h
                           EXTERNAL_PACKAGES ROOT )

# Libraries in the package:
atlas_add_library( VSIPerfLib 
                   include/*.h src/*.cxx ${VSIPerfCintDict}
                   PUBLIC_HEADERS VSIPerf ROOT_HEADERS
                   PRIVATE_INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} ${BOOST_INCLUDE_DIRS}
                   LINK_LIBRARIES AsgTools xAODCore xAODEventInfo xAODTracking xAODTruth xAODEgamma xAODMuon ${extra_public_libs}
                   PRIVATE_LINK_LIBRARIES ${ROOT_LIBRARIES} ${BOOST_LIBRARIES}
                   AthContainers EventPrimitives FourMomUtils xAODBase
                   xAODPrimitives PathResolver SampleHandler ${extra_libs} )

#atlas_add_executable( run_VSIPerf util/run_VSIPerf.cxx
#                     LINK_LIBRARIES ${ROOT_LIBRARIES} ${Boost_LIBRARIES} PATInterfaces
#                     xAODRootAccess AsgTools VSIPerfLib ${analysis_deps} ${extra_public_libs} )

atlas_install_data( data/* )
atlas_install_scripts( python/* )
