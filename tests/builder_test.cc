#include "../src/builder.hh"
#include <gtest/gtest.h>

#define arr_size(arr) sizeof arr / sizeof arr[0]

struct string_test {
    const char* str;
    size_t str_len;
    const char* exp;
    size_t exp_len;
};

struct cpp_string_test {
    std::string str;
    const char* exp;
    size_t exp_len;
};

TEST(Builder, Ping) {
    lexi::builder b;
    lexi::builder& x = b.add_ping();
    std::vector<uint8_t>& buf = x.out();
    uint8_t exps[] = {
        '+', 'P', 'I', 'N', 'G', '\r', '\n',
    };
    size_t i, len = arr_size(exps);
    EXPECT_EQ(len, buf.size());
    for (i = 0; i < len; ++i) {
        uint8_t got = buf[i];
        uint8_t exp = exps[i];
        EXPECT_EQ(got, exp);
    }
    EXPECT_EQ(1, 1);
}

TEST(Builder, AddString) {
    string_test tests[] = {
        {
            "foo",
            3,
            "$3\r\nfoo\r\n",
            9,
        },
        {
            "foobarbazfoo",
            12,
            "$12\r\nfoobarbazfoo\r\n",
            19,
        },
    };

    size_t i, len = arr_size(tests);

    for (i = 0; i < len; ++i) {
        lexi::builder b;
        string_test t = tests[i];
        lexi::builder& x = b.add_string(t.str, t.str_len);
        std::vector<uint8_t> buf = x.out();
        size_t j, e_len = t.exp_len;
        EXPECT_EQ(e_len, buf.size());
        for (j = 0; j < e_len; ++j) {
            uint8_t got = buf[j];
            uint8_t exp = t.exp[j];
            EXPECT_EQ(got, exp);
        }
    }
}

TEST(Builder, CppStr) {
    cpp_string_test tests[] = {
        {
            "foo",
            "$3\r\nfoo\r\n",
            9,
        },
        {
            "foobarbazfoo",
            "$12\r\nfoobarbazfoo\r\n",
            19,
        },
    };

    size_t i, len = arr_size(tests);

    for (i = 0; i < len; ++i) {
        lexi::builder b;
        cpp_string_test t = tests[i];
        lexi::builder& x = b.add_string(t.str);
        std::vector<uint8_t> buf = x.out();
        size_t j, e_len = t.exp_len;
        EXPECT_EQ(e_len, buf.size());
        for (j = 0; j < e_len; ++j) {
            uint8_t got = buf[j];
            uint8_t exp = t.exp[j];
            EXPECT_EQ(got, exp);
        }
    }
}
