#pragma once

#include "ClangHelpers.hpp"

#include "../nlohmann/json.hpp"

#include <fstream>
#include <iomanip>
#include <memory>

class JsonFile
{
public:
    JsonFile(const std::string& filePath)
        : m_filePath(filePath)
    {
    }

    ~JsonFile()
    {
        if (!m_filePath.empty()) {
            std::ofstream stream(m_filePath.c_str());
            if (stream.is_open()) {
                stream << std::setw(2) << m_json << std::endl;
            }
        }
    }

    operator bool() const
    {
        return !m_filePath.empty();
    }

    void write(
        const clang::SourceManager& sm,
        const clang::CallExpr& callExpr)
    {
        if (!m_filePath.empty()) {
            const std::string filename = sm.getFilename(callExpr.getBeginLoc());

            m_json += {
                { "file", filename },
                { "line", sm.getSpellingLineNumber(callExpr.getBeginLoc()) },
                { "message", toString(callExpr) },
                { "type", callTypeAsString(callExpr) }
            };
        }
    }

    void write(
        const clang::SourceManager& sm,
        const clang::DeclaratorDecl& declDecl)
    {
        if (!m_filePath.empty()) {
            const std::string filename = sm.getFilename(declDecl.getBeginLoc());

            m_json += {
                { "file", filename },
                { "line", sm.getSpellingLineNumber(declDecl.getBeginLoc()) },
                { "message", toString(declDecl, sm) },
                { "type", varDeclTypeAsString(declDecl) }
            };
        }
    }

private:
    std::string m_filePath;
    nlohmann::json m_json;
};