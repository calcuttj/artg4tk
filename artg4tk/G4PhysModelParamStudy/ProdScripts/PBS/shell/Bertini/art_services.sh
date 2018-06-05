#!/usr/bin/env bash
#

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
/usr/bin/printf " TFileService: { fileName: \"${ts_filename}.root\" } \n"
fi

/usr/bin/printf "   RandomNumberGenerator: {} \n"

# check if either process-level or full-scale app
# but not both at the same time
#
if [ "x" == "x$gdmlfile" ] && [ "x" == "x$proc_level" ]; then
echo " both process-level and full-scale apps activated; bail out "
exit 3
fi

if [ ! "x" == "x$gdmlfile" ]; then
/usr/bin/printf "// GDML-based geometry \n"
/usr/bin/printf "// \n"
/usr/bin/printf "   DetectorHolder: {} \n"
/usr/bin/printf "   GDMLDetector: { \n"
/usr/bin/printf "      category: \"world\" \n"
/usr/bin/printf "      gdmlFileName_: \"${gdmlfile}\" } \n"   
fi

if [ ! "x" == "x$proc_level" ]; then
/usr/bin/printf "   ProcLevelSimSetup: { \n"
/usr/bin/printf "      HadronicModelName: \"${proc_level}\" \n"
if [ "x" == "x$target" ]; then
echo "Target nucleus is NOT specified for process-level simulation; bail out"
exit 3
fi
/usr/bin/printf "      TargetNucleus: \"${target}\" \n"
/usr/bin/printf "      RNDMSeed: ${seed} \n"
/usr/bin/printf "   } \n"
fi

if [ ! "x" == "x$physlist" ]; then 
/usr/bin/printf "   PhysicsListHolder: {} \n"
/usr/bin/printf "   PhysicsList: { PhysicsListName: \"${physlist}\" } \n"
fi

/usr/bin/printf "   PhysModelConfig: { Verbosity: true } \n"

/usr/bin/printf "} \n" 

}
