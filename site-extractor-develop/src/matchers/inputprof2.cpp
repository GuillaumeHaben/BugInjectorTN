#include "inputprof2.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
prof2Matcher::registerASTMatcherCallback(MatchFinder &finder)
{
  /*finder.addMatcher(
  //functionDecl(
    //has(
      compoundStmt(
        forEachDescendant(
          binaryOperator(
            hasDescendant(
              declRefExpr(
                to(
                  varDecl(
                    hasAnyName("age")
                  )
                )
              )
            )
        ).bind("scan")
      )
  //  )
  //)

      ).bind("while-null-pointer"),
    this);*/
  }

void
prof2Matcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("while-null-pointer")) {
    std::string descr = "is the function line\"";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("scan")) {
    std::string descr = "p\"";

    Matcher::print(result, stmt, descr);
  }
}
