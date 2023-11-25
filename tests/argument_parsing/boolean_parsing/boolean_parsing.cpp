#include <arser/argument_parser.hpp>
#include <arser/argument_register.hpp>

void parse_boolean_from_number(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--fromNumber")) return;

    static constexpr auto EXPECTED = true;

    if (arser::argument_as<bool>(argumentParser.get_argument("--fromNumber")) != EXPECTED)
    {
        std::exit(1);
    }
}

void parse_boolean_from_string(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--fromString")) return;

    static constexpr auto EXPECTED = true;

    if (arser::argument_as<bool>(argumentParser.get_argument("--fromString")) != EXPECTED)
    {
        std::exit(1);
    }
}

int main(int argumentCount, char const** argumentValues)
{
    std::span<char const*> arguments { argumentValues, static_cast<size_t>(argumentCount) };

    arser::argument_register argumentRegister {};

    argumentRegister.register_argument("--fromNumber", arser::kind::boolean_t{});
    argumentRegister.register_argument("--fromString", arser::kind::boolean_t{});

    arser::argument_parser argumentParser { argumentRegister };
    argumentParser.parse(arguments | std::views::drop(1));

    parse_boolean_from_number(argumentParser);
    parse_boolean_from_string(argumentParser);
}
