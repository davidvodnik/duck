#include "fmt/format.h"
#include <generate_interface.h>
#include <parser.h>
#include <print_error.h>

#ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

namespace Binding {

std::string generate_interface(std::string input) {
    Duck::Tokenizer t(input);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        return print_error(interface.error());
    }

    return ::generate_interface(interface.value());
}

} // namespace Binding

EMSCRIPTEN_BINDINGS(duck) {
    emscripten::function("generate_interface", &Binding::generate_interface);
}

#else

#include <fmt/core.h>

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    Duck::Tokenizer t(argv[1]);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        fmt::print("{}", print_error(interface.error()));
        return -1;
    }

    fmt::print("{}", generate_interface(interface.value()));
}

#endif
