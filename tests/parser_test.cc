#include "../src/lexi_data.hh"
#include "../src/parser.hh"
#include <gtest/gtest.h>

struct string_test {
    std::vector<uint8_t> input;
    const char* exp;
};

#define arr_size(arr) sizeof arr / sizeof arr[0]

TEST(Parser, ParseStrings) {
    string_test tests[] = {
        {
            {'$', '3', '\r', '\n', 'f', 'o', 'o', '\r', '\n'},
            "foo",
        },
        {
            {'$', '1', '2', '\r', '\n', 'f', 'o', 'o', 'b', 'a', 'r', 'b', 'a',
             'z', 'f', 'o', 'o', '\r', '\n'},
            "foobarbazfoo",
        },
        {
            {'$', '5', '\r', '\n', 'f', 'o', 'o', '\r', '\n', '\r', '\n'},
            "foo\r\n",
        },
    };
    size_t i, len = arr_size(tests);
    for (i = 0; i < len; ++i) {
        string_test t = tests[i];
        lexi::parser p(t.input, t.input.size());
        lexi::lexi_data parsed = p.parse();
        EXPECT_EQ(parsed.type, lexi::lexi_data_type::String);
        auto s = std::get<std::string>(parsed.data);
        EXPECT_STREQ(t.exp, s.c_str());
    }
}

TEST(Parser, ParseArrays) {
    std::vector<uint8_t> input = {'*',  '2', '\r', '\n', '$',  '3', '\r', '\n',
                                  'f',  'o', 'o',  '\r', '\n', '$', '3',  '\r',
                                  '\n', 'b', 'a',  'r',  '\r', '\n'};
    lexi::parser p(input, input.size());
    lexi::lexi_data parsed = p.parse();
    EXPECT_EQ(parsed.type, lexi::lexi_data_type::Array);
    const char* exps[] = {
        "foo",
        "bar",
    };
    size_t i, len = arr_size(exps);
    auto v = std::get<std::vector<lexi::lexi_data>>(parsed.data);
    for (i = 0; i < len; ++i) {
        auto& cur = v[i];
        const char* exp = exps[i];
        EXPECT_EQ(cur.type, lexi::lexi_data_type::String);
        auto s = std::get<std::string>(cur.data);
        EXPECT_STREQ(exp, s.c_str());
    }
}
