/*This software was developed at the National Institute of Standards and
Technology by employees of the Federal Government in the course of their
official duties. Pursuant to title 17 Section 105 of the United States
Code this software is not subject to copyright protection and is in the
public domain. NIST assumes no responsibility whatsoever for its use by
other parties, and makes no guarantees, expressed or implied, about its
quality, reliability, or any other characteristic.

We would appreciate acknowledgement if the software is used.
The SAMATE project website is: http://samate.nist.gov*/
#pragma once

#include "matcher.hpp"
#include "clang/ASTMatchers/ASTMatchFinder.h"

class BufferWriteFunctionsMatcher : public Matcher,
                                 public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
  void registerASTMatcherCallback(clang::ast_matchers::MatchFinder &finder) override;

  void run(const clang::ast_matchers::MatchFinder::MatchResult &result) override;
};
