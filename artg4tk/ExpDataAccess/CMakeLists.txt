# services CMakeLists
#
# Do NOT use WERROR, or all warnings will be treated as errors !!!
#
cet_set_compiler_flags(DIAGS CAUTIOUS
EXTRA_FLAGS -pedantic
)
#    art_make( LIB_LIBRARIES  "${XERCESCLIB}" "${G4_LIB_LIST}" )
art_make( LIB_LIBRARIES  -lcurl )

# install_headers()
