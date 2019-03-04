/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "buffer-write-functions.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
BufferWriteFunctionsMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    callExpr(
      callee(
        functionDecl(
          hasAnyName("strcpy", "strncpy", "strncat", "strcat", "fgets", "wcsncpy", "wcscpy", "wcsncat", "wcscat", "wcpcpy", "wcpncpy", "memset", "wmemset", "memcpy", "memmove", "snprintf", "SNPRINTF")
        )
      )
      /*
      callee(
        functionDecl(
          hasName("fgets"),
          hasParameter(0, parmVarDecl(hasType(pointsTo(isAnyCharacter())))),
          hasParameter(1, parmVarDecl(hasType(isInteger()))),
          hasParameter(2, parmVarDecl(hasType(pointsTo(asString("FILE")))))
        )
      ),
      callee(
        functionDecl(
          hasName("wcsncpy"),
          hasParameter(0, parmVarDecl(hasType(pointsTo(asString("wchar_t"))))),
          hasParameter(2, parmVarDecl(hasType(asString("size_t"))))
        )
      )*/
    ).bind("ce"),
  this);
}

void
BufferWriteFunctionsMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("ce")) {
    std::string descr = "Opportunity : Buffer write. Look near: \"";

    Matcher::print(result, stmt, descr);
  }
}