#include "../src/lexer.hh"
#include "../src/token.hh"
#include <cstdint>
#include <gtest/gtest.h>
#include <variant>

TEST(Lexer, BulkStrings) {
    std::uint8_t* input = (std::uint8_t*)"$5\r\nvince\r\n";
    Lexer l = Lexer(input, strlen((char*)input));
    Token exps[] = {
        {TokenT::BulkType, std::monostate()},
        {TokenT::Len, "5"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Bulk, "vince"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Eof, std::monostate()},
    };
    std::size_t i, len = sizeof(exps) / sizeof(exps[0]);

    for (i = 0; i < len; ++i) {
        Token exp = exps[i];
        Token got = l.next_token();
        EXPECT_EQ(exp.type, got.type);
        if ((exp.type == TokenT::Len) || (exp.type == TokenT::Bulk)) {
            EXPECT_EQ(std::holds_alternative<std::string>(exp.literal), true);
            std::string exp_val = std::get<std::string>(exp.literal);
            std::string got_val = std::get<std::string>(got.literal);
            EXPECT_EQ(exp_val, got_val);
        } else {
            EXPECT_EQ(std::holds_alternative<std::monostate>(exp.literal),
                      true);
        }
    }
}

TEST(Lexer, Arrays) {
    std::uint8_t* input = (std::uint8_t*)"*1\r\n$5\r\nvince\r\n";
    Lexer l = Lexer(input, strlen((char*)input));
    Token exps[] = {
        {TokenT::ArrayType, std::monostate()},
        {TokenT::Len, "1"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::BulkType, std::monostate()},
        {TokenT::Len, "5"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Bulk, "vince"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Eof, std::monostate()},
    };
    std::size_t i, len = sizeof(exps) / sizeof(exps[0]);

    for (i = 0; i < len; ++i) {
        Token exp = exps[i];
        Token got = l.next_token();
        EXPECT_EQ(exp.type, got.type);
        if ((exp.type == TokenT::Len) || (exp.type == TokenT::Bulk)) {
            EXPECT_EQ(std::holds_alternative<std::string>(exp.literal), true);
            std::string exp_val = std::get<std::string>(exp.literal);
            std::string got_val = std::get<std::string>(got.literal);
            EXPECT_EQ(exp_val, got_val);
        } else {
            EXPECT_EQ(std::holds_alternative<std::monostate>(exp.literal),
                      true);
        }
    }
}
