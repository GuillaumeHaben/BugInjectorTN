#include "input.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
InputMatcher::registerASTMatcherCallback(MatchFinder &finder)
{

  // Scanf matcher
  finder.addMatcher(
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
      ).bind("function-line"),
    this);

  //fopen matcher
    finder.addMatcher(
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
      ).bind("fopenvarname")
      )
    ).bind("function-line"),
      this);
  }

  //Add you matcher here

void
InputMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("function-line")) {
    std::string descr = "is the function line\"";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("scan")) {
    //Token for the script to know the function (scanf, fopen, ...) will be given just after
    std::string descr = "v";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("fopen")) {
    std::string descr = "v";

    Matcher::print(result, stmt, descr);
  }
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("fopenvarname")) {
    //Token for the script: the following will give the VAR in VAR = fopen(...)
    std::string descr = "t";

    Matcher::print(result, stmt, descr);
  }

  //Add your printer here, with the token (a new one if necessary)

}
