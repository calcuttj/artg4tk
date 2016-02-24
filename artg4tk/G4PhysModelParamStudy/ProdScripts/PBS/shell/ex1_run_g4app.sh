#!/usr/bin/env bash
#

# pass it in via -v arg of qsub
#
# --> WORKDIR_TOP=${1}
#
if [ "x" == "x$WORKDIR_TOP" ] ; then
# G4WORKDIR=${PBS_O_WORKDIR}/.. 
   echo " env.var. WORKDIR_TOP is NOT set; bail out "
   exit 3
# else
#     echo "Variable says: $G4WORKDIR"
#     echo "Variable PBS_O_WORKDIR says: $PBS_O_WORKDIR"
fi

export EXTERNALS=/g4/yarba_j/scisoft-installs/externals

source ${EXTERNALS}/setup

# NOTE: setup == /usr/bin/setup
#
setup artg4tk v0_01_00 -q e7:prof
setup mrb v1_05_01

# This is needed for code development only
# 
#setup gitflow v1_8_0
#setup git v2_3_0
# export MRB_PROJECT=artg4tk

cd ${WORKDIR_TOP}

source ./localProducts_artg4tk_v0_01_00_e7_prof/setup

cd ${WORKDIR_TOP}/srcs/RooMUHistos
source ./env_set.sh

cd ${WORKDIR_TOP}

# mrbsetenv is an alias for the complete command below; 
# it will NOT work if issued from a script - one has to "source" it explicitly
# and this should be done AFTER source-ing this ./localProducts...../setup thing
# such deps on MRB are NOT super convenient, and at some point we might want
# to explore alternatives ^_^
#
source ${MRB_DIR}/bin/mrbSetEnv

JobID=${1}

G4ParamTest=${WORKDIR_TOP}/srcs/artg4tk/artg4tk/G4PhysModelParamStudy
cd ${G4ParamTest}/ProdScripts/PBS

rundirname=ex1_run_g4app
if [ ! "x" == "x$JobID" ]; then
rundirname=ex1_run_g4app_${JobID}
fi

if [ -d ${rundirname} ]; then
/usr/bin/rm -r -f ${rundirname}
fi

/bin/mkdir ${rundirname}
cd ${rundirname}

# NOTE: OK, this has worked out alright - now move to "parallel processing" !
#
# NOTE: art == ${ART_FQ_DIR}/bin/art
#
#art -c ${G4ParamTest}/fcl/test-g4params-producer-default.fcl
#art -c ${G4ParamTest}/fcl/test-g4params-producer-add-bertini-rscratio0.75.fcl

cfg_empty_source () {

#output=${1}

/usr/bin/printf " \n"
/usr/bin/printf "source: { \n" 
/usr/bin/printf "   module_type: EmptyEvent \n" 
/usr/bin/printf "   maxEvents: ${maxevents} \n" 
/usr/bin/printf "} \n" 

}

cfg_rootinput_source () {

/usr/bin/printf " \n"
/usr/bin/printf "source: { \n" 
/usr/bin/printf "   module_type: RootInput \n" 
/usr/bin/printf "   maxVvents: -1 \n" 
/usr/bin/printf "   fileNames: [ \"fname1.root\", \"fname2.root\" ] \n"
/usr/bin/printf "} \n" 

}

cfg_services () {

/usr/bin/printf " \n"
/usr/bin/printf "services: { \n" 

/usr/bin/printf "   message: { \n"  
/usr/bin/printf "      debugModules : [\"*\"] \n"  
/usr/bin/printf "      suppressInfo : [] \n"   
/usr/bin/printf "      destinations : { \n"   
/usr/bin/printf "         LogToConsole : { \n"  
/usr/bin/printf "            type : \"cout\" \n"  
/usr/bin/printf "            threshold : \"DEBUG\" \n"  
/usr/bin/printf "            categories : { default : { limit : 50 } } \n"  
/usr/bin/printf "       } \n" 
/usr/bin/printf "      } \n"  
/usr/bin/printf "   } \n" 

if [ ! "x" == "x$ts_filename" ]; then
/usr/bin/printf " TFileService: { fileName: \"${tsfilename}.root\" } \n"
fi

/usr/bin/printf "   RandomNumberGenerator: {} \n"
if [ ! "x" == "x$physlist" ]; then 
/usr/bin/printf "   PhysicsListHolder: {} \n"
/usr/bin/printf "   PhysicsList: { PhysicsListName: \"${physlist}\" } \n"
fi

/usr/bin/printf "   PhysModelConfig: { } \n"

/usr/bin/printf "} \n" 

}

cfg_outputs () {

/usr/bin/printf " \n"
/usr/bin/printf "outputs: { \n"
/usr/bin/printf "   ${outlabel}: { \n"
/usr/bin/printf "      module_type: RootOutput \n"
/usr/bin/printf "      fileName: \"${outfile}.root\" \n" 
/usr/bin/printf "   } \n"
/usr/bin/printf "} \n"

}

cfg_generator () {

/usr/bin/printf " \n"
/usr/bin/printf "      PrimaryGenerator: { \n"
/usr/bin/printf "         module_type: EventGenerator \n"
/usr/bin/printf "         nparticles : 1 \n"
/usr/bin/printf "         momentum: [ 0., 0., 5. ] // in GeV \n" 
/usr/bin/printf "      } \n"

}

cfg_g4producer () {

/usr/bin/printf " \n"
/usr/bin/printf "      G4${g4label}: { \n"
/usr/bin/printf "         module_type: ModelParamStudyProducer \n"
/usr/bin/printf "         RNDMSeed: ${seed} \n"
/usr/bin/printf "         TargetGeom: { \n"
/usr/bin/printf "            targetSize: [ 0., 3.15, 160. ] \n"
/usr/bin/printf " 	     targetPosition: [ 0, 0, 0 ] \n"
/usr/bin/printf "	     targetMaterial: \"Pb\" \n"
/usr/bin/printf "	     targetShape: \"G4Tubs\" \n"
/usr/bin/printf "         } \n"
/usr/bin/printf "  	  Verbosity: 0 \n"
/usr/bin/printf "	  HadronicModelParameters: { \n"
/usr/bin/printf "	     DefaultPhysics: ${defaults} \n"

#/usr/bin/printf "	     Bertini: { \n"
#/usr/bin/printf "	        RadiusScaleByRatio: 0.75 \n"
#/usr/bin/printf "	     } \n"

/usr/bin/printf "	  } \n"

/usr/bin/printf "      } \n"

}

#
# Make all of those then env.var. input, via -v foeld of qsub !
#
#process=A
#physlist=QGSP_FTFP_BERT 
#
#g4label=Default
#defaults=true
#
#maxevents=1000

config=ex1_cfg${JobID}.fcl

ts_filename=

outlabel=out${process}
outfile=Evt-G4-${g4label}
seed=1234
if [ ! "x" == "x$JobID" ]; then
seed=$((1234+${JobID}))
outfile=Evt-G4-${g4label}-${JobID}
fi

/usr/bin/printf "process_name: process${process} \n" >> ${config}

cfg_empty_source >> ${config}
cfg_services >> ${config}
cfg_outputs >> ${config}

/usr/bin/printf "physics: { \n" >> ${config}
/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "   producers: { \n" >> ${config}

cfg_generator >> ${config}
/usr/bin/printf " \n" >> ${config}
cfg_g4producer >> ${config}

/usr/bin/printf "   } \n" >> ${config}

/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "   analyzers: { \n" >> ${config}
/usr/bin/printf " \n" >> ${config}
/usr/bin/printf "   } \n" >> ${config}

/usr/bin/printf " \n" >> ${config}
/usr/bin/printf "   path1: [ PrimaryGenerator, G4${g4label} ] \n" >> ${config} 
/usr/bin/printf "   stream1: [ ${outlabel} ] \n" >> ${config}
/usr/bin/printf "   end_paths: [ path1, stream1 ] \n" >> ${config}

/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "} \n" >> ${config}

# NOTE: art == ${ART_FQ_DIR}/bin/art
#
art -c ${config}

# /usr/bin/rm -f ${config} 

# procname=( Default )
# for (( i=0; i<${#procname[@]}; ++i )) do
# config=G4App.${procname[$i]}
# done
