#include "Helpers.hpp"
#include "JsonFile.hpp"
#include "NotifyFunctionCallAction.hpp"
#include "NotifyVariableDeclarationAction.hpp"
#include "Options.hpp"

#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Rewrite/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/ReplacementsYaml.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/YAMLTraits.h"

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;

namespace boost
{
    void throw_exception(const std::exception& e)
    {
        llvm::errs() << "Error: " << e.what();
        std::exit(1);
    }
}

namespace {

void printVersion(llvm::raw_ostream& ostream)
{
    ostream << "find-api-usage version 0.1.0\n";

    std::exit(0);
}

static cl::OptionCategory optionCategory("find-api-usage Options");

// clang-format off
static cl::opt<std::string> json("json",
    cl::desc("Export violations to JSON file"),
    cl::value_desc("filename"),
    cl::cat(optionCategory)
);

static cl::opt<bool> verbose("verbose",
    cl::desc("Be verbose when reporting violations"),
    cl::init(false),
    cl::cat(optionCategory)
);

static cl::alias verboseAlias("v",
    cl::desc("Same as -verbose"),
    cl::aliasopt(verbose)
);

static cl::list<std::string> functionCall("function-call",
    cl::desc("Function or method call"),
    cl::cat(optionCategory),
    cl::CommaSeparated
);

static cl::alias functionCallAlias("fc",
    cl::desc("Same as -function-call"),
    cl::aliasopt(functionCall)
);

static cl::list<std::string> variableDeclaration("variable-declaration",
    cl::desc("Variable declaration"),
    cl::cat(optionCategory),
    cl::CommaSeparated
);

static cl::alias variableDeclarationAlias("vd",
    cl::desc("Same as -variable-declaration"),
    cl::aliasopt(variableDeclaration)
);
// clang-format off

static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
// static cl::extrahelp MoreHelp("\nMore help text...\n");

template<typename T>
void addOpt(Options& opts, const T& opt, OptionValue optInit = OptionValue())
{
    opts.add({ opt.ArgStr, opt.HelpStr, opt.getNumOccurrences() > 0 ? opt : optInit });
}

template<typename ActionType>
void addMatcher(
    MatchFinder& matchFinder,
    std::vector<std::unique_ptr<FrontendAction>>& actions,
    ActionType&& action)
{
    matchFinder.addMatcher(action->matcher(), action.get());
    actions.push_back(std::move(action));
}

} // namespace


int main(int argc, const char** argv)
{
    cl::SetVersionPrinter(&printVersion);

    CommonOptionsParser optionsParser(argc, argv, optionCategory);

    ClangTool tool(optionsParser.getCompilations(), optionsParser.getSourcePathList());

    // CLANG_BUILTIN_INCLUDES_DIR is defined in CMakeLists.txt
    // const auto clangBuiltinIncludesDir = std::string("-I") + CLANG_BUILTIN_INCLUDES_DIR;
    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster(clangBuiltinIncludesDir.c_str()));

    // tool.appendArgumentsAdjuster(getInsertArgumentAdjuster("-Wno-gnu-include-next"));

    Options options;
    addOpt(options, json, std::string());
    addOpt(options, verbose, false);

    JsonFile jsonFile(options["json"].as<std::string>());

    std::vector<std::unique_ptr<FrontendAction>> actions;

    MatchFinder matchFinder;

    for (auto& fc : functionCall) {
        const auto [ className, methodName ] = splitClassMethod(fc);
        addMatcher(matchFinder, actions,
            std::make_unique<NotifyFunctionCallAction>(
                className, methodName, options, jsonFile));
    }

    for (auto& vd : variableDeclaration) {
        addMatcher(matchFinder, actions,
            std::make_unique<NotifyVariableDeclarationAction>(
                vd, options, jsonFile));
    }

    // MatchPrinter matchPrinter;
    // matchFinder.addMatcher(statementMatcher, &matchPrinter);

    return tool.run(newFrontendActionFactory(&matchFinder).get());
}
