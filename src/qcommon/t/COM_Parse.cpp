//
// Testing Com_Parse()
//
#include "qcommon/q_shared.h"

#include <cstring>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

void Com_Error( int level, const char *error, ... ) { exit(0); }
void Com_Printf( const char *msg, ... ) {}

namespace {
    struct Deleter {
        void operator()(char* p) { ::free(p); }
    };
    using Tu = std::unique_ptr<char, Deleter>;
    std::ostream& operator<<(std::ostream& os, Tu& up) {
        os << up.get();
        return os;
    }
}

static inline Tu _strdup(std::string s) {
    return Tu { ::strdup(s.c_str()) };
}

TEST_CASE("test")
{
    auto input = _strdup(R"(test)").get();
    std::string token = COM_Parse(&input);
    REQUIRE(token == "test");
}

TEST_CASE("test1 test2")
{
    auto input = _strdup(R"(test1 test2)").get();

    std::string token;
    token = COM_Parse(&input);
    REQUIRE(token == "test1");

    token = COM_Parse(&input);
    REQUIRE(token == "test2");
}

TEST_CASE(R"(a "b")")
{
    auto input = _strdup(R"(a "b")").get();

    std::string token;
    token = COM_Parse(&input);
    REQUIRE(token == R"(a)");

    token = COM_Parse(&input);
    REQUIRE(token == R"(b)");
}

TEST_CASE(R"(a \"b\")")
{
    auto input = _strdup(R"(a \"b\")").get();

    std::string token;
    token = COM_Parse(&input);
    REQUIRE(token == R"(a)");

    token = COM_Parse(&input);
    REQUIRE(token == R"(b)");
}

TEST_CASE(R"(a "\"b\"")")
{
    auto input = _strdup(R"(a "\"b\"")").get();

    std::string token;
    token = COM_Parse(&input);
    REQUIRE(token == R"(a)");

    token = COM_Parse(&input);
    REQUIRE(token == R"("b")");
}

TEST_CASE(R"(a '\"b\"')")
{
    auto input = _strdup(R"(a '\"b\"')").get();

    std::string token;
    token = COM_Parse(&input);
    REQUIRE(token == R"(a)");

    token = COM_Parse(&input);
    REQUIRE(token == R"('\"b\"')");
}
