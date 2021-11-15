#include "parser.h"
#include <fmt/core.h>

void print_interface(const Duck::Interface &interface) {
    fmt::print("#include <memory>\n\n"
               "class {} {{\n"
               "    struct concept_ {{\n",
               interface.name_);

    for (const auto &m : interface.methods_) {
        fmt::print("        virtual {} {}_();\n", m.type_, m.name_);
    }

    fmt::print("    }};\n"
               "\n"
               "    template<typename T> struct model_ : concept_ {{\n"
               "        model_(T t) : value_(t) {{}}\n");

    for (const auto &m : interface.methods_) {
        fmt::print("        {} {}_() override {{ value_.draw(); }}\n", m.type_,
                   m.name_);
    }

    fmt::print("        T value_;\n"
               "    }};\n"
               "\n"
               "public:\n");

    for (const auto &m : interface.methods_) {
        fmt::print("    {} {}() {{ value_->draw_(); }}\n", m.type_, m.name_);
    }

    fmt::print("\n"
               "    template <typename T> Drawable(const T &t) {{\n"
               "        value_ = std::make_unique<model_<T>>(t);\n"
               "    }}\n"
               "\n"
               "private:\n"
               "    std::unique_ptr<concept_> value_;\n"
               "}};\n");
}

void print_line(Duck::Line line) {
    fmt::print("{: >{}} | {}\n", line.number, 5, line.contents_);
    fmt::print("      | {: >{}}\n", "^", line.position + 1);
}

void print_error(const Duck::Error &error) {
    if (error.is<Duck::UnexpectedToken>()) {
        auto e = error.as<Duck::UnexpectedToken>();
        fmt::print(":{}:{}: error: expected '{}'\n", e.line_.number + 1,
                   e.line_.position + 1, e.expected_, e.token_.token_);
        print_line(e.line_);
    }
    if (error.is<Duck::EndOfStream>()) {
        auto e = error.as<Duck::EndOfStream>();
        fmt::print(":{}:{}: error: end of stream\n", e.line_.number + 1,
                   e.line_.position + 1);
        print_line(e.line_);
    }
    if (error.is<Duck::InvalidName>()) {
        auto e = error.as<Duck::InvalidName>();
        fmt::print(":{}:{}: error: invalid name '{}'\n", e.line_.number + 1,
                   e.line_.position + 1, e.token_.token_);
        print_line(e.line_);
    }
}

int main(int argc, char **argv) {
    if (argc != 2)
        return -1;

    Duck::Tokenizer t(argv[1]);
    auto interface = Duck::parse_interface(t);

    if (!interface.valid()) {
        print_error(interface.error());
        return -1;
    }

    print_interface(interface.value());
}
