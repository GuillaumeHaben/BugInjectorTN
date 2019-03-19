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
PROJECT=grep
#
# Harness version requirement for this project
PRJ_HARNESS_VERSION=0.0.0

## For 'docker' command
#
# Array of packages to install in ubuntu based images.
# Defaults to make, build-essential autoconf libtool
APT_GET_PACKAGES+=(coreutils)
#
# Array of packages to install in arch based images
# Defaults to base-devel
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
prep() {
    touch grep.c
}

# Cleans the build environment, such as make clean
# Arguments: None
clean() {
    make clean
}

# Builds some or all targets.  Assume prep has been run and only perform build
# steps.  Common harness will run prep if not already run by managing
# .gt-harness-prepped.
# Arguments: $1 - target to build
build(){
    if [ -z ${CC} ]
    then
        CC="gcc"
    fi

    make $PROJECT CC="$CC" CFLAGS="$CFLAGS"
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
    echo 199
}

# Runs a single test.  The common gt-harness code handles iteration, so
# an invocation should run the indicated test only.
#
# Tests should assume the target binary to be tested is in the standard build
# output location at this point.  See test_begin and test_end in
# gt-harness-common.sh for details and override instructions if something
# different is needed.
test(){
    local test_stdout=$(mktemp)
    local test_stderr=$(mktemp)
    local oracle_stdout=$(mktemp)
    local oracle_stderr=$(mktemp)

    timeout 20s $DIR/support/test.sh $DIR/grep $@ 1>$test_stdout 2>$test_stderr
    timeout 20s $DIR/support/test.sh grep $@ 1>$oracle_stdout 2>$oracle_stderr

    local out_diff=0
    local err_diff=0

    diff $test_stdout $oracle_stdout 1>/dev/null 2>/dev/null
    out_diff=$?
    diff $test_stderr $oracle_stderr 1>/dev/null 2>/dev/null
    err_diff=$?

    rm $test_stdout
    rm $test_stderr
    rm $oracle_stdout
    rm $oracle_stderr

    if [[ $errdiff -eq 0 && $outdiff -eq 0 ]];
    then
        return 0
    else
        return 1
    fi
}

## Project specific - end
#############################################################################

#
# Call main processing in gt-harness-common.sh
gt-harness-main $@
