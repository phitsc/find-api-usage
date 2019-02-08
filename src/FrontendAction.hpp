#pragma once

#include "clang/ASTMatchers/ASTMatchFinder.h"
/*! Base class for frontend actions
*/
class FrontendAction : public clang::ast_matchers::MatchFinder::MatchCallback
{
public:
    virtual ~FrontendAction() {}
};