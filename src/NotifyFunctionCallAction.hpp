#pragma once

#include "ClangHelpers.hpp"
#include "FrontendAction.hpp"
#include "JsonFile.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

/*! A frontend action matching function calls
*/
class NotifyFunctionCallAction : public FrontendAction
{
  public:
    //! Constructs a NotifyFunctionCallAction
    //! \param className The name of the class for which to find method invocations
    //! \param functionName The name of the function or method for which to find invocations
    //! \param options An Options object holding the program arguments
    //! \param jsonFile A JsonFile object to write details about found function calls to
    NotifyFunctionCallAction(
        const std::string& className,
        const std::string& functionName,
        const Options& options,
        JsonFile& jsonFile
    )
        : m_className(className)
        , m_functionName(functionName)
        , m_options(options)
        , m_jsonFile(jsonFile)
    {
    }

    //! Returns the MatchFinder expression for matching
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

    //! \private
    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (const auto funcCall = result.Nodes.getNodeAs<clang::CallExpr>("function_call")) {
            if (m_jsonFile) {
                m_jsonFile.write(
                    *result.SourceManager,
                    *funcCall
                );
            } else {
                printToConsole(
                    *result.SourceManager,
                    *funcCall,
                    m_options["verbose"].as<bool>());
            }
        }
    }

private:
    std::string m_className;
    std::string m_functionName;
    Options m_options;
    JsonFile& m_jsonFile;
};
