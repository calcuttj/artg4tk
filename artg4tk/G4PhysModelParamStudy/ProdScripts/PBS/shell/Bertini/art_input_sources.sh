#!/usr/bin/env bash
#

# NOTE: one can also say "function cfg_empty_source()" 
#       - it does not matter
#
cfg_empty_source () {

#output=${1}

/usr/bin/printf " \n"
/usr/bin/printf "source: { \n" 
/usr/bin/printf "   module_type: EmptyEvent \n" 
/usr/bin/printf "   maxEvents: ${maxevents} \n" 
/usr/bin/printf "} \n" 

}

cfg_rootinput_source () {

/usr/bin/printf " \n"
/usr/bin/printf "source: { \n" 
/usr/bin/printf "   module_type: RootInput \n" 
/usr/bin/printf "   maxEvents: -1 \n" 
/usr/bin/printf "   fileNames: [ \"fname1.root\", \"fname2.root\" ] \n"
/usr/bin/printf "} \n" 

}
