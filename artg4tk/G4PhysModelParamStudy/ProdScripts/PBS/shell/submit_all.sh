#!/usr/bin/env bash
#


qsub -q amd32_Geant4 -l nodes=1:amd32,walltime=24:00:00 -v  WORKDIR_TOP=${WKDIRTOP},App=ex1_run_g4app.sh,process=A,g4label=Default,defaults=true,gdmlfile=./lariat.gdml,physlist=QGSP_FTFP_BERT,maxevents=10000 -A g4p pbs_multiCore_master.sh

#end of script



