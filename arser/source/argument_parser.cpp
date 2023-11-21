#include "argument_parser.hpp"
#include "argument.hpp"

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <sstream>
#include <cassert>

namespace detail {

    template <class ... Ts> struct overload : Ts ...
    {
        using Ts::operator()...;
    };

}

arser::argument const& arser::argument_parser::get_argument(std::string_view argumentName, std::string_view argumentAlias) const
{
    if (this->argumentsMap.contains(argumentName.data()))
    {
        return this->argumentsMap.at(argumentName.data());
    }

    return this->argumentsMap.at(argumentAlias.data());
}

bool arser::argument_parser::contains(std::string_view argumentName, std::string_view argumentAlias) const
{
    return this->argumentsMap.contains(argumentName.data()) || this->argumentsMap.contains(argumentAlias.data());
}

void arser::argument_parser::parse(std::span<const  char*> const& arguments)
{
    auto argumentsStack = argument_parser::tokenize(arguments, this->argumentRegister);

    while (!argumentsStack.empty())
    {
        auto const& argument = argumentsStack.top();
        auto const& [name, alias] = argument.get_name();
        this->argumentsMap[!name.empty() ? name : alias] = argument;
        argumentsStack.pop();
    }

    if (auto argument = argument_parser::has_missing_required_argument(*this, this->argumentRegister))
    {
        auto const& [name, alias] = argument.value().get_name();
        std::cerr << "MISSING REQUIRED ARGUMENT " << std::quoted(!name.empty() ? name : alias) << '\n';
    }

    if (auto argument = argument_parser::has_argument_with_missing_value(*this))
    {
        auto const& [name, alias] = argument.value().get_name();
        std::cerr << "MISSING VALUE FOR NON-FLAG ARGUMENT " << std::quoted(!name.empty() ? name : alias) << '\n';
    }

    if (auto argument = argument_parser::has_argument_with_missing_dependencies(*this))
    {
        auto const& [name, alias] = argument->first.get_name();

        std::cerr << "MISSING THE FOLLOWING DEPENDENCIES FOR ARGUMENT " << std::quoted(!name.empty() ? name : alias) << " [ ";

        for (auto const& [_name, _alias] : argument->second)
        {
            std::cerr << (!_name.empty() ? _name : _alias) << ' ';
        }

        std::cerr << "]\n";
    }
}

std::stack<arser::argument> arser::argument_parser::tokenize(std::span<char const*> const& arguments, argument_register const& argumentRegister)
{
    std::stack<arser::argument> argumentStack {};

    for (auto argument : arguments | std::views::transform([] (auto value) { return std::string_view(value); }))
    {
        if (argument.starts_with("-"))
        {
            auto argumentIterator = argumentRegister.find_registered_argument({ argument, argument });

            if (argumentIterator != argumentRegister.get_registered_arguments().end())
            {
                argumentStack.push(*argumentIterator);
                continue;
            }

            std::cerr << "IGNORING UNRECOGNIZED ARGUMENT: " << std::quoted(argument) << '\n';
            continue;
        }

        if (argumentStack.empty() || argumentStack.top().has_value() || arser::argument_is<kind::flag_t>(argumentStack.top()))
        {
            std::cerr << "IGNORING EXTRANEOUS ARGUMENT VALUE: " << std::quoted(argument) << '\n';
            continue;
        }

        std::visit(detail::overload
        {
            [&] (auto kind) requires std::is_same_v<std::decay_t<decltype(kind)>, kind::flag_t>
            {
                assert(false && "UNREACHABLE");
            },
            [&] (auto kind) requires std::is_same_v<std::decay_t<decltype(kind)>, kind::string_t>
            {
                argumentStack.top().set_value(argument.data());
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

std::optional<arser::argument> arser::argument_parser::has_missing_required_argument(argument_parser const& argumentParser, argument_register const& argumentRegister)
{
    for (auto const& argument : argumentRegister.get_registered_arguments() | std::views::filter(&arser::argument::is_required))
    {
        if (!argumentParser.contains(argument.get_name().first, argument.get_name().second))
        {
            return argument;
        }
    }

    return std::nullopt;
}

std::optional<arser::argument> arser::argument_parser::has_argument_with_missing_value(argument_parser const& argumentParser)
{
    auto arguments = argumentParser.get_arguments() | std::views::values;
    auto argument  = std::ranges::find_if(arguments, std::not_fn(&arser::argument::has_value));

    if (argument != arguments.end())
    {
        return *argument;
    }

    return std::nullopt;
}

std::optional<std::pair<arser::argument, std::vector<std::pair<std::string_view, std::string_view>>>> arser::argument_parser::has_argument_with_missing_dependencies(argument_parser const& argumentParser)
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
