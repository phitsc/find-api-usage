//! \file ClangHelpers.hpp
//! \brief Free functions adding Clang-related functionality
//! \addtogroup freefunctions Free functions
//! \{
#pragma once

#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>
#include <clang/Lex/Lexer.h>
#include <llvm/Support/Casting.h>
#include <string>

//! Convert a \c clang:Stmt to a string
//! \param statement The clang statement to convert
//! \result Returns the converted string
inline std::string toString(
    const clang::Stmt& statement,
    const clang::SourceManager&)
{
    std::string buffer;
    llvm::raw_string_ostream os(buffer);
    statement.printPretty(os, nullptr, clang::PrintingPolicy(clang::LangOptions()));
    return os.str();
}


//! Convert a \c clang:NamedDecl to a string
//! \param namedDecl The named declaration to convert
//! \param sourceManager A clang::SourceManager instance
//! \result Returns the converted named declaration
inline std::string toString(
    const clang::NamedDecl& namedDecl,
    const clang::SourceManager& sourceManager)
{
    return clang::Lexer::getSourceText(
        clang::CharSourceRange::getTokenRange(namedDecl.getSourceRange()),
        sourceManager,
        clang::LangOptions());
}


//! Returns \c "method" if \c callExpr refers to a method (member function) and
//! \c "function" if \c callExpr refers to a function (free function)
inline std::string callTypeAsString(const clang::CallExpr& callExpr)
{
    if (clang::dyn_cast<clang::CXXMemberCallExpr>(&callExpr)) {
        return "method";
    } else {
        return "function";
    }
}


//! Returns one of the following strings, depending on \c declDecl
//! \li \c "member" for member variables
//! \li \c "parameter" for function parameters
//! \li \c "non-member" for global variables
//! \li \c "other" if none of the above
inline std::string varDeclTypeAsString(const clang::DeclaratorDecl& declDecl)
{
    if (clang::dyn_cast<clang::FieldDecl>(&declDecl)) {
        return "member";
    } else if (clang::dyn_cast<clang::ParmVarDecl>(&declDecl)) {
        return "parameter";
    } else if (clang::dyn_cast<clang::VarDecl>(&declDecl)) {
        return "non-member";
    } else {
        return "other";
    }
}


//! Returns a verbose message string describing \c callExpr
inline std::string verboseMessage(const clang::CallExpr& callExpr)
{
    return callTypeAsString(callExpr) + " call";
}


//! Returns a verbose message string describing \c declDecl
inline std::string verboseMessage(const clang::DeclaratorDecl& declDecl)
{
    return varDeclTypeAsString(declDecl) + " variable declaration of type '"
         + declDecl.getType()->getLocallyUnqualifiedSingleStepDesugaredType().getAsString();
}


//! \private
template<typename T>
inline void printToConsole(
    const clang::SourceManager& sm,
    const T& astObject,
    bool beVerbose,
    bool onlyUserCode = true)
{
    const auto ploc = sm.getPresumedLoc(astObject.getBeginLoc());

    if (ploc.isValid() &&
        (!onlyUserCode || sm.getFileCharacteristic(astObject.getBeginLoc()) == clang::SrcMgr::C_User)) {
        llvm::outs()
            << ploc.getFilename() << ":" << ploc.getLine() << ": "
            << toString(astObject, sm) << "\n";

        if (beVerbose) {
            llvm::outs() << " : " << verboseMessage(astObject) << "\n";
        }
    }
}

//! \}