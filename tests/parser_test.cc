#include "../src/builder.hh"
#include "../src/lexer.hh"
#include "../src/token.hh"
#include "../src/parser.hh"
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
