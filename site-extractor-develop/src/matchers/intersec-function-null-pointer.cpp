/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "intersec-function-null-pointer.hpp"
#include "matcher.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

#define CONDITION hasCondition(anyOf(binaryOperator(anyOf(hasOperatorName("=="), hasOperatorName("!=")),hasLHS(ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(isAnyPointer())))))),hasRHS(ignoringParenImpCasts(has(expr(nullPointerConstant()))))),ignoringParenImpCasts(declRefExpr(to(varDecl(hasType(isAnyPointer())))))))

using namespace std;
using namespace clang;
using namespace clang::ast_matchers;


void
IntersecFunctionNullPointerMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    compoundStmt(
      hasParent(
        functionDecl(
          hasDescendant(
            ifStmt(CONDITION).bind("stmt")
          )
        ).bind("fd")
      )
    ),
  this);
}

void
IntersecFunctionNullPointerMatcher::run(const MatchFinder::MatchResult &result)
{
  const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("stmt");
  const FunctionDecl *fd = result.Nodes.getNodeAs<FunctionDecl>("fd");

  if (stmt && fd) {
    std::string descr = "Statement with null pointer inside condition. Look inside: \"";

    Matcher::printIntersec(result, fd, stmt, descr);
  }
}