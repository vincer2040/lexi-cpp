#include "parser.hh"
#include <cctype>
#include <cstring>
#include <optional>

namespace lexi {

struct simple_lookup {
    const char* str;
    size_t str_len;
    simple_string simple;
};

const simple_lookup simple_lookups[] = {
    {"OK", 2, simple_string::Ok},
    {"NONE", 4, simple_string::None},
    {"PONG", 2, simple_string::Pong},
};

size_t simple_lookups_len = sizeof simple_lookups / sizeof simple_lookups[0];

lexi_data invalid_data = {lexi_data_type::Invalid, std::monostate()};

parser::parser(std::vector<uint8_t>& input, size_t input_len)
    : input(input), pos(0), input_len(input_len), byte(0) {
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
    case '*':
        return this->parse_array();
    case '-':
        return this->parse_error();
    default:
        break;
    }
    return invalid_data;
}

lexi_data parser::parse_simple() {
    this->read_byte();
    std::string s = this->read_string();
    std::optional<simple_string> simple = this->lookup_simple(s);
    if (!simple.has_value()) {
        return invalid_data;
    }
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    return {lexi_data_type::Simple, simple.value()};
}

lexi_data parser::parse_string() {
    this->read_byte();
    if (!this->expect_peek_to_be_num()) {
        return invalid_data;
    }
    uint64_t len = this->parse_len();
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
    uint64_t tmp = 0;
    int64_t res;
    this->read_byte();
    uint8_t i, shift = 56;
    for (i = 0; i < 8; ++i) {
        tmp |= (uint64_t)(((uint64_t)this->byte) << shift);
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

lexi_data parser::parse_array() {
    uint64_t i;
    this->read_byte();
    uint64_t len = parse_len();
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    std::vector<lexi_data> arr;
    for (i = 0; i < len; ++i) {
        lexi_data parsed = this->parse_data();
        if (parsed.type == lexi_data_type::Invalid) {
            return invalid_data;
        }
        arr.push_back(std::move(parsed));
    }
    return {lexi_data_type::Array, std::move(arr)};
}

lexi_data parser::parse_error() {
    this->read_byte();
    std::string err = this->read_string();
    if (!this->cur_byte_is('\r')) {
        return invalid_data;
    }
    if (!this->expect_peek('\n')) {
        return invalid_data;
    }
    this->read_byte();
    return {lexi_data_type::Error, std::move(err)};
}

std::string parser::read_string() {
    std::string res;
    while (this->byte != '\r' && this->byte != 0) {
        res.push_back(this->byte);
        this->read_byte();
    }
    return res;
}

std::string parser::read_string(uint64_t len) {
    uint64_t i;
    std::string res;
    for (i = 0; i < len; ++i) {
        res.push_back(this->byte);
        this->read_byte();
    }
    return res;
}

std::optional<simple_string> parser::lookup_simple(const std::string& str) {
    size_t i;
    for (i = 0; i < simple_lookups_len; ++i) {
        simple_lookup sl = simple_lookups[i];
        if (str.length() != sl.str_len) {
            continue;
        }
        if (memcmp(str.c_str(), sl.str, sl.str_len) != 0) {
            continue;
        }

        return sl.simple;
    }
    return std::nullopt;
}

uint64_t parser::parse_len() {
    uint64_t res = 0;
    while (isdigit(this->byte)) {
        res = (res * 10) + (this->byte - '0');
        this->read_byte();
    }
    return res;
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

bool parser::expect_peek(uint8_t byte) {
    if (this->peek_byte() == byte) {
        this->read_byte();
        return true;
    }
    return false;
}

bool parser::expect_peek_to_be_num() {
    return '0' <= this->byte && this->byte <= '9';
}
} // namespace lexi
