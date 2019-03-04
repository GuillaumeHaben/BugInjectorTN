#pragma once

#include "matcher.hpp"
#include "clang/ASTMatchers/ASTMatchFinder.h"

class InputMatcher : public Matcher,
                           public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
  void registerASTMatcherCallback(clang::ast_matchers::MatchFinder &finder) override;

  void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
