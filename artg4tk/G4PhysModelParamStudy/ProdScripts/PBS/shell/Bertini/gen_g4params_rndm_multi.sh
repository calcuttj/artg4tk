#!/usr/bin/env bash
#

BertiniParRange=( 
'RadiusScale=(1 3.5) ' 
'XSecScale=(0.1 2) ' 
'FermiScale=(0.5 1) ' 
'TrailingRadius=(0 5) ' 
)

declare -a g4pname
declare -a g4pval

config=g4params_bertini_rndm_multi.sh

if [ -e ${config} ]; then
/bin/rm -f ${config}
fi

/usr/bin/printf "#!/usr/bin/env bash \n" >> ${config}
/usr/bin/printf "# \n" >> ${config}
/usr/bin/printf "BertiniParamsMulti=( \n" >> ${config}

for j in `seq 100`; do
/usr/bin/printf "'" >> ${config}
for ((i=0; i<${#BertiniParRange[@]}; ++i)) do
pname=`echo ${BertiniParRange[$i]} | awk -F '=' '{print $1}'`
range=`echo ${BertiniParRange[$i]} | awk -F '=' '{print $NF}' | sed 's/(//g' | sed 's/)//g'`
# range=`echo ${BertiniParRange[$i]} | awk -F '=' '{print $NF}'` 
# echo " pname = ${pname} "
# echo " range = ${range} "
rmin=`echo ${range} | awk -F ' ' '{print $1}'`
rmax=`echo ${range} | awk -F ' ' '{print $2}'`
pval=`python -c "import random; print random.uniform($rmin,$rmax)"`
/usr/bin/printf "${pname}=${pval} " >> ${config}
done
/usr/bin/printf "' \n" >> ${config}
done
/usr/bin/printf ") \n" >> ${config}

/bin/chmod +x ${config}

. ${config}

for ((i=0; i<${#BertiniParamsMulti[@]}; ++i)) do
pgroup=`echo ${BertiniParamsMulti[$i]} | awk -F ')' '{print $1}' | sed 's/(//g'`
echo " pgroup = ${pgroup} "
icount=0
g4pname=()
g4pval=()
echo " size of g4pname = ${#g4pname[@]} "
echo " size of g4pval  = ${#g4pval[@]} "
for pval in ${pgroup}; do
echo " pval = ${pval}"
parname=`echo ${pval} | awk -F '=' '{print $1}'`
parval=`echo ${pval} | awk -F '=' '{print $NF}'`
echo " ${parname}: ${parval} "
g4pname[$icount]=${parname}
g4pval[$icount]=${parval}
echo " re-check --> ${g4pname[$icount]}: ${g4pval[$icount]} "
icount=$((${icount}+1))
done
echo " ################# "
echo " size of g4pname = ${#g4pname[@]} "
echo " size of g4pval  = ${#g4pval[@]} "
for ((j=0; j<${#g4pname[@]}; ++j ))do
echo " for j = ${j}  ${g4pname[$j]}: ${g4pval[$j]} "  
done
done






