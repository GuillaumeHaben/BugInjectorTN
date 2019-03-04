/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#pragma once

#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace std;

namespace clang
{

  namespace ast_matchers
  {
    class MatchFinder;
  } // namespace ast_matchers

} // clang

class Matcher
{
public:
  virtual void registerASTMatcherCallback(clang::ast_matchers::MatchFinder &finder)
  {}

  void print(const clang::ast_matchers::MatchFinder::MatchResult &result,
                const clang::Stmt *stmt, const string descr) const;

  void printIntersec(const clang::ast_matchers::MatchFinder::MatchResult &result, const clang::FunctionDecl *fd, const clang::Stmt *stmt, const string descr) const;
};
