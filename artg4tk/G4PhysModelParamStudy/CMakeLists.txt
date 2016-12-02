#
# Do NOT use WERROR, or all warnings will be treated as errors !!!
#
cet_set_compiler_flags( DIAGS CAUTIOUS EXTRA_FLAGS -pedantic )
#
add_subdirectory( G4Components )
add_subdirectory( G4Services )
#
add_subdirectory( AnalysisProducts )
#
add_subdirectory( RooMUTools )
add_subdirectory( AnalysisBase )
add_subdirectory( AnalysisModules )
add_subdirectory( StatAnalysisTools )
add_subdirectory( ApplicationModules )
