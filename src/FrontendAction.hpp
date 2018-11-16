#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"

class FrontendAction : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    virtual ~FrontendAction() {}
};