#pragma once

#include "argument.hpp"

#include <vector>
#include <cassert>

namespace arser {

    class argument_register
    {
    public:
        using argument_const_iterator_t = std::vector<arser::argument>::const_iterator;

    public:
        auto const& get_registered_arguments() const noexcept { return this->arguments; }

        argument_const_iterator_t find_registered_argument(std::string_view argumentName, std::string_view argumentAlias = "") const;

        bool contains(std::string_view argumentName, std::string_view argumentAlias = "") const;

        auto& register_argument(auto&& ... parameters)
        {
            this->arguments.emplace_back(parameters...);
            auto& argument = this->arguments.back();

            assert(!argument.get_name().first.empty() && "CANNOT HAVE AN ARGUMENT WITHOUT A NAME");

            return argument;
        }

    private:
        std::vector<arser::argument> arguments {};
    };

}
