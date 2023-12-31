#pragma once

#include "argument.hpp"
#include "argument_register.hpp"

#include <stack>
#include <span>
#include <unordered_map>
#include <optional>

namespace arser {

    class argument_parser
    {
    public:
        using dependencies_t = std::vector<std::pair<std::string_view, std::string_view>>;
        using arguments_t    = std::unordered_map<std::string, argument>;

        argument_parser(argument_register const& argumentRegister):
            argumentRegister(argumentRegister)
        {}

        std::size_t get_number_of_arguments() const noexcept;
        arguments_t const& get_arguments() const noexcept;
        argument const& get_argument(std::string_view argumentName) const;

        bool contains(std::string_view argumentName) const;

        static std::optional<argument> has_missing_required_argument(argument_parser const& argumentParser, argument_register const& argumentRegister);
        static std::optional<argument> has_argument_with_missing_value(argument_parser const& argumentParser);
        static std::optional<std::pair<argument, dependencies_t>> has_argument_with_missing_dependencies(argument_parser const& argumentParser);

        void parse(std::span<char const*> const& arguments);

    private:
        static std::stack<argument> tokenize(std::span<char const*> const& arguments, argument_register const& argumentRegister);

        argument_register const& argumentRegister;
        arguments_t argumentsMap {};

        std::size_t numberOfArguments = 0;
    };

}
