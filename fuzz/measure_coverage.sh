#!/bin/sh
#
# makes a coverage build.
#
# To measure and display the coverage:
#
#cd build-coverage
#fuzz/fuzz_parser path/to/corpus/* # repeat with other fuzzers
#gcovr -r . --html --html-details --sort-uncovered -o out.html
# and view the results in out.html

bdir=build-coverage
if [ ! -d $bdir ] ; then
    mkdir -p $bdir
    cd $bdir

    export CC=gcc
    export CXX="g++"
    export CFLAGS="-fprofile-arcs -ftest-coverage"
    export CXXFLAGS="-fprofile-arcs -ftest-coverage"
    export LDFLAGS="-fprofile-arcs -ftest-coverage"

    cmake .. \
          -GNinja \
          -DCMAKE_BUILD_TYPE=Debug \
          -DSIMDJSON2_DEVELOPER_MODE=ON \
          -DBUILD_SHARED_LIBS=OFF \
          -DSIMDJSON2_ENABLE_FUZZING=On \
          -DSIMDJSON2_DISABLE_DEPRECATED_API=On \
	  -DSIMDJSON2_FUZZ_LINKMAIN=On
    ninja all_fuzzers
    cd ..
fi



