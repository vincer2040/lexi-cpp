#include "../src/builder.hh"
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

TEST(Lexer, SimpleStrings) {
    std::uint8_t* input = (std::uint8_t*)"+PONG\r\n";
    Lexer l = Lexer(input, strlen((char*)input));
    Token exps[] = {
        {TokenT::Simple, "PONG"},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Eof, std::monostate()},
    };
    std::size_t i, len = sizeof(exps) / sizeof(exps[0]);

    for (i = 0; i < len; ++i) {
        Token exp = exps[i];
        Token got = l.next_token();
        EXPECT_EQ(exp.type, got.type);
        if (exp.type == TokenT::Simple) {
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

TEST(Lexer, Integers) {
    Builder b;
    std::uint8_t* out;
    std::size_t out_len;
    b.add_int(42069);
    out = b.out();
    out_len = b.length();
    Lexer l = Lexer(out, out_len);
    std::vector<uint8_t> exp_bytes;
    exp_bytes.push_back(0);
    exp_bytes.push_back(0);
    exp_bytes.push_back(0);
    exp_bytes.push_back(0);
    exp_bytes.push_back(0);
    exp_bytes.push_back(0);
    exp_bytes.push_back(0xa4);
    exp_bytes.push_back(0x55);
    Token exps[] = {
        {TokenT::Int, exp_bytes},
        {TokenT::Retcar, std::monostate()},
        {TokenT::NewL, std::monostate()},
        {TokenT::Eof, std::monostate()},
    };
    std::size_t i, len = sizeof(exps) / sizeof(exps[0]);

    for (i = 0; i < len; ++i) {
        Token exp = exps[i];
        Token got = l.next_token();
        EXPECT_EQ(exp.type, got.type);
        if (exp.type == TokenT::Int) {
            EXPECT_EQ(
                std::holds_alternative<std::vector<std::uint8_t>>(got.literal),
                true);
            std::vector<std::uint8_t> exp_val =
                std::get<std::vector<std::uint8_t>>(exp.literal);
            std::vector<std::uint8_t> got_val =
                std::get<std::vector<std::uint8_t>>(got.literal);
            EXPECT_EQ(exp_val.size(), got_val.size());
            std::size_t i, len = exp_val.size();
            for (i = 0; i < len; ++i) {
                uint8_t exp_at = exp_val[i];
                uint8_t got_at = got_val[i];
                EXPECT_EQ(exp_at, got_at);
            }
        }
    }

    free(out);
}
