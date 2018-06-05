#!/usr/bin/env bash
#


#qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${WKDIRTOP},App=bertini_all_params.sh,process=A,g4label=Default,defaults=true,target=Pb,pdgcode=-211,momz=5.0 -A g4p pbs_multiCore_master.sh
#
# pi- jobs
#
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=Pb,PARAMETER=rscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=Pb,PARAMETER=xsscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=Pb,PARAMETER=fscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=Pb,PARAMETER=trr -A g4p analysis_each_par.sh
#
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=C,PARAMETER=rscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=C,PARAMETER=xsscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=C,PARAMETER=fscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=piminus,TARGET=C,PARAMETER=trr -A g4p analysis_each_par.sh
#
# proton jobs
#
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=Pb,PARAMETER=rscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=Pb,PARAMETER=xsscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=Pb,PARAMETER=fscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=Pb,PARAMETER=trr -A g4p analysis_each_par.sh
#
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=C,PARAMETER=rscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=C,PARAMETER=xsscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=C,PARAMETER=fscale -A g4p analysis_each_par.sh
qsub -q amd32_g4val -l nodes=1:slow,walltime=24:00:00 -v WORKDIR_TOP=${MRB_TOP},BEAM=proton,TARGET=C,PARAMETER=trr -A g4p analysis_each_par.sh

#end of script



