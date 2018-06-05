#!/usr/bin/env bash
#

cfg_outputs () {

/usr/bin/printf " \n"
/usr/bin/printf "outputs: { \n"
/usr/bin/printf "   ${outlabel}: { \n"
/usr/bin/printf "      module_type: RootOutput \n"
/usr/bin/printf "      fileName: \"${outfile}.root\" \n" 
/usr/bin/printf "   } \n"
/usr/bin/printf "} \n"

}
