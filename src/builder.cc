#include "builder.hh"
#include <cstdint>
#include <string>
#include <string_view>

namespace lexi {
builder::builder() : buf(std::vector<uint8_t>()) {}

builder_out builder::out() {
    builder_out out = {this->buf.data(), this->buf.size()};
    return out;
}

builder builder::add_ping() {
    this->buf.push_back('+');
    this->buf.push_back('P');
    this->buf.push_back('I');
    this->buf.push_back('N');
    this->buf.push_back('G');
    this->add_end();
    return *this;
}

builder builder::add_array(size_t len) {
    this->buf.push_back('*');
    this->add_len(len);
    this->add_end();
    return *this;
}

builder builder::add_string(const char* str, size_t str_len) {
    size_t i;
    this->buf.push_back('$');
    this->add_len(str_len);
    this->add_end();
    for (i = 0; i < str_len; ++i) {
        this->buf.push_back(str[i]);
    }
    this->add_end();
    return *this;
}

builder builder::add_string(const std::string& str) {
    return this->add_string(str.c_str(), str.length());
}

builder builder::add_string(std::string_view& str) {
    size_t len = str.length();
    this->buf.push_back('$');
    this->add_len(len);
    this->add_end();
    for (auto ch : str) {
        this->buf.push_back(ch);
    }
    this->add_end();
    return *this;
}

builder builder::add_int(int64_t integer) {
    this->buf.push_back(':');
    uint8_t i, shift = 56;
    for (i = 0; i < 8; ++i, shift -= 8) {
        uint8_t cur = (integer >> shift);
        this->buf.push_back(cur);
    }
    this->add_end();
    return *this;
}

builder builder::reset() {
    this->buf.clear();
    return *this;
}

void builder::add_len(size_t len) {
    std::string len_string = std::to_string(len);
    for (auto ch : len_string) {
        this->buf.push_back(ch);
    }
}

void builder::add_end() {
    this->buf.push_back('\r');
    this->buf.push_back('\n');
}
} // namespace lexi
