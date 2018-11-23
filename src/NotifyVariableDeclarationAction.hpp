#pragma once

#include "ClangHelpers.hpp"
#include "FrontendAction.hpp"
#include "JsonFile.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

class NotifyVariableDeclarationAction : public FrontendAction
{
  public:
    NotifyVariableDeclarationAction(
        const std::string& typeName,
        const Options& options,
        JsonFile& jsonFile
    )
        : m_typeName(typeName)
        , m_options(options)
        , m_jsonFile(jsonFile)
    {
    }

    auto matcher() const
    {
        using namespace clang::ast_matchers;

        return declaratorDecl(anyOf(
            varDecl(hasType(namedDecl(hasName(m_typeName)))),
            fieldDecl(hasType(namedDecl(hasName(m_typeName))))
        )).bind("declarator_decl");
    }

    virtual void run(const clang::ast_matchers::MatchFinder::MatchResult& result) override
    {
        if (const auto declDecl = result.Nodes.getNodeAs<clang::DeclaratorDecl>("declarator_decl")) {
            if (m_jsonFile) {
                m_jsonFile.write(
                    *result.SourceManager,
                    *declDecl
                );
            } else {
                printToConsole(
                    *result.SourceManager,
                    *declDecl,
                    m_options["verbose"].as<bool>());
            }
        }
    }

private:
    std::string m_typeName;
    Options m_options;
    JsonFile& m_jsonFile;
};
