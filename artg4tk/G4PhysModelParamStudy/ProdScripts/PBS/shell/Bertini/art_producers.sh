#!/usr/bin/env bash
#

cfg_generator () {

/usr/bin/printf " \n"
/usr/bin/printf "      PrimaryGenerator: { \n"
/usr/bin/printf "         module_type: EventGenerator \n"
/usr/bin/printf "         nparticles : 1 \n"
/usr/bin/printf "         pdgcode: ${pdgcode} \n"
/usr/bin/printf "         momentum: [ 0., 0., ${momz} ] // in GeV \n" 
/usr/bin/printf "      } \n"

}

cfg_g4model_parameters() {

if [ "x" == "x${1}" ]; then
/usr/bin/printf "               ${pname}: ${pval} \n"
else
for ((j=0; j<${#g4pname[@]}; ++j ))do
/usr/bin/printf "               ${g4pname[$j]}: ${g4pval[$j]} \n"  
done
fi

}

cfg_g4producer () {

/usr/bin/printf " \n"
/usr/bin/printf "      G4${g4label}: { \n"
/usr/bin/printf "         module_type: ModelParamStudyProducer \n"
/usr/bin/printf "         RNDMSeed: ${seed} \n"
# --> no longer needed since we are using GDML
# /usr/bin/printf "         TargetGeom: { \n"
# /usr/bin/printf "            targetSize: [ 0., 3.15, 160. ] \n"
# /usr/bin/printf " 	     targetPosition: [ 0, 0, 0 ] \n"
# /usr/bin/printf "	     targetMaterial: \"Pb\" \n"
# /usr/bin/printf "	     targetShape: \"G4Tubs\" \n"
# /usr/bin/printf "         } \n"
# -->
/usr/bin/printf "  	  Verbosity: 0 \n"
/usr/bin/printf "	  HadronicModelParameters: { \n"
/usr/bin/printf "	     DefaultPhysics: ${defaults} \n"

#/usr/bin/printf "	     Bertini: { \n"
#/usr/bin/printf "	        RadiusScaleByRatio: 0.75 \n"
#/usr/bin/printf "	     } \n"

/usr/bin/printf "	  } \n"

/usr/bin/printf "      } \n"

}

cfg_g4processlevel() {

/usr/bin/printf " \n"
/usr/bin/printf "      G4${proc_level}${g4label}: { \n"
/usr/bin/printf "         module_type: ProcLevelMPVaryProducer \n"
/usr/bin/printf "         Verbosity: 0 \n"
/usr/bin/printf "         HadronicModel: { \n"
/usr/bin/printf "            DefaultPhysics: ${defaults} \n"
/usr/bin/printf "            ModelParameters: { \n"
if [ ! ${defaults} == "true" ]; then
cfg_g4model_parameters ${1}
fi
/usr/bin/printf "            } \n"
/usr/bin/printf "         } \n"
/usr/bin/printf "      } \n"

}
