#pragma once

#include "token.hh"
#include <cstddef>
#include <cstdint>

class Lexer {
  public:
    Lexer(std::uint8_t* input, std::size_t input_len);
    Token next_token(void);

  private:
    std::uint8_t* input;
    std::size_t pos;
    std::size_t input_len;
    std::uint8_t ch;
    void read_char(void);
    std::string read_len(void);
    std::string read_string(void);
};
