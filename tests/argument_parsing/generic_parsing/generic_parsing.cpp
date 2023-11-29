#include <arser/argument_parser.hpp>
#include <arser/argument_register.hpp>

void parse_integer(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--integer")) return;

    static constexpr auto EXPECTED = 69420;

    if (arser::argument_as<int>(argumentParser.get_argument("--integer")) != EXPECTED)
    {
        std::exit(1);
    }
}

void parse_decimal(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--decimal")) return;

    static constexpr auto EXPECTED = 69.420;

    if (arser::argument_as<double>(argumentParser.get_argument("--decimal")) != EXPECTED)
    {
        std::exit(1);
    }
}

void parse_string(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--string")) return;

    static constexpr auto EXPECTED = "hello, world";

    if (arser::argument_as<std::string>(argumentParser.get_argument("--message")) != EXPECTED)
    {
        std::exit(1);
    }
}

int main(int argumentCount, char const** argumentValues)
{
    std::span<char const*> arguments { argumentValues, static_cast<size_t>(argumentCount) };

    arser::argument_register argumentRegister {};

    argumentRegister.register_argument("--integer", arser::kind::integer_t{});
    argumentRegister.register_argument("--decimal", arser::kind::decimal_t{});
    argumentRegister.register_argument("--message", arser::kind::string_t{});

    arser::argument_parser argumentParser { argumentRegister };
    argumentParser.parse(arguments | std::views::drop(1));

    parse_integer(argumentParser);
    parse_decimal(argumentParser);
    parse_string(argumentParser);
}
