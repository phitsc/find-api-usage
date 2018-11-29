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
        write(sm, callExpr, callTypeAsString);
    }

    void write(
        const clang::SourceManager& sm,
        const clang::DeclaratorDecl& declDecl)
    {
        write(sm, declDecl, varDeclTypeAsString);
    }

private:
    template<typename T, typename U>
    void write(
        const clang::SourceManager& sm,
        const T& astObject,
        const U& astObjectAsString)
    {
        if (!m_filePath.empty()) {
            const auto ploc = sm.getPresumedLoc(astObject.getBeginLoc());
            if (ploc.isValid()) {
                m_json += {
                    { "file", ploc.getFilename() },
                    { "line", ploc.getLine() },
                    { "message", toString(astObject, sm) },
                    { "type", astObjectAsString(astObject) }
                };
            }
        }
    }

    std::string m_filePath;
    nlohmann::json m_json;
};
