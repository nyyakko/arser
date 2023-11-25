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

arser::argument& arser::argument::depends_on(std::string_view dependencyName, std::string_view dependencyAlias)
{
    this->dependencies.emplace_back(dependencyName, dependencyAlias);
    return *this;
}

bool arser::argument::operator==(argument const& lhs) const noexcept
{
    return this->name.first == lhs.name.first || (!this->name.second.empty() && this->name.second == lhs.name.second);
}
