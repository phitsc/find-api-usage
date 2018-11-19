#pragma once

#include <clang/AST/Decl.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Stmt.h>
#include <clang/Basic/LangOptions.h>
#include <clang/Basic/SourceLocation.h>
#include <clang/Basic/SourceManager.h>

inline std::string toString(const clang::Stmt& statement)
{
    std::string buffer;
    llvm::raw_string_ostream os(buffer);
    statement.printPretty(os, nullptr, clang::PrintingPolicy(clang::LangOptions()));
    return os.str();
}


inline void printToConsole(
    const clang::SourceManager& sm,
    const clang::Stmt& statement,
    bool beVerbose,
    bool onlyUserCode = true)
{
    const auto ploc = sm.getPresumedLoc(statement.getBeginLoc());

    if (ploc.isValid() &&
        (!onlyUserCode || sm.getFileCharacteristic(statement.getBeginLoc()) == clang::SrcMgr::C_User)) {
        llvm::outs()
            << ploc.getFilename() << ":" << ploc.getLine() << ": "
            << toString(statement) << "\n";

        // if (beVerbose) {
        //     llvm::outs()
        //         << ": " << fc.verboseMessage() << "\n";
        // }
    }
}
