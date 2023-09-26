#include "../src/builder.hh"
#include "../src/lexer.hh"
#include "../src/parser.hh"
#include "../src/token.hh"
#include <cstdint>
#include <gtest/gtest.h>
#include <variant>

TEST(Parser, BulkStrings) {
    std::uint8_t* input = (std::uint8_t*)"$5\r\nvince\r\n";
    std::size_t input_len = strlen((char*)input);
    Lexer l = Lexer(input, input_len);
    Parser p = Parser(l);
    LexiType res = p.parse();
    EXPECT_EQ(res.type, LexiTypeT::Bulk);
    EXPECT_EQ(std::holds_alternative<std::string>(res.data), true);
    std::string data = std::get<std::string>(res.data);
    EXPECT_EQ(data, "vince");
}

TEST(Parser, Integers) {
    Builder b;
    std::uint8_t* out;
    std::size_t out_len;
    b.add_int(42069);
    out = b.out();
    out_len = b.length();

    Lexer l = Lexer(out, out_len);
    Parser p = Parser(l);
    LexiType res = p.parse();
    EXPECT_EQ(res.type, LexiTypeT::Int);
    EXPECT_EQ(std::holds_alternative<std::int64_t>(res.data), true);
    std::int64_t data = std::get<std::int64_t>(res.data);
    EXPECT_EQ(data, 42069);
    free(out);
}

TEST(Parser, Arrays) {
    std::uint8_t* input =
        (std::uint8_t*)"*2\r\n$5\r\nvince\r\n$7\r\nis cool\r\n";
    std::size_t input_len = strlen((char*)input);
    Lexer l = Lexer(input, input_len);
    Parser p = Parser(l);
    LexiType res = p.parse();
    EXPECT_EQ(res.type, LexiTypeT::Array);
    EXPECT_EQ(std::holds_alternative<std::vector<LexiType>>(res.data), true);
    std::vector<LexiType> got = std::get<std::vector<LexiType>>(res.data);
    EXPECT_EQ(got.size(), 2);

    LexiType first = got[0];
    LexiType second = got[1];

    EXPECT_EQ(first.type, LexiTypeT::Bulk);
    EXPECT_EQ(second.type, LexiTypeT::Bulk);

    EXPECT_EQ(std::holds_alternative<std::string>(first.data), true);
    EXPECT_EQ(std::holds_alternative<std::string>(second.data), true);

    std::string s1 = std::get<std::string>(first.data);
    std::string s2 = std::get<std::string>(second.data);

    EXPECT_EQ(s1, "vince");
    EXPECT_EQ(s2, "is cool");
}
