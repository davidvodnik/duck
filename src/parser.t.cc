#include <catch.hpp>
#include <parser.h>

using namespace Duck;

//Result<int> parse_expected(Tokenizer &t, const std::string &e);
//Result<std::string_view> parse_name(Tokenizer &t);
//Result<int> parse_nested_types(Tokenizer &t);
//Result<Type> parse_type(Tokenizer &t);
//Result<Parameter> parse_parameter(Tokenizer &t);
//Result<std::vector<Parameter>> parse_parameter_list(Tokenizer &t);
//Result<Method> parse_method(Tokenizer &t);
//Result<std::vector<Method>> parse_methods(Tokenizer &t);
//Result<Interface> parse_interface(Tokenizer &t);

TEST_CASE("Parse expected") {
    Tokenizer t("x");
    auto e = parse_expected(t, "y");;
    REQUIRE(e.valid() == false);
    REQUIRE(e.error().is<UnexpectedToken>());
    REQUIRE(e.error().as<UnexpectedToken>().expected_ == "y");
    REQUIRE(e.error().as<UnexpectedToken>().token == "x");
    REQUIRE(parse_expected(t, "x").valid() == true);
    REQUIRE(parse_expected(t, "x").valid() == false);
}

TEST_CASE("Parse type") {
    Tokenizer t("const int & x");
    auto type = parse_type(t);
    auto name = parse_name(t);
    REQUIRE(type.valid());
    REQUIRE(name.valid());
    REQUIRE(!t.valid());
}

TEST_CASE("Parse parameter list") {
    Tokenizer t("(int x)");
    auto m = parse_parameter_list(t);
    REQUIRE(m.valid());
    REQUIRE(m.value().size() == 1);
}

TEST_CASE("Parse method") {
    Tokenizer t("void fun(int x);");
    auto m = parse_method(t);
    REQUIRE(m.valid());
    REQUIRE(m.value().type.name == "void");
    REQUIRE(m.value().name == "fun");
    REQUIRE(m.value().parameters.size() == 1);
}

TEST_CASE("Parse interface") {
    Tokenizer t("struct interface{void fun();};");
    auto i = parse_interface(t);
    REQUIRE(i.valid());
    REQUIRE(i.value().name == "interface");
    REQUIRE(i.value().methods.size() == 1);
}

TEST_CASE("Parse unexpected token") {
    Tokenizer t("xstruct interface{void fun();}");
    auto i = parse_interface(t);
    REQUIRE(!i.valid());
    REQUIRE(i.error().is<UnexpectedToken>());
    REQUIRE(i.error().as<UnexpectedToken>().token == "xstruct");
}
