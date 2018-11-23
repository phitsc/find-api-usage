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


inline std::string toString(const clang::Stmt& callExpr)
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


inline void printToConsole(
    const clang::SourceManager& sm,
    const clang::CallExpr& callExpr,
    bool beVerbose,
    bool onlyUserCode = true)
{
    const auto ploc = sm.getPresumedLoc(callExpr.getBeginLoc());

    if (ploc.isValid() &&
        (!onlyUserCode || sm.getFileCharacteristic(callExpr.getBeginLoc()) == clang::SrcMgr::C_User)) {
        llvm::outs()
            << ploc.getFilename() << ":" << ploc.getLine() << ": "
            << toString(callExpr) << "\n";

        if (beVerbose) {
            llvm::outs() << ": " << callTypeAsString(callExpr) << " call\n";
        }
    }
}


inline std::string varDeclTypeAsString(const clang::DeclaratorDecl& declDecl)
{
    if (clang::dyn_cast<clang::FieldDecl>(&declDecl)) {
        return "member";
    } else if (clang::dyn_cast<clang::VarDecl>(&declDecl)) {
        return "non-member";
    } else {
        return "other";
    }
}


inline void printToConsole(
    const clang::SourceManager& sm,
    const clang::DeclaratorDecl& declDecl,
    bool beVerbose,
    bool onlyUserCode = true)
{
    const auto ploc = sm.getPresumedLoc(declDecl.getBeginLoc());

    if (ploc.isValid() &&
        (!onlyUserCode || sm.getFileCharacteristic(declDecl.getBeginLoc()) == clang::SrcMgr::C_User)) {
        llvm::outs()
            << ploc.getFilename() << ":" << ploc.getLine() << ": "
            << toString(declDecl, sm) << "\n";

        if (beVerbose) {
            llvm::outs() << ": " << varDeclTypeAsString(declDecl) << " variable declaration\n";
        }
    }
}
