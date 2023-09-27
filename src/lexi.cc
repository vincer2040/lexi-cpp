#include "lexi.hh"
#include "lexer.hh"
#include "lexitype.hh"
#include "parser.hh"
#include <variant>

Lexi::Lexi(std::string addr_str, std::uint16_t port)
    : sock(Socket(addr_str, port)) {}

int Lexi::connect(void) { return this->sock.create_connection(); }

LexiType Lexi::ping(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_ping();
    buf = this->builder.out();
    buf_len = this->builder.length();
    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    Lexer l = Lexer(read_buf, read_len);
    Parser p = Parser(l);
    return p.parse();
}

LexiType Lexi::set(std::string key, std::string data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("SET");
    this->builder.add_string(key);
    this->builder.add_string(data);
    buf = this->builder.out();
    buf_len = this->builder.length();
    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    Lexer l = Lexer(read_buf, read_len);
    Parser p = Parser(l);
    return p.parse();
}

LexiType Lexi::set(std::string key, std::int64_t data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("SET");
    this->builder.add_string(key);
    this->builder.add_int(data);
    buf = this->builder.out();
    buf_len = this->builder.length();
    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    Lexer l = Lexer(read_buf, read_len);
    Parser p = Parser(l);
    return p.parse();
}

LexiType Lexi::get(std::string key) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("GET");
    this->builder.add_string(key);
    buf = this->builder.out();
    buf_len = this->builder.length();
    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    Lexer l = Lexer(read_buf, read_len);
    Parser p = Parser(l);
    return p.parse();
}

LexiType Lexi::del(std::string key) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("DEL");
    this->builder.add_string(key);
    buf = this->builder.out();
    buf_len = this->builder.length();
    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    Lexer l = Lexer(read_buf, read_len);
    Parser p = Parser(l);
    return p.parse();
}

Lexi::~Lexi(void) { this->builder.free_buf(); }
