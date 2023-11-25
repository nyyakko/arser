#include <arser/argument_parser.hpp>
#include <arser/argument_register.hpp>

int main(int argumentCount, char const** argumentValues)
{
    std::span<char const*> arguments { argumentValues, static_cast<size_t>(argumentCount) };

    arser::argument_register argumentRegister {};

    argumentRegister.register_argument("--full", "-f");

    arser::argument_parser argumentParser { argumentRegister };
    argumentParser.parse(arguments | std::views::drop(1));

    if (!argumentParser.contains("--full"))
    {
        std::exit(1);
    }
}
