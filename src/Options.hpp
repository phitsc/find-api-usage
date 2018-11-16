#pragma once

#include <map>
#include <string>
#include <boost/variant.hpp>

using OptionValue = boost::variant<bool, int, std::string>;

class Option
{
public:
    Option() = default;

    Option(std::string id, std::string description, OptionValue value)
        : m_id(id)
        , m_description(description)
        , m_value(value)
    {}

    const std::string& id() const
    {
        return m_id;
    }

    const OptionValue& value() const
    {
        return m_value;
    }

    template<typename T>
    const T& as() const
    {
        return boost::get<T>(value());
    }

private:
    std::string m_id;
    std::string m_description;
    OptionValue m_value;
};


class Options
{
public:
    const Option& operator[](const std::string& id) const
    {
        const auto it = m_options.find(id);

        if (it != m_options.end()) {
            return it->second;
        } else {
            static Option option;
            return option;
        }
    }

    void add(Option option)
    {
        m_options[option.id()] = option;
    }

private:
    std::map<std::string, Option> m_options;
};
