#include "../src/lexer.hh"
#include "../src/token.hh"
#include <cstdint>
#include <gtest/gtest.h>

TEST(Lexer, BulkStrings) {
    std::uint8_t* input = (std::uint8_t*)"$5\r\nvince\r\n";
    Lexer l = Lexer(input, 11);
    Token exps[] = {
        { TokenT::BulkType, std::monostate() },
        { TokenT::Len, "5" },
        { TokenT::Retcar, std::monostate() },
        { TokenT::NewL, std::monostate() },
        { TokenT::Bulk, "vince" },
        { TokenT::Retcar, std::monostate() },
        { TokenT::NewL, std::monostate() },
        { TokenT::Eof, std::monostate() },
    };
    std::size_t i, len = sizeof(exps) / sizeof(exps[0]);

    for (i = 0; i < len; ++i) {
        Token exp = exps[i];
        Token got = l.next_token();
        EXPECT_EQ(exp.type, got.type);
    }
}
