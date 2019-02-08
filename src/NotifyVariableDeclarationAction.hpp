#pragma once

#include "ClangHelpers.hpp"
#include "FrontendAction.hpp"
#include "JsonFile.hpp"
#include "Options.hpp"

#include "clang/ASTMatchers/ASTMatchFinder.h"

#include <string>

/*! A frontend action matching variable declarations
*/
class NotifyVariableDeclarationAction : public FrontendAction
{
  public:
    //! Constructs a NotifyVariableDeclarationAction
    //! \param typeName The name of the data type for which to find variable declarations
    //! \param options An Options object holding the program arguments
    //! \param jsonFile A JsonFile object to write details about found variable declarations to
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

    //! Returns the MatchFinder expression for matching
    auto matcher() const
    {
        using namespace clang::ast_matchers;

        return declaratorDecl(hasType(hasUnqualifiedDesugaredType(
            anyOf(
                enumType(hasDeclaration(namedDecl(hasName(m_typeName)))),
                recordType(hasDeclaration(namedDecl(hasName(m_typeName))))
            )
        ))).bind("declarator_decl");
    }

    //! \private
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
