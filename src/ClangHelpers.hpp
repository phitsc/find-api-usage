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

inline std::string toString(
    const clang::Stmt& callExpr,
    const clang::SourceManager&)
{
    std::string buffer;
    llvm::raw_string_ostream os(buffer);
    callExpr.printPretty(os, nullptr, clang::PrintingPolicy(clang::LangOptions()));
    return os.str();
}


inline std::string toString(
    const clang::NamedDecl& namedDecl,
    const clang::SourceManager& sourceManager)
{
    return clang::Lexer::getSourceText(
        clang::CharSourceRange::getTokenRange(namedDecl.getSourceRange()),
        sourceManager,
        clang::LangOptions());
}


inline std::string callTypeAsString(const clang::CallExpr& callExpr)
{
    if (clang::dyn_cast<clang::CXXMemberCallExpr>(&callExpr)) {
        return "method";
    } else {
        return "function";
    }
}


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


inline std::string verboseMessage(const clang::CallExpr& callExpr)
{
    return callTypeAsString(callExpr) + " call";
}


inline std::string verboseMessage(const clang::DeclaratorDecl& declDecl)
{
    return varDeclTypeAsString(declDecl) + " variable declaration of type '"
         + declDecl.getType()->getLocallyUnqualifiedSingleStepDesugaredType().getAsString();
}


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
