#pragma once

#include <string>
#include <variant>
#include <vector>

namespace arser {

    namespace kind {

        struct flag_t    { using type = void; };
        struct boolean_t { using type = bool; };
        struct integer_t { using type = int; };
        struct decimal_t { using type = double; };
        struct string_t  { using type = std::string; };

        using kind_t = std::variant<kind::flag_t, kind::boolean_t, kind::integer_t, kind::decimal_t, kind::string_t>;

    }

    namespace level {

        using level_t = std::size_t;

        constexpr level_t optional = 0;
        constexpr level_t required = 1;

    }

    template <class T>
    static constexpr auto argument_as(auto&& argument)
    {
        return std::get<T>(std::forward<decltype(argument)>(argument).get_value());
    }

    template<class T>
    static constexpr auto argument_is(auto&& argument)
        requires (requires () { typename T::type; })
    {
        return std::holds_alternative<T>(argument.get_kind());
    }

    class argument
    {
        struct empty_t {};

    public:
        using name_t         = std::pair<std::string, std::string>;
        using value_t        = std::variant<empty_t, kind::boolean_t::type, kind::integer_t::type, kind::decimal_t::type, kind::string_t::type>;
        using dependencies_t = std::vector<std::pair<std::string, std::string>>;

        argument() = default;

        argument(std::string_view argumentName, level::level_t argumentLevel):
            name(argumentName, ""), kind(kind::flag_t{}), level(argumentLevel)
        {}

        argument(std::string_view argumentName, std::string_view argumentAlias, level::level_t argumentLevel = level::optional):
            name(argumentName, argumentAlias), kind(kind::flag_t{}), level(argumentLevel)
        {}

        argument(std::string_view argumentName, kind::kind_t argumentKind = kind::flag_t{}, level::level_t argumentLevel = level::optional):
            name(argumentName, ""), kind(argumentKind), level(argumentLevel)
        {}

        argument(std::string_view argumentName, std::string_view argumentAlias, kind::kind_t argumentKind, level::level_t argumentLevel = level::optional):
            name(argumentName, argumentAlias), kind(argumentKind), level(argumentLevel)
        {}

        name_t const& get_name() const noexcept;
        kind::kind_t const& get_kind() const noexcept;
        level::level_t const& get_level() const noexcept;
        value_t const& get_value() const noexcept;
        dependencies_t const& get_dependencies() const noexcept;

        auto set_value(auto const& newValue) noexcept { this->value = newValue; }

        bool has_dependencies() const noexcept;
        bool has_value() const noexcept;

        bool is_required() const noexcept;
        bool is_optional() const noexcept;

        argument& depends_on(std::string_view dependencyName, std::string_view dependencyAlias = "");

        bool operator==(argument const& lhs) const noexcept;

    private:
        name_t name;
        kind::kind_t kind {};
        level::level_t level {};

        value_t value {};

        dependencies_t dependencies {};
    };

}
