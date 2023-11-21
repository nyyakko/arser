#include "argument_register.hpp"

#include <algorithm>

using argument_const_iterator_t = arser::argument_register::argument_const_iterator_t;

std::vector<arser::argument> const& arser::argument_register::get_registered_arguments() const
{
    return this->arguments;
}

argument_const_iterator_t arser::argument_register::find_registered_argument(std::pair<std::string_view, std::string_view> name) const
{
    return std::ranges::find_if(this->arguments, [&] (arser::argument const& argument)
    {
        return argument.get_name().first == name.first || argument.get_name().second == name.second;
    });
}

bool arser::argument_register::contains(std::pair<std::string_view, std::string_view> argumentNameorAlias) const
{
    return this->find_registered_argument(argumentNameorAlias) != this->arguments.end();
}

