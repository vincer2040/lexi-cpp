#pragma once

#include "lexi_data.hh"
#include <cstdint>
#include <optional>
#include <vector>

namespace lexi {
class parser {
  public:
    parser(std::vector<uint8_t>& input, size_t input_len);
    lexi_data parse();

  private:
    std::vector<uint8_t>& input;
    size_t pos;
    size_t input_len;
    uint8_t byte;

    lexi_data parse_data();
    lexi_data parse_simple();
    lexi_data parse_string();
    lexi_data parse_int();
    lexi_data parse_double();
    lexi_data parse_array();
    lexi_data parse_error();

    std::string read_string();
    std::string read_string(uint64_t len);

    std::optional<lexi::simple_string> lookup_simple(const std::string& str);

    uint64_t parse_len();

    void read_byte();
    uint8_t peek_byte();
    inline bool cur_byte_is(uint8_t byte);
    inline bool peek_byte_is(uint8_t byte);
    bool expect_peek(uint8_t byte);
    bool expect_peek_to_be_num();
};
} // namespace lexi
