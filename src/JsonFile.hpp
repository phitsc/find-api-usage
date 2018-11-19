#pragma once

#include "ClangStatement.hpp"

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
        const clang::Stmt& statement)
    {
        if (!m_filePath.empty()) {
            const std::string filename = sm.getFilename(statement.getBeginLoc());

            m_json += {
                { "file", filename },
                { "line", sm.getSpellingLineNumber(statement.getBeginLoc()) },
                { "message", toString(statement) }
            };
        }
    }

private:
    std::string m_filePath;
    nlohmann::json m_json;
};