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
        argument_parser(arser::argument_register const& argumentRegister):
            argumentRegister(argumentRegister)
        {}

        auto const& get_arguments() const noexcept { return this->argumentsMap; }
        argument const& get_argument(std::string_view argumentName, std::string_view argumentAlias = "") const;

        bool contains(std::string_view argumentName, std::string_view argumentAlias = "") const;

        static std::optional<argument> has_missing_required_argument(argument_parser const& argumentParser, argument_register const& argumentRegister);
        static std::optional<argument> has_argument_with_missing_value(argument_parser const& argumentParser);
        static std::optional<std::pair<argument, std::vector<std::pair<std::string_view, std::string_view>>>> has_argument_with_missing_dependencies(argument_parser const& argumentParser);

        void parse(std::span<char const*> const& arguments);

    private:
        static std::stack<argument> tokenize(std::span<char const*> const& arguments, argument_register const& argumentRegister);

        argument_register const& argumentRegister;
        std::unordered_map<std::string, argument> argumentsMap {};
    };

}
