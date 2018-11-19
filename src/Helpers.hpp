#pragma once

#include <string>
#include <tuple>

inline std::tuple<std::string, std::string> splitClassMethod(const std::string& classDoubleColonMethod)
{
    const auto pos = classDoubleColonMethod.find("::");

    if (pos != std::string::npos) {
        return std::make_tuple(
            classDoubleColonMethod.substr(0, pos),
            classDoubleColonMethod.substr(pos + 2));
    } else {
        return std::make_tuple(
            "",
            classDoubleColonMethod);
    }
}
