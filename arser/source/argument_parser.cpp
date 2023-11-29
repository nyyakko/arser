#include "argument_parser.hpp"
#include "argument.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>

namespace arser {

    namespace detail {

        template <class ... Ts> struct overload : Ts ...
        {
            using Ts::operator()...;
        };

    }

std::size_t argument_parser::get_number_of_arguments() const noexcept
{
    return this->numberOfArguments;
}

argument_parser::arguments_t const& argument_parser::get_arguments() const noexcept
{
    return this->argumentsMap;
}

argument const& argument_parser::get_argument(std::string_view argumentName) const
{
    return this->argumentsMap.at(argumentName.data());
}

bool argument_parser::contains(std::string_view argumentName) const
{
    return this->argumentsMap.contains(argumentName.data());
}

void argument_parser::parse(std::span<const  char*> const& arguments)
{
    auto argumentsStack = argument_parser::tokenize(arguments, this->argumentRegister);

    this->numberOfArguments = argumentsStack.size();

    while (!argumentsStack.empty())
    {
        auto const& argument = argumentsStack.top();
        this->argumentsMap[argument.get_name().first] = argument;
        argumentsStack.pop();
    }

    if (auto argument = argument_parser::has_missing_required_argument(*this, this->argumentRegister))
    {
        std::println(std::cerr, "[arser/warn] missing required argument: {}\n", argument.value().get_name().first);
        return;
    }

    if (auto argument = argument_parser::has_argument_with_missing_value(*this))
    {
        std::println(std::cerr, "[arser/warn] missing value for non-flag argument: {}", argument.value().get_name().first);
        return;
    }

    if (auto argument = argument_parser::has_argument_with_missing_dependencies(*this))
    {
        std::println(std::cerr, "[arser/warn] argument {} requires the following dependencies: ", argument->first.get_name().first);

        for (auto const& [_name, _alias] : argument->second)
        {
            std::println(std::cerr, "{} ", _name);
        }

        std::print(std::cerr, "\n");
        return;
    }
}

std::stack<argument> argument_parser::tokenize(std::span<char const*> const& arguments, argument_register const& argumentRegister)
{
    std::stack<argument> argumentStack {};

    for (auto argument : arguments | std::views::transform([] (auto value) { return std::string_view(value); }))
    {
        if (argument.starts_with("-"))
        {
            auto argumentIterator = argumentRegister.find_registered_argument(argument, argument);

            if (argumentIterator != argumentRegister.get_registered_arguments().end())
            {
                argumentStack.push(*argumentIterator);
                continue;
            }

            std::println(std::cerr, "[arser/warn] ignoring unrecognized argument: {}", argument);
            continue;
        }

        if (argumentStack.empty() || argumentStack.top().has_value() || arser::argument_is<kind::flag_t>(argumentStack.top()))
        {
            std::println(std::cerr, "[arser/warn] ignoring extraneous argument value: {}", argument);
            continue;
        }

        std::visit(detail::overload
        {
            [&] (auto kind) requires std::is_same_v<std::decay_t<decltype(kind)>, kind::flag_t>
            {
                assert(false && "UNREACHABLE");
            },
            [&] (auto kind) requires std::is_same_v<std::decay_t<decltype(kind)>, kind::boolean_t>
            {
                auto value = std::ranges::to<std::string>(argument | std::views::transform(::tolower));

                if (value != "true" && value != "false" && value != "1" && value != "0")
                {
                    std::println(std::cerr, "[arser/warn] ignoring unrecognized boolean value: ", argument);
                    argumentStack.top().set_value(false);
                }
                else
                {
                    argumentStack.top().set_value(argument == "true" || argument == "1");
                }
            },
            [&] (auto kind)
            {
                typename decltype(kind)::type value {};
                std::stringstream stream { argument.data() };
                stream >> value;

                argumentStack.top().set_value(value);
            }
        }, argumentStack.top().get_kind());
    }

    return argumentStack;
}

std::optional<argument> argument_parser::has_missing_required_argument(argument_parser const& argumentParser, argument_register const& argumentRegister)
{
    for (auto const& argument : argumentRegister.get_registered_arguments() | std::views::filter(&argument::is_required))
    {
        if (!argumentParser.contains(argument.get_name().first))
        {
            return argument;
        }
    }

    return std::nullopt;
}

std::optional<argument> argument_parser::has_argument_with_missing_value(argument_parser const& argumentParser)
{
    for (auto const& argument : argumentParser.get_arguments() | std::views::values)
    {
        if (!argument.has_value())
        {
            return argument;
        }
    }

    return std::nullopt;
}

std::optional<std::pair<argument, argument_parser::dependencies_t>> argument_parser::has_argument_with_missing_dependencies(argument_parser const& argumentParser)
{
    auto arguments = argumentParser.get_arguments() | std::views::values;

    for (auto const& argument : arguments)
    {
        std::vector<std::pair<std::string_view, std::string_view>> missingDependencies {};

        for (auto const& dependency : argument.get_dependencies())
        {
            auto begin = std::ranges::begin(arguments);
            auto end   = std::ranges::find(arguments, argument);

            auto precedingArguments = arguments | std::views::drop(1 + std::distance(begin, end));

            if (std::ranges::find(precedingArguments, dependency, &argument::get_name) == precedingArguments.end())
            {
                missingDependencies.push_back(dependency);
            }
        }

        if (!missingDependencies.empty())
        {
            return std::pair { argument, missingDependencies };
        }
    }

    return std::nullopt;
}

}
