# SATESE - Static Analysis Tool Exposition Site Extractor
SATESE helps matching sites (locations in code where weaknesses might occur). It has been developed to speed up implementation of test cases for [SATE VI](https://samate.nist.gov/SATE.html).

## How to run

To setup this environment, follow this [tutorial](http://clang.llvm.org/docs/LibASTMatchersTutorial.html) to build llvm and clang from source or use the [Docker](./docker) image.

#### Prerequisites
SATESE is based on [Clang](https://clang.llvm.org) libraries [LibTooling](https://clang.llvm.org/docs/LibTooling.html) and [LibASTMatchers](http://clang.llvm.org/docs/LibASTMatchers.html) so you'll need to have the following envirronment (available in the Docker image):
* LLVM
* Clang
* CMake
* Ninja build system

#### Intall
You will just need to add `add_subdirectory(satese)` at the end of the `/clang-llvm/llvm/tools/clang/tools/extra/CMakeLists.txt`, so it will be built with LLVM/Clang.
The sources must be located in `/clang-llvm/llvm/tools/clang/tools/extra/satese`. (Current branch : `develop`)
Run the following command to build the project:
```sh
$ cd /clang-llvm/build
$ ninja satese
```
Link the binary to its proper location to avoid include failures during the execution: `ln -s /clang-llvm/build/bin/satese /usr/local/bin/satese`

#### Usage
###### SATESE on a single file
Once build, run the following command to get the matching sites of a c program:
```sh
$ satese path/to/your/testFile.c --
```
###### SATESE on a project
You need to compile your project with bear option to generate a `compile_database.json` file that will allow the site extractor to run through all the project.
Example with Wireshark:
```sh
$ cd your/project
$ ./configure --disable-wireshark --disable-glibtest --disable-dftest CC=clang # Using Clang as compiler
$ bear make -j 6 # To run on 6 cores
$ cd clang-llvm/build
$ satese -b your/project/compile_commands.json -a 2>/dev/null
```

## Matchers available

Here is the actual list of matchers:
  - Buffer underflow
  - Buffer write array
  - Buffer write functions
  - Cast pointer
  - Critical functions
  - Double free
  - Integer overflow
  - Return expr
  - While loop null pointer condition

## Development

Want to contribute? Great! Here is how to add a new matcher to the project:

* Copy/paste cpp and hpp files of an existing matcher and change their names
* Include this new matcher in `src/main.cpp` and add this matcher to the finder as are the other
* Add it to `src/CMakeList.txt`
* (Optional) Create a c program in `tests/` to test your matcher

## Authors

* Damien Cupif - [damien.cupif@nist.gov](damien.cupif@nist.gov)
* Guillaume Haben - [guillaume.haben@nist.gov](guillaume.haben@nist.gov)
