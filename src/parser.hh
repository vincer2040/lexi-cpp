#pragma once
#include "lexi_data.hh"
#include <cstddef>
#include <cstdint>
#include <optional>

namespace lexi {
class parser {
  public:
    parser(const uint8_t* input, size_t input_len);
    lexi_data parse();
  private:
    const uint8_t* input;
    size_t input_len;
    size_t pos;
    uint8_t byte;

    lexi_data parse_data();
    lexi_data parse_simple();
    lexi_data parse_string();
    lexi_data parse_int();
    lexi_data parse_error();
    lexi_data parse_array();

    size_t parse_len();
    std::string read_string();
    std::string read_string(size_t len);
    std::optional<simple_string> lookup_simple(const std::string& str);

    void read_byte();
    uint8_t peek_byte();
    inline bool cur_byte_is(uint8_t byte);
    inline bool peek_byte_is(uint8_t byte);
    bool expect_peek_byte_to_be_num();
    bool expect_peek(uint8_t byte);
};
} // namespace lexi
