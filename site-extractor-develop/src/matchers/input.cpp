#include "input.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
InputMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
  //functionDecl(
    //has(
      compoundStmt(
        forEachDescendant(
          callExpr(
            hasDescendant(
              declRefExpr(
                to(
                  functionDecl(
                    hasAnyName("scanf")
                  )
                )
              )
            )
        ).bind("scan")
      )
  //  )
  //)

      ).bind("while-null-pointer"),
    this);
    finder.addMatcher(
  //  declRefExpr(
      //hasDescendant(
        compoundStmt(
          forEachDescendant(
            binaryOperator(
              hasDescendant(
              callExpr(
              hasDescendant(
                declRefExpr(
                  to(
                    functionDecl(
                      hasAnyName("fopen")
                    )
                  )
                )
              )
          ).bind("fopen")
        )
      ).bind("coucou")
      )
//      )
  //  )

        ).bind("while-null-pointer"),
      this);
  }

void
InputMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("while-null-pointer")) {
    std::string descr = "is the function line\"";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("scan")) {
    std::string descr = "v";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("fopen")) {
    std::string descr = "v";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("coucou")) {
    std::string descr = "t";

    Matcher::print(result, stmt, descr);
  }
}
