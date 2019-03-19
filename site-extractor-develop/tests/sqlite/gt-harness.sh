#!/bin/bash
#
# Project specific code to be used with gt-harness-common.sh.  For
# documentation, refer to comments here and in the gt-harness-common.sh file.
# If not present locally, it will be retrived via wget as seen below.

#
# Bootstrap code from template. DO NOT CHANGE
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [ ! -f $DIR/gt-harness-common.sh ]; then
  wget -O $DIR/gt-harness-common.sh \
       https://git.grammatech.com/benchmark/meta/raw/master/gt-harness-common.sh;
fi
. $DIR/gt-harness-common.sh

#############################################################################
## Project specific - start

# Project specific variable overrides follow this comment.
#
# Project name, defaults to last component of CWD.
PROJECT=sqlite3
#
# Harness version requirement for this project
PRJ_HARNESS_VERSION=0.0.0

## For 'docker' command
#
# Array of packages to install in ubuntu based images.
# Defaults to make, build-essential autoconf libtool
# APT_GET_PACKAGES+=(libfoo)
APT_GET_PACKAGES+=(sqlite3)
APT_GET_PACKAGES+=(tcl)
APT_GET_PACKAGES+=(indent)
APT_GET_PACKAGES+=(coreutils)
#
# Array of packages to install in arch based images
# Defaults to base-devel
PACMAN_PACKAGES+=(sqlite)
PACMAN_PACKAGES+=(tcl)
PACMAN_PACKAGES+=(coreutils)
#
# Local name of created temporary Dockerfile.  Defaults to "Dockerfile.gt"
# DOCKERFILE=Dockerfile.gt
#
# Default image name of created docker image.  Defaults to gt-harness-$PROJECT.
# IMAGENAME=my-docker-image-name

# Prepares for the build, such as ./configure.  Typically done prior to using
# the build step for DVT.
# Arguments: None
prep(){
    cd $DIR 1>/dev/null 2>/dev/null
    if [ ! -f $DIR/sqlite3.c ]; then
        $DIR/configure
        make sqlite3.c
    fi
    cd - 1>/dev/null 2>/dev/null
}

# Builds some or all targets.  Assume prep has been run and only perform build
# steps.  Common harness will run prep if not already run by managing
# .gt-harness-prepped.
# Arguments: $1 - target to build
build(){
    prep

    cd $DIR 1>/dev/null 2>/dev/null
    ret=0
    make $PROJECT CC=clang CFLAGS=-O0
    ret=$?
    cd - 1>/dev/null 2>/dev/null

    return $ret
}

# Cleans the build environment, such as make clean
# Arguments: None
clean() {
    cd $DIR 1>/dev/null 2>/dev/null
    make distclean
    cd - 1>/dev/null 2>/dev/null
}

# Lists the output location of each build project binary.  The first one listed
# is the primary binary (the one you would run).
# Arguments: None
binaries(){
    echo $PROJECT
}

# Lists the main source files in the repository.
# Arguments: None
source(){
    find . -type f         \
         -name "*.c"       \
         -or -name "*.h"   \
         -or -name "*.cpp" \
         -or -name "*.hpp" \
         -or -name "*.cxx" \
         -or -name "*.hxx"
}

# Shows the number of tests available.  Tests are identified by numeric ID in
# the range of 1 to num-tests.
# Arguments: None
num_tests(){
    echo "select count(*) from xsql;" | sqlite3 test/fuzzdata1.db
}

# Runs a single test.  The common gt-harness code handles iteration, so
# an invocation should run the indicated test only.
#
# Tests should assume the target binary to be tested is in the standard build
# output location at this point.  See test_begin and test_end in
# gt-harness-common.sh for details and override instructions if something
# different is needed.
#
# Arguments: $1 - the id of the test to run
test(){
    TMP1=$(mktemp)
    TMP2=$(mktemp)

    cp $DIR/test/fuzzdata1.db $TMP1
    echo "select sqltext from xsql where sqlid = $1;" | sqlite3 $TMP1 > $TMP2
    timeout 20s ./sqlite3 $TMP1 < $TMP2 1>/dev/null 2>/dev/null
    rm $TMP1 $TMP2
}

## Project specific - end
#############################################################################

#
# Call main processing in gt-harness-common.sh
gt-harness-main $@
