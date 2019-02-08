#pragma once

#include <map>
#include <string>
#include <boost/variant.hpp>

using OptionValue = boost::variant<bool, int, std::string>;

/*! A program option
*/
class Option
{
public:
    Option() = default;

    //! Constructs an Option object
    //! \param id The option's identifier
    //! \param description The option's description
    //! \param value The option's value
    Option(std::string id, std::string description, OptionValue value)
        : m_id(id)
        , m_description(description)
        , m_value(value)
    {}

    //! Returns the option's identifier
    const std::string& id() const
    {
        return m_id;
    }

    //! Returns the option's value
    const OptionValue& value() const
    {
        return m_value;
    }

    //! Returns the option's value converted to type T
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


/*! A collection of Option objects
*/
class Options
{
public:
    //! Returns the Option with the identifier \c id or
    //! an empty Option if no option with the passed
    //! identifier can be found
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

    //! Adds \c option to this collection of options
    void add(Option option)
    {
        m_options[option.id()] = option;
    }

private:
    std::map<std::string, Option> m_options;
};
