#include "builder.hh"
#include <cstring>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <string>

#define BUILDER_INITIAL_CAP 32

Builder::Builder(void) {
    this->ins = 0;
    this->cap = BUILDER_INITIAL_CAP;
    this->buf =
        (std::uint8_t*)calloc(BUILDER_INITIAL_CAP, sizeof(std::uint8_t));
}

std::uint8_t* Builder::out() { return this->buf; }

const std::size_t Builder::length() { return this->ins; }

void Builder::reset(void) {
    memset(this->buf, 0, this->ins);
    this->ins = 0;
}

void Builder::free_buf(void) {
    if (this->buf) {
        free(this->buf);
    }
}

int Builder::add_arr(std::size_t len) {
    std::string len_to_string = std::to_string(len);
    std::size_t needed_len = this->ins + len_to_string.length() + 3;
    if (needed_len >= this->cap) {
        if (this->realloc_buf(needed_len) != 0) {
            return -1;
        }
    }
    this->add_byte('*');
    this->add_len(len_to_string);
    this->add_end();
    return 0;
}

int Builder::add_string(const char* str) {
    std::size_t i, s_len = strlen(str);
    std::string len_to_string = std::to_string(s_len);
    std::size_t needed_len = this->ins + len_to_string.length() + s_len + 5;
    if (needed_len >= this->cap) {
        if (this->realloc_buf(needed_len) != 0) {
            return -1;
        }
    }
    this->add_byte('$');
    this->add_len(len_to_string);
    this->add_end();
    for (i = 0; i < s_len; ++i) {
        char c = str[i];
        this->add_byte(c);
    }
    this->add_end();
    return 0;
}

int Builder::add_string(const std::string& s) {
    std::size_t s_len = s.length();
    std::string len_to_string = std::to_string(s_len);
    std::size_t needed_len = this->ins + len_to_string.length() + s_len + 5;
    if (needed_len >= this->cap) {
        if (this->realloc_buf(needed_len) != 0) {
            return -1;
        }
    }
    this->add_byte('$');
    this->add_len(len_to_string);
    this->add_end();
    for (auto c : s) {
        this->add_byte(c);
    }
    this->add_end();
    return 0;
}

int Builder::add_int(std::int64_t integer) {
    std::size_t needed_len = this->ins + 11;
    std::uint8_t shift = 56;
    std::size_t i;
    if (needed_len >= this->cap) {
        if (this->realloc_buf(needed_len) != 0) {
            return -1;
        }
    }
    this->add_byte(':');

    for (i = 0; i < 8; ++i, shift -= 8) {
        std::uint8_t at = integer >> shift;
        this->add_byte(at);
    }

    this->add_end();
    return 0;
}

int Builder::add_ping(void) {
    std::size_t needed_len = 7;
    if (needed_len >= this->cap) {
        if (this->realloc_buf(needed_len) != 0) {
            return -1;
        }
    }
    this->add_byte('+');
    this->add_byte('P');
    this->add_byte('I');
    this->add_byte('N');
    this->add_byte('G');
    this->add_byte('\r');
    this->add_byte('\n');
    return 0;
}

void inline Builder::add_byte(std::uint8_t byte) {
    this->buf[this->ins] = byte;
    this->ins += 1;
}

void inline Builder::add_end(void) {
    this->add_byte('\r');
    this->add_byte('\n');
}

void inline Builder::add_len(std::string len) {
    for (auto c : len) {
        this->add_byte(c);
    }
}

int Builder::realloc_buf(std::size_t needed) {
    std::size_t new_cap = this->cap + needed;
    void* tmp = realloc(this->buf, new_cap);
    if (tmp == NULL) {
        return -1;
    }
    this->buf = (std::uint8_t*)tmp;
    memset(this->buf + this->ins, 0, new_cap - ins);
    this->cap = new_cap;
    return 0;
}
