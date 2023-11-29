#include <arser/argument_parser.hpp>
#include <arser/argument_register.hpp>

#include <utility>

void missing_two_dependencies(arser::argument_parser const& argumentParser)
{
    if (argumentParser.get_number_of_arguments() != 1) return;

    static constexpr auto EXPECTED = 2;
    auto argument = arser::argument_parser::has_argument_with_missing_dependencies(argumentParser);

    if (!argument || argument->second.size() != EXPECTED)
    {
        std::exit(1);
    }
}

void missing_one_dependency(arser::argument_parser const& argumentParser)
{
    if (argumentParser.get_number_of_arguments() != 2) return;

    static constexpr auto EXPECTED = 1;
    auto argument = arser::argument_parser::has_argument_with_missing_dependencies(argumentParser);

    if (!argument || argument->second.size() != EXPECTED)
    {
        std::exit(1);
    }
}

void missing_no_dependency(arser::argument_parser const& argumentParser)
{
    if (argumentParser.get_number_of_arguments() != 3) return;

    if (arser::argument_parser::has_argument_with_missing_dependencies(argumentParser))
    {
        std::exit(1);
    }
}

int main(int argumentCount, char const** argumentValues)
{
    std::span<char const*> arguments { argumentValues, static_cast<size_t>(argumentCount) };

    arser::argument_register argumentRegister {};

    argumentRegister.register_argument("--flagC");
    argumentRegister.register_argument("--flagB");
    argumentRegister.register_argument("--flagA").depends_on("--flagB").depends_on("--flagC");

    arser::argument_parser argumentParser { argumentRegister };
    argumentParser.parse(arguments | std::views::drop(1));

    missing_two_dependencies(argumentParser);
    missing_one_dependency(argumentParser);
    missing_no_dependency(argumentParser);
}

