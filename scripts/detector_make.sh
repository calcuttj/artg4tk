#!/bin/bash 

# Make a detector service from templates

# Arguments: $1 = namespace
#            $2 = Class name (case is important)

namespace=$1
classname=$2

# Figure out where we are running from
thisDirAB=$(cd ${BASH_SOURCE[0]%/*} && echo $PWD/${BASH_SOURCE[0]##*/} )
thisDir=`dirname $thisDirAB`

upperClassName=$( echo $classname | tr '[:lower:]' '[:upper:]' )

fileName="${classname}_service"
headerName="${fileName}.hh"

# Make the cc file
sed -e "s/%%classNameAllCaps%%/$upperClassName/g"\
    -e "s/%%className%%/$classname/g"\
    -e "s/%%namespace%%/$namespace/g"\
    -e "s/%%headerName%%/$fileName/g"\
    $thisDir/Detector_service.cc_template > ${fileName}.cc
echo "Created ${fileName}.cc"

# Make the hh file
sed -e "s/%%classNameAllCaps%%/$upperClassName/g"\
    -e "s/%%className%%/$classname/g"\
    -e "s/%%namespace%%/$namespace/g"\
    $thisDir/Detector_service.hh_template > ${fileName}.hh
echo "Created ${fileName}.hh"

echo "Edit each file and look for CHANGE_ME"
