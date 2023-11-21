#include "argument.hpp"

bool arser::argument::has_value() const noexcept
{
    if (std::holds_alternative<kind::flag_t>(this->kind))
    {
        return true;
    }

    static constexpr auto variantSize = std::variant_size_v<decltype(this->kind)> - 1;

    return [this] <std::size_t ... I> (std::index_sequence<I...>)
    {
        return (std::holds_alternative<
                    typename std::decay_t<
                        decltype(std::get<I + 1>(this->kind))
                    >::type
                >(this->value) || ...);
    }(std::make_index_sequence<variantSize>{});
}

bool arser::argument::has_dependencies() const noexcept
{
    return !this->dependencies.empty();
}

bool arser::argument::is_required() const noexcept
{
    return this->level == level::required;
}

bool arser::argument::is_optional() const noexcept
{
    return this->level == level::optional;
}

arser::argument& arser::argument::depends_on(std::string_view dependencyName, std::string_view dependencyAlias)
{
    this->dependencies.emplace_back(dependencyName, dependencyAlias);
    return *this;
}

