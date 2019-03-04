/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "matcher.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;
using namespace std;

string
process(string const &s)
{
    string::size_type pos = s.find('\n');
    if (pos != string::npos)
    {
        return s.substr(0, pos);
    }
    else
    {
        return s;
    }
}

void
Matcher::print(const MatchFinder::MatchResult &result,
              const Stmt *stmt, const string descr) const
{
  // Get node's string value
  string temp;
  raw_string_ostream str_o(temp);
  stmt->printPretty(str_o, 0, clang::PrintingPolicy(clang::LangOptions()));

  // Get source location
  FullSourceLoc fullLoc(stmt->getBeginLoc(), *result.SourceManager);
  const string &fileName = result.SourceManager->getFilename(fullLoc);
  const unsigned int lineNum = fullLoc.getSpellingLineNumber();

  // Print results
  if (fileName != "") {
      cout  << fileName
            << "@"
            << lineNum
            << ":"
            << descr
            << process(str_o.str())
            << "\"\n";
  }
}

void
Matcher::printIntersec(const MatchFinder::MatchResult &result, const FunctionDecl *fd,
              const Stmt *stmt, const string descr) const
{
  ifstream file("~/Desktop/prepSatese/interestingFunctionsForSatese.txt");
  string str;

  // Get stmt's string value
  string temp;
  llvm::raw_string_ostream str_o(temp);
  stmt->printPretty(str_o, 0, clang::PrintingPolicy(clang::LangOptions()));

  // Get source location
  FullSourceLoc fullLoc(stmt->getBeginLoc(), *result.SourceManager);
  const string &fileName = result.SourceManager->getFilename(fullLoc);
  const unsigned int lineNum = fullLoc.getSpellingLineNumber();
  string functionNameComma;
  functionNameComma = fd->getNameInfo().getAsString() + ",";

  // Print results
  if (fileName != "" && fileName.find("packet-") != string::npos) {
    while (getline(file, str))
    {
      if (str.find(functionNameComma) != string::npos && str.find(fileName) != string::npos) {
       cout  << fileName
              << ": "
              << fd->getNameInfo().getAsString()
              << " "
              << lineNum
              << ": "
              << descr
              << " "
              << process(str_o.str())
              << "\"\n";
      }
    }
  }
}
