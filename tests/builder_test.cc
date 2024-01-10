#include "../src/builder.hh"
#include <gtest/gtest.h>

struct const_char_star_test {
    const char* str;
    size_t str_len;
    const uint8_t* exp;
};

#define arr_size(arr) sizeof arr / sizeof arr[0]

TEST(Builder, AddPing) {
    lexi::builder builder = lexi::builder().add_ping();
    lexi::builder_out out = builder.out();
    EXPECT_EQ(out.len, 7);
    uint8_t exp[] = {'+', 'P', 'I', 'N', 'G', '\r', '\n'};
    EXPECT_STREQ((char*)out.data, (char*)exp);
}

TEST(Builder, AddConstStr) {
    const_char_star_test tests[] = {
        {"foo", 3, (const uint8_t*)"$3\r\nfoo\r\n"},
        {"foobarbazfoo", 12, (const uint8_t*)"$12\r\nfoobarbazfoo\r\n"},
    };
    size_t i, len = arr_size(tests);
    for (i = 0; i < len; ++i) {
        const_char_star_test t = tests[i];
        lexi::builder builder = lexi::builder().add_string(t.str, t.str_len);
        lexi::builder_out out = builder.out();
        EXPECT_STREQ((char*)out.data, (char*)t.exp);
    }
}

