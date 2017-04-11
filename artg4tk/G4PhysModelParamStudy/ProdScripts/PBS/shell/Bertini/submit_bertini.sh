#!/usr/bin/env bash
#


#qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${WKDIRTOP},App=bertini_all_params.sh,process=A,g4label=Default,defaults=true,target=Pb,pdgcode=-211,momz=5.0 -A g4p pbs_multiCore_master.sh
#
# pi- jobs
#
# qsub -q amd32_g4val -l nodes=1:slow,walltime=12:00:00 -v WORKDIR_TOP=${MRB_TOP},App=bertini_all_params.sh,process=A,g4label=Default,defaults=true,target=C,pdgcode=-211,momz=5.0 -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=36:00:00 -v WORKDIR_TOP=${MRB_TOP},App=bertini_all_params.sh,process=B,g4label=Default,defaults=true,target=Pb,pdgcode=-211,momz=5.0,random=yes -A g4p pbs_multiCore_master.sh
#
# proton jobs
#
# qsub -q amd32_g4val -l nodes=1:slow,walltime=12:00:00 -v WORKDIR_TOP=${MRB_TOP},App=bertini_all_params.sh,process=A,g4label=Default,defaults=true,target=C,pdgcode=2212,momz=5.0 -A g4p pbs_multiCore_master.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=36:00:00 -v WORKDIR_TOP=${MRB_TOP},App=bertini_all_params.sh,process=B,g4label=Default,defaults=true,target=Pb,pdgcode=2212,momz=5.0,random=yes -A g4p pbs_multiCore_master.sh

#end of script



