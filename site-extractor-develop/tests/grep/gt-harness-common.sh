#!/bin/bash
#
# Usage: gt-harness.sh COMMAND [OPTIONS]
#
# Support script for running (build/test) PROJECT as a benchmark program.
# (Canonical documentation in https://git.grammatech.com/benchmark/meta)
#
# Commands:
#  help --------------- Print these comments and usage information.
#  docker [IMAGE [BASE [DIST]]]
#                       Build docker image for PROJECT.
#                       IMAGE names the docker image.
#                       BASE names a base image to use.
#                       DIST names a Linux distribution (ubuntu|arch).
#  prep --------------- Prep PROJECT for building (e.g. ./configure).
#  prep-tests --------- Prepare for tests to be run (OPTIONAL)
#  clean -------------- Clean the project
#  clean-tests -------- Clean test artifacts from the project (OPTIONAL)
#  build [NAME] ------- Build the project
#                       Optional NAME for repositories w/multiple targets
#                       NOTE: build should not assume 'prep' has been run
#  build-oss-harnesses [NAME]
#                       Build OSS style harnesses (if exist)
#  binaries ----------- List the paths to the compiled binaries
#                       (Often one executable, possibly w/libraries)
#  source ------------- List the paths to the source files
#  num-tests ---------- Show the number of tests available
#  test PATH [NUM [WRAPPER]]
#                       Execute test NUM on the binary at PATH wrapped with the
#                       wrapper WRAPPER.
#                       Return zero on success, non-zero on failure.
#                       Optional NUM only runs test number NUM (0 to num-tests-1)
#                       If PATH contains {}, it is considered a pattern and will
#                       be used for each binary listed in 'binaries' with the
#                       original name in place of the {}.
#  version ------------ Show the version of the test harness
#  upgrade ------------ Upgrade this test harness
#

# Make sure we're in the project directory (the one containg gt-harness.sh)
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
HARNESS_URL=https://git.grammatech.com/benchmark/meta/raw/master/gt-harness-common.sh
HARNESS_PREPPED=./.gt-harness-prepped
help(){
    cat "gt-harness-common.sh" \
        |sed -E '/^[^#]|^$/q' \
        |sed "s/PROJECT/${PROJECT}/" \
        |head -n -1 \
        |tail -n +3 \
        |sed -e :a -e '/^\n*$/{$d;N;ba' -e '}' \
        |cut -c3- >&2
    exit 1;
}

# Declaration of variables with default values.
HARNESS_VERSION=0.0.0
PROJECT=`basename $(pwd)`
DOCKERFILE=Dockerfile.gt
IMAGENAME=gt-harness-$PROJECT
DOCKERBASE=ubuntu:16.04
declare -a APT_GET_PACKAGES
APT_GET_PACKAGES+=(make)
APT_GET_PACKAGES+=(build-essential)
APT_GET_PACKAGES+=(autoconf)
APT_GET_PACKAGES+=(libtool)
declare -a PACMAN_PACKAGES
PACMAN_PACKAGES+=(base-devel)
declare -a DOCKER_CUSTOM_UBUNTU
declare -a DOCKER_CUSTOM_ARCH

check_versions(){
  # TODO - check that major version of PRJ_HARNESS_VERSION == major version of
  # HARNESS_VERSION.
  # We MAY want to more aggressively check for upgrades via the following rules.
  # If >, then look for an upgrade and upgrade immediately.
  # If <, then allow, but warn the user.
  # If ==, then continue silently.
  # Use "$PRJ_HARNESS_VERSION" and  "$HARNESS_VERSION"
  :
}

build_docker(){
    # Set the IMAGE name for the docker image.
    if [ ! -z $1 ];then
        IMAGENAME=$1
        shift
    fi
    # Set the BASE of the docker image.
    if [ ! -z $1 ];then
        if [ $1 == "arch" ] || [ $1 == "Arch" ];then
            # We special case a value of "arch" or "Arch"
            # as the second argument making it easy to
            # both (i) build from the Arch base image and
            # (ii) set the distribution type to Arch with
            # a single argument.  This re-use of the second
            # argument why this branch has no shift.
            DOCKERBASE=base/archlinux
        else
            DOCKERBASE=$1
            shift
        fi
    fi
    # Build the file based on the Linux distribution
    if [ ! -z $1 ];then
        DIST=$1
        shift
    else
        DIST=ubuntu
    fi
    # Write the Dockerfile.
    (
        # Dockerfile: Prelude loads from $DOCKERBASE image.
        cat <<EOF
FROM $DOCKERBASE

EOF
        # Dockerfile: Distribution specific package installation.
        case $DIST in
            ubuntu*|Ubuntu*)
                cat <<EOF
RUN apt-get update -y
RUN apt-get install -y --no-install-recommends ${APT_GET_PACKAGES[@]}
${DOCKER_CUSTOM_UBUNTU[@]}
EOF
                ;;
            arch*|Arch*)
                cat <<EOF
RUN pacman --noconfirm -Syu archlinux-keyring
RUN pacman --noconfirm -Syu ${PACMAN_PACKAGES[@]}
${DOCKER_CUSTOM_ARCH[@]}
EOF
                ;;
            *)
                echo "Distribution $DIST not supported." >&2
                exit 1
        esac
        # Dockerfile: Postlude copies in project directory.
        cat <<EOF

# Copy the contents of this repository into /gt/PROJECT.
RUN mkdir -p /gt/
COPY . /gt/$PROJECT

WORKDIR /gt/$PROJECT

CMD /bin/bash
EOF
    ) > $DOCKERFILE
    # Build the docker image.
    docker build -t $IMAGENAME -f $DOCKERFILE .
}

# [...]

missing_okay(){
    local TMP=$(mktemp)
    $1 2>${TMP}
    ERRNO=$?
    if [ $ERRNO -eq 127 ];then
        if $(grep -q "$1: command not found" ${TMP});then
            echo "Command '$1' missing, but that's okay."
            ERRNO=0
        else
            cat ${TMP}
        fi
    else
        cat ${TMP}
    fi
    rm -f ${TMP}
    return ${ERRNO}
}

run_prep(){
  touch $HARNESS_PREPPED

  pushd $DIR >/dev/null 2>/dev/null
  prep
  ret=$?
  popd >/dev/null 2>/dev/null

  return $ret
}

run_clean(){
  rm -f $HARNESS_PREPPED

  pushd $DIR >/dev/null 2>/dev/null
  clean
  ret=$?
  popd >/dev/null 2>/dev/null

  return $ret
}

run_build(){
  pushd $DIR >/dev/null 2>/dev/null
  build
  ret=$?
  popd >/dev/null 2>/dev/null

  return $ret
}

# Copy file or directory from $1 to $2.
# Skip if they're already the same.
test_copy_file(){
  if [[ ( -f "$1" ) && ( -f "$2" ) && $(cmp "$1" "$2") ]];
  then
    cp -pf "$1" "$2"
  elif [[ ( ! -f $1 || ! -f $2 ) && ( -e $1 ) && ( ! $1 -ef $2 ) ]];
  then
    rm -rf "$2"
    cp -pr "$1" "$2"
  fi
}

firstword(){
  echo $1
}

# Prepares to run the tests.  Usually by copying the original to a backup, and
# replacing it with the target.
# Arguments: $1 - target binary to test
# Override by re-defining in gt-harness.sh project specific file
# TODO - workout multiple binary tests
test_begin(){
  if [[ $1 == *{}* ]]; then
    for bin in $(binaries); do
      new=${1/\{\}/$(basename $bin)}
      test_copy_file "$DIR/$bin" "$DIR/$bin.bak"
      test_copy_file "$new" "$DIR/$bin"
    done
  else
    local orig=$(firstword $(binaries))
    test_copy_file "$DIR/$orig" "$DIR/$orig.bak"
    test_copy_file "$1" "$DIR/$orig"
  fi
}

# Cleanup after running the tests.  Usually by restoring the orignal binary(s)
# from backup.
# Arguments: None
# Override by re-defining in gt-harness.sh project specific file
test_end(){
  if [[ $1 == *{}* ]]; then
    for bin in $(binaries); do
      rm -rf "$DIR/$bin"
      mv "$DIR/$bin.bak" "$DIR/$bin" > /dev/null 2>&1
    done
  else
    local orig=$(firstword $(binaries))
    rm -rf "$DIR/$orig"
    mv "$DIR/$orig.bak" "$DIR/$orig" > /dev/null 2>&1
  fi
}

run_test(){
  pushd $DIR >/dev/null 2>/dev/null
  test $@
  ret=$?
  popd >/dev/null 2>/dev/null

  return $ret
}

gt-harness-main(){
  ## check compatibility and for upgrades
  check_versions

  if [ $# -lt 1 ];
      then help;
  fi

  KEY="$1"
  shift
  case "$KEY" in
      help)
          help;;
      docker)
          build_docker $@;;
      prep)
          run_prep;;
      clean)
          run_clean;;
      clean-tests)
          missing_okay clean_tests;;
      build|${PROJECT})
          if [ ! -z $1 ];then
              if [ ! $1 == "${PROJECT}" ];then
                  echo "Can only build '${PROJECT}' (not '$1')" >&2
                  exit 1
              fi
          fi
          if [ ! -f $HARNESS_PREPPED ]; then
              run_prep
          fi
          run_build;;
      build-oss-harness|${PROJECT}_oss_harness)
          if [ ! -z $1 ];then
              if [ ! $1 == "${PROJECT}_oss_harness" ];then
                  echo "Can only build '${PROJECT}_oss_harness' (not '$1')" >&2
                  exit 1
              fi
          fi
          build_oss_harness;;
      binaries)
          binaries;;
      source)
          source;;
      num-tests)
          num_tests;;
      prep-tests)
          missing_okay prep_tests;;
      test)
          if [ -z $2 ];then
              # Run the whole test suite.
              ERRNO=0
              NUM_TESTS=$(num_tests)
              if [ -z $1 ]; then
                echo "WARNING: No binaries specified, running with files in place." >&2
              else
                test_prelude=yes
                test_begin $1
              fi
              for i in $(seq 0 $((NUM_TESTS - 1)));do
                  run_test $i
                  RES=$?
                  if [ $RES -eq 0 ];then
                      echo -e "GT_PASS\t$i" >&2
                  else
                      echo -e "GT_FAIL\t$i\t$RES" >&2
                  fi
                  # Max of RES and ERRNO.
                  ERRNO=$(($ERRNO>$RES?$ERRNO:$RES))
              done
              if [ ! -z $test_prelude ]; then
                test_end $1
              fi
              exit $ERRNO
          else
              # Run a specific test.
              test_begin $1
              shift
              run_test $@
              RES=$?
              test_end $1
              exit $RES
          fi;;
      version)
        echo $HARNESS_VERSION;;
      upgrade)
        mv gt-harness-common.sh gt-harness-common.sh.bak;
        wget $HARNESS_URL && \
            rm gt-harness-common.sh.bak || \
                mv gt-harness-common.sh.bak gt-harness-common.sh;;
      *)
          echo -e "Unrecognized command \"${KEY}\".\n" >&2
          help
          ;;
  esac
}
