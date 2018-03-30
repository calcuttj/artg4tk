# services CMakeLists
#
# Do NOT use WERROR, or all warnings will be treated as errors !!!
#
cet_set_compiler_flags(DIAGS CAUTIOUS
EXTRA_FLAGS -pedantic
)
#    art_make( LIB_LIBRARIES  "${XERCESCLIB}" "${G4_LIB_LIST}" )
art_make( LIB_LIBRARIES  -lcurl )
# ---> art_make( LIB_LIBRARIES  -L/usr/local/Anaconda3-5.0.1/lib -lcurl )

# install_headers()
