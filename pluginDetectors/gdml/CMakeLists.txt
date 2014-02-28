# Use the magic that is @art_make@ to construct the shared object files for
# this whole directory. 

art_make( LIB_LIBRARIES artg4tk_pluginActions_particleGun_ParticleGunAction_service  ${G4_LIB_LIST} ${XERCESCLIB})

# Install header files into the products area
install_headers() 