# Use the magic that is @art_make@ to construct the shared object files for
# this whole directory. 

art_make( LIBRARIES ${allG4Libs})
# Install header files into the products area
install_headers() 