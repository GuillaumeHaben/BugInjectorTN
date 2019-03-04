/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "intersec-function-null-pointer-init.hpp"
#include "matcher.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;


void
IntersecFunctionNullPointerInitMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
        functionDecl(
          hasDescendant(
            stmt(hasDescendant(expr(nullPointerConstant()))).bind("stmt")
          )
        ).bind("fd"),
  this);
}

void
IntersecFunctionNullPointerInitMatcher::run(const MatchFinder::MatchResult &result)
{
  const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("stmt");
  const FunctionDecl *fd = result.Nodes.getNodeAs<FunctionDecl>("fd");

  if (stmt && fd) {
    std::string descr = "offset used. Look inside: \"";

    Matcher::printIntersec(result, fd, stmt, descr);
  }
}