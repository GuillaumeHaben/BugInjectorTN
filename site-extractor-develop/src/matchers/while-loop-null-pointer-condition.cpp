/* WHILE LOOP */


/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#include "while-loop-null-pointer-condition.hpp"
#include "matcher.hpp"
#include <iostream>
#include <clang/ASTMatchers/ASTMatchFinder.h>

using namespace clang;
using namespace clang::ast_matchers;

void
WhileLoopNullPointerConditionMatcher::registerASTMatcherCallback(MatchFinder &finder)
{
  finder.addMatcher(
    whileStmt(
      anyOf(
      hasCondition(hasType(isAnyPointer())),
      hasCondition(expr(nullPointerConstant()))
    )
      /*hasCondition(
        binaryOperator(
          hasOperatorName("==")
        )
      )*/
    ).bind("while-null-pointer"),
  this);
}

void
WhileLoopNullPointerConditionMatcher::run(const MatchFinder::MatchResult &result)
{
  if (const Stmt *stmt = result.Nodes.getNodeAs<Stmt>("while-null-pointer")) {
    std::string descr = "While loop with null pointer condition detected in \"";

    Matcher::print(result, stmt, descr);
  }
}



/*


expr(nullPointerConstant())


hasCondition(
	anyOf(
		binaryOperator(
			anyOf(
			     hasOperatorName("=="),
			     hasOperatorName("=")
      ),
      hasLHS(
				ignoringParenImpCasts(
          declRefExpr(
            to(varDecl(hasType(isAnyPointer())))
          )
        )
			),
			hasRHS(
        ignoringParenImpCasts(
          has(expr(nullPointerConstant()))
        )
			)
		),
		ignoringParenImpCasts(
				declRefExpr(
          to(varDecl(hasType(isAnyPointer())))
        )
		)
	)
)
*/
