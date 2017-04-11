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

cd /g4/yarba_j/art-g4-apps-work/try_artg4tk_nov14_2016/RooMUHistos
source ./env_set.sh

cd ${WORKDIR_TOP}
source ./localProducts_artg4tk_v0_01_00_e7_prof/setup
source ${MRB_DIR}/bin/mrbSetEnv

cd ${WORKDIR_TOP}/srcs/artg4tk/artg4tk/G4PhysModelParamStudy/analysis/Bertini

beam=${BEAM}
target=${TARGET}
param=${PARAMETER}

config=${beam}5GeV-${target}-${param}.fcl

art -c ${config}

