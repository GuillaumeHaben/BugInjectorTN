/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "return-expr.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
ReturnExpressionMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    compoundStmt(
      hasParent(
        functionDecl()
      ),
      findAll(
        returnStmt(
          unless(
            anyOf(
              has(ignoringParenImpCasts(
                characterLiteral()
              )),
              has(ignoringParenImpCasts(
                floatLiteral()
              )),
              has(ignoringParenImpCasts(
                integerLiteral()
              )),
              has(ignoringParenImpCasts(
                stringLiteral()
              )),
              has(ignoringParenImpCasts(
                userDefinedLiteral()
              )) 
              ,has(ignoringParenImpCasts(
                compoundLiteralExpr()
              ))
            )
          )
        ).bind("return-expr"))
      ),
  this);
}

void
ReturnExpressionMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("return-expr")) {
    std::string descr = "Opportunity: Return statements returning an expression. Look near: \"";
  
    Matcher::print(result, stmt, descr);
  }
}