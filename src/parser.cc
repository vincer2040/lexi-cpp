#include "parser.hh"
#include "lexi_data.hh"
#include <memory.h>
#include <optional>
#include <string>
#include <variant>

namespace lexi {
struct simple_lookup {
    const char* str;
    size_t str_len;
    simple_string simple;
};

const simple_lookup simple_lookups[] = {
    {"OK", 2, simple_string::Ok},
    {"NONE", 4, simple_string::None},
    {"PONG", 4, simple_string::Pong},
};

size_t simple_lookups_len = sizeof simple_lookups / sizeof simple_lookups[0];

lexi_data invalid_data = {lexi_data_type::Invalid, std::monostate()};

parser::parser(const uint8_t* input, size_t input_len)
    : input(input), input_len(input_len), pos(0), byte(0) {
    this->read_byte();
}

lexi_data parser::parse() { return this->parse_data(); }

lexi_data parser::parse_data() {
    switch (this->byte) {
    case '+':
        return this->parse_simple();
    case '$':
        return this->parse_string();
    case ':':
        return this->parse_int();
    case '-':
        return this->parse_error();
    case '*':
        return this->parse_array();
    default:
        break;
    }
    return invalid_data;
}

lexi_data parser::parse_simple() {
    this->read_byte();
    std::string s = this->read_string();
    std::optional<simple_string> simple_string_option = this->lookup_simple(s);
    if (!simple_string_option.has_value()) {
        return invalid_data;
    }
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    return {lexi_data_type::Simple, simple_string_option.value()};
}

lexi_data parser::parse_string() {
    this->read_byte();
    size_t len = this->parse_len();
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    std::string s = this->read_string(len);
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    return {lexi_data_type::String, std::move(s)};
}

lexi_data parser::parse_int() {
    this->read_byte();
    uint64_t tmp = 0;
    int64_t res;
    uint8_t i, shift = 56;

    for (i = 0; i < 8; ++i, shift -= 8) {
        tmp |= (uint64_t)((uint64_t)this->byte << shift);
        this->read_byte();
    }

    if (tmp <= 0x7fffffffffffffffu) {
        res = tmp;
    } else {
        res = -1 - (int64_t)(0xffffffffffffffffu - tmp);
    }

    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }

    this->read_byte();

    return {lexi_data_type::Int, res};
}

lexi_data parser::parse_error() {
    this->read_byte();
    std::string s = this->read_string();

    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }

    this->read_byte();

    return {lexi_data_type::Error, std::move(s)};
}

lexi_data parser::parse_array() {
    this->read_byte();
    size_t i, len = this->parse_len();

    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }

    this->read_byte();

    std::vector<lexi_data> array;

    for (i = 0; i < len; ++i) {
        lexi_data parsed = this->parse_data();
        if (parsed.type == lexi_data_type::Invalid) {
            return invalid_data;
        }
        array.push_back(std::move(parsed));
    }
    return {lexi_data_type::Array, std::move(array)};
}

size_t parser::parse_len() {
    size_t res;
    std::string s = read_string();
    res = std::stoull(s);
    return res;
}

std::string parser::read_string() {
    std::string res;
    while (this->byte != '\r') {
        res.push_back(this->byte);
        this->read_byte();
    }
    return res;
}

std::string parser::read_string(size_t len) {
    std::string res;
    size_t i;
    for (i = 0; i < len; ++i) {
        res.push_back(this->byte);
        this->read_byte();
    }
    return res;
}

std::optional<simple_string> parser::lookup_simple(const std::string& str) {
    size_t i;
    size_t str_len = str.length();
    const char* str_data = str.c_str();
    for (i = 0; i < simple_lookups_len; ++i) {
        simple_lookup lookup = simple_lookups[i];
        if (str_len != lookup.str_len) {
            continue;
        }
        if (memcmp(str_data, lookup.str, str_len) != 0) {
            continue;
        }
        return lookup.simple;
    }
    return std::nullopt;
}

void parser::read_byte() {
    if (this->pos >= this->input_len) {
        this->byte = 0;
        return;
    }
    this->byte = this->input[this->pos];
    this->pos++;
}

uint8_t parser::peek_byte() {
    if (this->pos >= this->input_len) {
        return 0;
    }
    return this->input[this->pos];
}

inline bool parser::cur_byte_is(uint8_t byte) { return this->byte == byte; }

inline bool parser::peek_byte_is(uint8_t byte) {
    return this->peek_byte() == byte;
}

bool parser::expect_peek_byte_to_be_num() {
    uint8_t peek = this->peek_byte();
    if ('0' <= peek && peek <= '9') {
        this->read_byte();
        return true;
    }
    return false;
}

bool parser::expect_peek(uint8_t byte) {
    if (this->peek_byte() == byte) {
        this->read_byte();
        return true;
    }
    return false;
}
} // namespace lexi
