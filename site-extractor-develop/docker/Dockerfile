FROM ubuntu:trusty
MAINTAINER Guillaume Haben <guillaume.haben@nist.gov>

# Enable future toolchain PPA
RUN apt-get update \
 && apt-get install -y software-properties-common git \

# Install pre-reqs
 && apt-get update \
 && apt-get install -y \
  build-essential 

# Obtain Clang/LLVM source code
WORKDIR /clang-llvm
RUN git clone --depth 1 https://github.com/llvm-mirror/llvm.git
WORKDIR /clang-llvm/llvm/tools
RUN git clone --depth 1 https://github.com/llvm-mirror/clang
WORKDIR /clang-llvm/llvm/tools/clang/tools
RUN git clone --depth 1 https://github.com/llvm-mirror/clang-tools-extra extra

# Setup Clang/LLVM build system
##Python
RUN add-apt-repository ppa:jonathonf/python-3.6
RUN apt-get update
RUN apt-get install -y python3.6
RUN python3 -V

## Ninja
WORKDIR /clang-llvm
RUN git clone https://github.com/martine/ninja.git
WORKDIR /clang-llvm/ninja
RUN git checkout release
RUN python3 ./configure.py --bootstrap
RUN cp ninja /usr/bin/

## Cmake
WORKDIR /clang-llvm
RUN git clone https://github.com/Kitware/CMake
WORKDIR /clang-llvm/CMake
RUN ./bootstrap
RUN make
RUN make install

## Clang
WORKDIR /clang-llvm/build
RUN cmake -G Ninja ../llvm -DCMAKE_BUILD_TYPE=Release \
                       -DCLANG_INCLUDE_DOCS=ON \
                       -DLLVM_BUILD_DOCS=ON \
                       -DLLVM_INCLUDE_DOCS=ON \
                       -DLLVM_BUILD_TESTS=ON
                       
# !!!! The following step must be done manually inside a running container because the build usually fails a few time before working.
#RUN ninja -k 20 -j 1
#RUN ninja check       # Test LLVM only.
#RUN ninja clang-test  # Test Clang only.
#RUN ninja install
