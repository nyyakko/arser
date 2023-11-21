#pragma once

#include "argument.hpp"

#include <vector>

namespace arser {

    class argument_register
    {
    public:
        using argument_const_iterator_t = std::vector<arser::argument>::const_iterator;

    public:
        std::vector<arser::argument> const& get_registered_arguments() const;

        argument_const_iterator_t find_registered_argument(std::pair<std::string_view, std::string_view> argumentNameorAlias) const;

        bool contains(std::pair<std::string_view, std::string_view> argumentNameorAlias) const;

        auto& register_argument(auto&& ... parameters)
        {
            this->arguments.emplace_back(parameters...);
            return this->arguments.back();
        }

    private:
        std::vector<arser::argument> arguments {};
    };

}
