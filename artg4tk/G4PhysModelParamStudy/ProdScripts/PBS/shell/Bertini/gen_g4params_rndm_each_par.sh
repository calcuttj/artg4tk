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

config=g4params_bertini_rndm_each_par.sh

if [ -e ${config} ]; then
/bin/rm -f ${config}
fi

/usr/bin/printf "#!/usr/bin/env bash \n" >> ${config}
/usr/bin/printf "# \n" >> ${config}
/usr/bin/printf "BertiniParamsOneByOne=( \n" >> ${config}

for ((i=0; i<${#BertiniParRange[@]}; ++i)) do
pname=`echo ${BertiniParRange[$i]} | awk -F '=' '{print $1}'`
range=`echo ${BertiniParRange[$i]} | awk -F '=' '{print $NF}' | sed 's/(//g' | sed 's/)//g'`
rmin=`echo ${range} | awk -F ' ' '{print $1}'`
rmax=`echo ${range} | awk -F ' ' '{print $2}'`
/usr/bin/printf "'${pname}=(" >> ${config}
for j in `seq 100`; do
pval=`python -c "import random; print random.uniform($rmin,$rmax)"`
/usr/bin/printf "${pval} " >> ${config}
done
/usr/bin/printf ")' \n" >> ${config}
done
/usr/bin/printf ") \n" >> ${config}

/bin/chmod +x ${config}


. ${config}

for ((i=0; i<${#BertiniParamsOneByOne[@]}; ++i)) do
pname=`echo ${BertiniParamsOneByOne[$i]} | awk -F '=' '{print $1}' | sed 's/(//g'`
echo " pname = ${pname} "
content=`echo ${BertiniParamsOneByOne[$i]} | awk -F '=' '{print $NF}' | sed 's/(//g' | sed 's/)//g'`
for pval in ${content}; do
echo " --> ${pname}: ${pval} "
done
done






