#include <arser/argument_parser.hpp>
#include <arser/argument_register.hpp>

#include <utility>

void correct_dependency_chain(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--C1")) return;

    auto argument = arser::argument_parser::has_argument_with_missing_dependencies(argumentParser);

    if (argument)
    {
        std::exit(1);
    }
}

void incorrect_dependency_chain(arser::argument_parser const& argumentParser)
{
    if (!argumentParser.contains("--C2")) return;

    auto argument = arser::argument_parser::has_argument_with_missing_dependencies(argumentParser);

    if (!argument || argument->first.get_name() != std::pair { "--flagB", "" })
    {
        std::exit(1);
    }
}

int main(int argumentCount, char const** argumentValues)
{
    std::span<char const*> arguments { argumentValues, static_cast<size_t>(argumentCount) };

    arser::argument_register argumentRegister {};

    argumentRegister.register_argument("--C1");
    argumentRegister.register_argument("--C2");

    argumentRegister.register_argument("--flagC");
    argumentRegister.register_argument("--flagB").depends_on("--flagC");
    argumentRegister.register_argument("--flagA").depends_on("--flagB");

    arser::argument_parser argumentParser { argumentRegister };
    argumentParser.parse(arguments | std::views::drop(1));

    correct_dependency_chain(argumentParser);
    incorrect_dependency_chain(argumentParser);
}

