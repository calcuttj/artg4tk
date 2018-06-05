#!/usr/bin/env bash
#

# pass it in via -v arg of qsub
#
# --> WORKDIR_TOP=${1}
#
if [ "x" == "x$WORKDIR_TOP" ] ; then
   echo " env.var. WORKDIR_TOP is NOT set; bail out "
   exit 3
fi

export EXTERNALS=/g4/yarba_j/scisoft-installs/externals

source ${EXTERNALS}/setup

# NOTE: setup == /usr/bin/setup
#
setup artg4tk v0_01_00 -q e7:prof
setup mrb v1_05_01

#cd ${WORKDIR_TOP}/srcs/RooMUHistos
cd /g4/yarba_j/art-g4-apps-work/try_artg4tk_nov14_2016/RooMUHistos
source ./env_set.sh

cd ${WORKDIR_TOP}
source ./localProducts_artg4tk_v0_01_00_e7_prof/setup

# mrbsetenv is an alias for the complete command below; 
# it will NOT work if issued from a script - one has to "source" it explicitly
# and this should be done AFTER source-ing this ./localProducts...../setup thing
# such deps on MRB are NOT super convenient, and at some point we might want
# to explore alternatives ^_^
#
source ${MRB_DIR}/bin/mrbSetEnv

nevents=${1}
JobID=${2}

declare -a g4paths

G4ParamTest=${WORKDIR_TOP}/srcs/artg4tk/artg4tk/G4PhysModelParamStudy

. ${PBS_O_WORKDIR}/art_input_sources.sh
. ${PBS_O_WORKDIR}/art_services.sh
. ${PBS_O_WORKDIR}/art_outputs.sh
. ${PBS_O_WORKDIR}/art_producers.sh
. ${PBS_O_WORKDIR}/g4params_bertini_rndm_each_par.sh

cd ${G4ParamTest}/ProdScripts/PBS

# process=A
maxevents=${nevents}
#
proc_level=Bertini
#target=Pb
# g4label=Default
#
#pdgcode=-211
#momz=5.0

if [ ${pdgcode} == "211" ]; then
particle=piplus
fi 
if [ ${pdgcode} == "-211" ]; then
particle=piminus
fi 
if [ ${pdgcode} == "2212" ]; then
particle=proton
fi 
if [ ${pdgcode} == "2112" ]; then
particle=proton
fi

base=${App%.*}
rundirname=${base}_${particle}${momz}GeV_${target}

if [ ! -d "${rundirname}" ]; then
/bin/mkdir ${rundirname}
fi

cd ${rundirname}

 
g4paths[0]=G4${proc_level}${g4label}

#config=bertini_all_params_${particle}${momz}GeV_${target}.fcl
config=${base}_${particle}${momz}GeV_${target}.fcl

ts_filename=

outlabel=out${process}
outfile=Evt-G4-${particle}${momz}GeV_${target}
seed=1234
if [ ! "x" == "x$JobID" ]; then
seed=$((1234+${JobID}))
outfile=Evt-G4-${particle}${momz}GeV-${target}-${JobID}
#config=bertini_all_params_${particle}${momz}GeV_${target}_${JobID}.fcl
config=${base}_${particle}${momz}GeV_${target}_${JobID}.fcl
fi

if [ -e ${config} ]; then
/bin/rm ${config}
fi

BertLabels=( Rsc XSsc Fsc TrR )

/usr/bin/printf "process_name: process${process} \n" >> ${config}

cfg_empty_source >> ${config}
cfg_services >> ${config}
cfg_outputs >> ${config}

/usr/bin/printf "physics: { \n" >> ${config}
/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "   producers: { \n" >> ${config}

cfg_generator >> ${config}

# default settings
#
cfg_g4processlevel >> ${config}

id=1
if [ "$proc_level" == "Bertini" ]; then
#
for ((i=0; i<${#BertiniParamsOneByOne[@]}; ++i)) do
pname=`echo ${BertiniParamsOneByOne[$i]} | awk -F '=' '{print $1}'`
content=`echo ${BertiniParamsOneByOne[$i]} | awk -F '=' '{print $NF}' | sed 's/(//g' | sed 's/)//g'`
icount=0
for pval in ${content}; do
defaults=false
# ---> g4label=${BertLabels[$i]}`echo ${pval} | sed 's/\.//g'`
g4label=${BertLabels[$i]}${icount}
icount=$((${icount}+1))
cfg_g4processlevel >> ${config}
g4paths[$id]=G4${proc_level}${g4label}
id=$((${id}+1))
done
done
#
fi

/usr/bin/printf "   } \n" >> ${config}

/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "   analyzers: { \n" >> ${config}
/usr/bin/printf " \n" >> ${config}
/usr/bin/printf "   } \n" >> ${config}

/usr/bin/printf " \n" >> ${config}
# /usr/bin/printf "   path1: [ PrimaryGenerator, G4${proc_level}${g4label} ] \n" >> ${config} 
/usr/bin/printf "   path1: [ PrimaryGenerator " >> ${config} 
for ((j=0; j<${#g4paths[@]}; ++j)) do
/usr/bin/printf ", ${g4paths[$j]} " >> ${config}
done
/usr/bin/printf " ] \n" >> ${config}
/usr/bin/printf "   stream1: [ ${outlabel} ] \n" >> ${config}
/usr/bin/printf "   end_paths: [ path1, stream1 ] \n" >> ${config}

/usr/bin/printf " \n" >> ${config}

/usr/bin/printf "} \n" >> ${config}

export LD_LIBRARY_PATH=${MRB_BUILDDIR}/artg4tk/lib:$LD_LIBRARY_PATH

echo " ***** ===== ***** "
echo "LD_LIBRARY_PATH = ${LD_LIBRARY_PATH}"
echo " ***** ===== ***** "

# NOTE: art == ${ART_FQ_DIR}/bin/art
#
art -c ${config}

#/bin/rm -f ${config}
