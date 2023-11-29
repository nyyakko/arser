#include "argument.hpp"

namespace arser {

argument::name_t const& argument::get_name() const noexcept
{
    return this->name;
}

kind::kind_t const& argument::get_kind() const noexcept
{
    return this->kind;
}

level::level_t const& argument::get_level() const noexcept
{
    return this->level;
}

argument::value_t const& argument::get_value() const noexcept
{
    return this->value;
}

argument::dependencies_t const& argument::get_dependencies() const noexcept
{
    return this->dependencies;
}

bool argument::has_dependencies() const noexcept
{
    return !this->dependencies.empty();
}

bool argument::has_value() const noexcept
{
    if (std::holds_alternative<kind::flag_t>(this->kind))
    {
        return true;
    }

    static constexpr auto VARIANT_SIZE = std::variant_size_v<decltype(this->kind)> - 1;

    return [this] <std::size_t ... I> (std::index_sequence<I...>)
    {
        return (std::holds_alternative<
                    typename std::decay_t<
                        decltype(std::get<I + 1>(this->kind))
                    >::type
                >(this->value) || ...);
    }(std::make_index_sequence<VARIANT_SIZE>{});
}

bool argument::is_required() const noexcept
{
    return this->level == level::required;
}

bool argument::is_optional() const noexcept
{
    return this->level == level::optional;
}

argument& argument::depends_on(std::string_view dependencyName, std::string_view dependencyAlias)
{
    this->dependencies.emplace_back(dependencyName, dependencyAlias);
    return *this;
}

bool argument::operator==(argument const& lhs) const noexcept
{
    return this->name.first == lhs.name.first || (!this->name.second.empty() && this->name.second == lhs.name.second);
}

}

