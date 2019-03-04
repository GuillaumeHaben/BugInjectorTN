/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "double-free.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
DoubleFreeMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    compoundStmt(
      anyOf(
        hasParent(
          whileStmt()
        ),
        hasParent(
          forStmt()
        ),
        hasParent(
          doStmt()
        )
      ),
      hasDescendant(
        callExpr(
          callee(
            functionDecl(
              hasAnyName("free", "g_free")
            )
          )
        ).bind("df")
      )
    ),
  this);
}

void
DoubleFreeMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Expr *stmt = result.Nodes.getNodeAs<Expr>("df")) {
    std::string descr = "Opportunity : Double free in loop: \"";

    Matcher::print(result, stmt, descr);
  }
}