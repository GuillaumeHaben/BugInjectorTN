/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/Path.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/Tooling/JSONCompilationDatabase.h>
#include <clang/Tooling/Tooling.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <llvm/ADT/Twine.h>

#include "matcher.hpp"
#include "critical-functions.hpp"
#include "cast-pointer.hpp"
#include "double-free.hpp"
#include "buffer-underflow.hpp"
#include "integer-overflow.hpp"
#include "buffer-write-array.hpp"
#include "buffer-write-functions.hpp"
#include "return-expr.hpp"
#include "while-loop-null-pointer-condition.hpp"
#include "input.hpp"
#include "inputprof2.hpp"
//#include "intersec-function-null-pointer.hpp"
//#include "intersec-function-null-pointer-init.hpp"
//#include "intersec-function-tvb_get_ptr.hpp"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;

static llvm::cl::opt<std::string> buildPath(
  "b",
  llvm::cl::value_desc("compile_commands.json"),
  llvm::cl::desc("Path to the compilation database (compile_commands.json). If "
                 "this argument is not passed, the compilation arguments can be "
                 "passed on the command line after '--'"),
  llvm::cl::Optional
);

static llvm::cl::list<std::string> sourcePaths(
  llvm::cl::Positional,
  llvm::cl::desc("<sources>* [-- <compile commands>]"),
  llvm::cl::ZeroOrMore
);

static llvm::cl::opt<bool> processAllSources(
  "a",
  llvm::cl::desc("Process all files from the compile_command.json. If this "
                 "argument is passed, the list of sources does not need to be "
                 "passed")
);

int
main(int argc, const char *argv[])
{

  /* -------------------- */
  /* ----- Options ------ */
  /* -------------------- */
  std::string err = "err";
  const Twine &Tmp = ".";

  std::unique_ptr<clang::tooling::CompilationDatabase> compilations(
    FixedCompilationDatabase::loadFromCommandLine(argc, argv, err, Tmp));

  llvm::cl::ParseCommandLineOptions(argc, argv);

  if (!compilations && llvm::sys::fs::exists(buildPath)) {
    std::string errorMessage;
    if (llvm::sys::fs::is_directory(buildPath)) {
      compilations = std::unique_ptr<CompilationDatabase>(
        CompilationDatabase::loadFromDirectory(buildPath, errorMessage));
    } else {
      compilations = std::unique_ptr<CompilationDatabase>(
        JSONCompilationDatabase::loadFromFile(
                                              buildPath,
                                              errorMessage,
                                              JSONCommandLineSyntax::AutoDetect));
    }
    if (!errorMessage.empty()) {
      std::cerr << errorMessage << std::endl;
    }
  }

  if (!compilations) {
    std::cerr << "Could not load the compilation database. Please use the -b option "
                 "to a path containing a compile_commands.json, or use '--' followed"
                 "by the compilations commands."
              << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<std::string> dirContents;
  std::vector<std::string> allFiles = compilations->getAllFiles();
  std::sort(allFiles.begin(), allFiles.end());
  llvm::ArrayRef<std::string> sources = sourcePaths;

  if (sources.empty() && processAllSources) {
    // Process all sources from the compilation database
    sources = allFiles;
  } else if (processAllSources) {
    std::cerr << "Cannot use both sources and '-a' option." << std::endl;
    return EXIT_FAILURE;
  } else if (sources.size()==1 && llvm::sys::fs::is_directory(sources.front())) {
#if CLANG_VERSION_MAJOR != 3 || CLANG_VERSION_MINOR >= 5
    // A directory was passed, process all the files in that directory
    llvm::SmallString<128> dirName;
    llvm::sys::path::native(sources.front(), dirName); // transforms the path in native OS format
    while (dirName.endswith("/"))
      dirName.pop_back();
    std::error_code EC;
    for (llvm::sys::fs::recursive_directory_iterator it(dirName.str(), EC), dirEnd;
         it!= dirEnd && !EC;
         it.increment(EC))
    {
      if (llvm::sys::path::filename(it->path()).startswith(".")) {
        it.no_push();
        continue;
      }
      dirContents.push_back(it->path());
    }
    sources = dirContents;
    if (EC) {
      std::cerr << "Error reading the directory: " << EC.message() << std::endl;
      return EXIT_FAILURE;
    }
#else
    std::cerr << "Passing directory is only implemented with llvm >= 3.5" << std::endl;
    return EXIT_FAILURE;
#endif
  }

  if (sources.empty()) {
    std::cerr << "No source files. Please pass source files as argument, or use the "
                 "'-a' option."
              << std::endl;
  }

  /* ------------------------ */
  /* ----- Matcher init ----- */
  /* ------------------------ */

  ClangTool tool(*compilations, sources);

  /* ----- Matchers ----- */
  CriticalFunctionsMatcher cf_matcher;
  CastPointerMatcher cp_matcher;
  DoubleFreeMatcher df_matcher;
  WhileLoopNullPointerConditionMatcher wnp_matcher;
  BufferUnderflowMatcher buf_matcher;
  /* ----- Intersec's matchers ----- */
  /*
  IntersecFunctionNullPointerMatcher ifnp_matcher;
  IntersecFunctionNullPointerInitMatcher ifnpi_matcher;
  IntersecFunctionTvbGetPtrMatcher ift_matcher;
  */
  /* ----- Okham's matchers ----- */

//  ReturnExpressionMatcher re_matcher;
//  IntegerOverflowMatcher int_matcher;
  BufferWriteArrayMatcher bwa_matcher;
//  BufferWriteFunctionsMatcher bwf_matcher;
  InputMatcher inp_matcher;
  prof2Matcher inp2_matcher;

  MatchFinder finder;

  cf_matcher.registerASTMatcherCallback(finder);
  cp_matcher.registerASTMatcherCallback(finder);
  df_matcher.registerASTMatcherCallback(finder);
  wnp_matcher.registerASTMatcherCallback(finder);
  buf_matcher.registerASTMatcherCallback(finder);
  /* ----- Intersec's matchers ----- */
  /*
  ifnp_matcher.registerASTMatcherCallback(finder);
  ifnpi_matcher.registerASTMatcherCallback(finder);
  ift_matcher.registerASTMatcherCallback(finder);
  */

  /* ----- Okham's matchers ----- */

//  re_matcher.registerASTMatcherCallback(finder);
//  int_matcher.registerASTMatcherCallback(finder);
  bwa_matcher.registerASTMatcherCallback(finder);
//  bwf_matcher.registerASTMatcherCallback(finder);
  inp_matcher.registerASTMatcherCallback(finder);
  inp2_matcher.registerASTMatcherCallback(finder);


  // Launch*/
  return tool.run(newFrontendActionFactory(&finder).get());
}
