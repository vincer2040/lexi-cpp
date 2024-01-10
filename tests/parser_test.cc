#include "../src/builder.hh"
#include "../src/parser.hh"
#include <cstring>
#include <gtest/gtest.h>

struct parser_simple_test {
    const uint8_t* input;
    size_t input_len;
    lexi::simple_string exp;
};

struct parser_string_test {
    const uint8_t* input;
    size_t input_len;
    const char* exp;
};

#define arr_size(arr) sizeof arr / sizeof arr[0]

TEST(Parser, ParseSimple) {
    parser_simple_test tests[] = {
        {
            (const uint8_t*)"+OK\r\n",
            5,
            lexi::simple_string::Ok,
        },
        {
            (const uint8_t*)"+NONE\r\n",
            7,
            lexi::simple_string::None,
        },
        {
            (const uint8_t*)"+PONG\r\n",
            7,
            lexi::simple_string::Pong,
        },
    };

    size_t i, len = arr_size(tests);
    for (i = 0; i < len; ++i) {
        parser_simple_test t = tests[i];
        lexi::parser p(t.input, t.input_len);
        lexi::lexi_data data = p.parse();
        EXPECT_EQ(data.type, lexi::lexi_data_type::Simple);
        auto simple = std::get<lexi::simple_string>(data.data);
        EXPECT_EQ(simple, t.exp);
    }
}

TEST(Parser, ParseString) {
    parser_string_test tests[] = {
        {
            (const uint8_t*)"$3\r\nfoo\r\n",
            strlen("$3\r\nfoo\r\n"),
            "foo",
        },
        {
            (const uint8_t*)"$5\r\nfoo\r\n\r\n",
            strlen("$5\r\nfoo\r\n\r\n"),
            "foo\r\n",
        },
        {
            (const uint8_t*)"$12\r\nfoobarbazfoo\r\n",
            strlen("$12\r\nfoobarbazfoor\n"),
            "foobarbazfoo",
        },
    };
    size_t i, len = arr_size(tests);
    for (i = 0; i < len; ++i) {
        parser_string_test t = tests[i];
        lexi::parser p(t.input, t.input_len);
        lexi::lexi_data data = p.parse();
        EXPECT_EQ(data.type, lexi::lexi_data_type::String);
        auto str = std::get<std::string>(data.data);
        EXPECT_STREQ(str.c_str(), t.exp);
    }
}

TEST(Parser, ParseArray) {
    const uint8_t* input = (const uint8_t*)"*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n";
    size_t input_len = strlen("*2\r\n$3\r\nfoo\r\n$3\r\nbar\r\n");
    lexi::parser p(input, input_len);
    lexi::lexi_data data = p.parse();
    EXPECT_EQ(data.type, lexi::lexi_data_type::Array);
    auto arr = std::get<std::vector<lexi::lexi_data>>(data.data);
    EXPECT_EQ(arr.size(), 2);
    const char* exps[] = {
        "foo",
        "bar",
    };
    size_t i, len = arr_size(exps);
    for (i = 0; i < len; ++i) {
        lexi::lexi_data& cur = arr[i];
        const char* exp = exps[i];
        EXPECT_EQ(cur.type, lexi::lexi_data_type::String);
        auto s = std::get<std::string>(cur.data);
        EXPECT_STREQ(s.c_str(), exp);
    }
}

TEST(Parser, ParseInts) {
    lexi::builder b = lexi::builder().add_int(1337);
    lexi::builder_out out = b.out();
    lexi::parser p(out.data, out.len);
    lexi::lexi_data d = p.parse();
    EXPECT_EQ(d.type, lexi::lexi_data_type::Int);
    int64_t i = std::get<int64_t>(d.data);
    EXPECT_EQ(i, 1337);

    lexi::builder b1 = lexi::builder().add_int(-1337);
    lexi::builder_out out1 = b1.out();
    lexi::parser p1(out1.data, out1.len);
    lexi::lexi_data d1 = p1.parse();
    EXPECT_EQ(d1.type, lexi::lexi_data_type::Int);
    int64_t i1 = std::get<int64_t>(d1.data);
    EXPECT_EQ(i1, -1337);
}
