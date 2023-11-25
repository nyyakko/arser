#include "argument_register.hpp"

#include <algorithm>

using argument_const_iterator_t = arser::argument_register::argument_const_iterator_t;

argument_const_iterator_t arser::argument_register::find_registered_argument(std::string_view argumentName, std::string_view argumentAlias) const
{
    return std::ranges::find_if(this->arguments, [&] (arser::argument const& argument)
    {
        return argument.get_name().first == argumentName || (!argumentAlias.empty() && argument.get_name().second == argumentAlias);
    });
}

bool arser::argument_register::contains(std::string_view argumentName, std::string_view argumentAlias) const
{
    return this->find_registered_argument(argumentName, argumentAlias) != this->arguments.end();
}

