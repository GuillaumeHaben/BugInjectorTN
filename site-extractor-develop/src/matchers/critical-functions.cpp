/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "critical-functions.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
CriticalFunctionsMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    callExpr(
      callee(
        functionDecl(
          hasAnyName("g_free", "g_strdup", "g_malloc", "g_strdup_printf", "g_strlcat", "g_strlcpy")
        ).bind("function")
      )
    ).bind("ce"),
  this);
}

void
CriticalFunctionsMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("ce")) {
    std::string descr = "Interesting function call: \"";

    Matcher::print(result, stmt, descr);
  }
}
