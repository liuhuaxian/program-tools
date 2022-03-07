add_library(main main.c)
install(TARGETS main DESTINATION bin)
##cmake . -DCMAKE_INSTALL_PREFIX=`pwd`;make;make install
