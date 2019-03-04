/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "buffer-underflow.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace llvm;
using namespace clang;
using namespace clang::ast_matchers;

#define CONDITION hasCondition(binaryOperator(forEachDescendant(declRefExpr().bind("varDecl"))))

void
BufferUnderflowMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    compoundStmt(
      anyOf(
        hasParent(
          whileStmt(
            CONDITION
          )
        ),
        hasParent(
          forStmt(
            CONDITION
          )
        ),
        hasParent(
          doStmt(
            CONDITION
          )
        )
      ),
      hasDescendant(
        unaryOperator(
          hasDescendant(
            declRefExpr().bind("varUsed")
          ),
          hasOperatorName("--")
        )
      )
    ),
  this);
}

void
BufferUnderflowMatcher::run(const MatchFinder::MatchResult &result)
{
  const Stmt *vu = result.Nodes.getNodeAs<Stmt>("varUsed");
  const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("varDecl");

  if (vu && stmt) {
    // Check node equivalence
    ASTContext *Context = result.Context;
    llvm::FoldingSetNodeID FirstID, SecondID;
    vu->Profile(FirstID, *Context, true);
    stmt->Profile(SecondID, *Context, true);

    if (FirstID == SecondID) {
      std::string descr = "Opportunity : Buffer underflow. var named: \"";
  
      Matcher::print(result, stmt, descr);
    }
  }
}