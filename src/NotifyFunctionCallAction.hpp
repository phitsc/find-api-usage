#pragma once

#include "FrontendAction.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

class NotifyFunctionCallAction : public FrontendAction
{
  public:
    NotifyFunctionCallAction(
        const std::string& functionName,
        const Options& options
    )
        : m_functionName(functionName)
        , m_options(options)
    {
    }

    NotifyFunctionCallAction(
        const std::string& className,
        const std::string& functionName,
        const Options& options
    )
        : m_className(className)
        , m_functionName(functionName)
        , m_options(options)
    {
    }

    auto matcher() const
    {
        using namespace clang::ast_matchers;

        if (!m_className.empty()) {
            return callExpr(
                callee(cxxMethodDecl(ofClass(hasName(m_className.c_str())))),
                callee(cxxMethodDecl(hasName(m_functionName.c_str())))
            ).bind("function_call");
        } else {
            return callExpr(
                callee(functionDecl(hasName(m_functionName.c_str())))
            ).bind("function_call");
        }
    }

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (const auto funcCall = result.Nodes.getNodeAs<clang::CallExpr>("function_call")) {
            auto& sm = *result.SourceManager;
            llvm::outs()
                << sm.getFilename(funcCall->getBeginLoc()) << ":" << sm.getSpellingLineNumber(funcCall->getEndLoc()) << ": "
                << "Found '" << m_className << "::" << m_functionName << "()'\n";
        } else {
            llvm::outs() << "found other match\n";
        }
    }

private:
    std::string m_className;
    std::string m_functionName;
    Options m_options;
};
