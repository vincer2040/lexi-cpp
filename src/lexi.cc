#include "lexi.hh"
#include "lexer.hh"
#include "lexitype.hh"
#include "parser.hh"
#include <variant>

Lexi::Lexi(const std::string& addr_str, std::uint16_t port)
    : sock(Socket(addr_str, port)) {}

Lexi::Lexi(const char* addr_str, std::uint16_t port)
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::set(const std::string& key, const std::string& data) {
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::set(const std::string& key, std::int64_t data) {
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::get(const std::string& key) {
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::del(const std::string& key) {
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::push(const std::string& data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("PUSH");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::push(std::int64_t data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("PUSH");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::pop(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_string("POP");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::keys(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_string("KEYS");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::values(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_string("VALUES");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::entries(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_string("ENTRIES");
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_new(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.NEW");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_set(const std::string& cluster_name,
                           const std::string& key, const std::string& data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(4);
    this->builder.add_string("CLUSTER.SET");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_set(const std::string& cluster_name,
                           const std::string& key, std::int64_t data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(4);
    this->builder.add_string("CLUSTER.SET");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_get(const std::string& cluster_name,
                           const std::string& key) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("CLUSTER.GET");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_del(const std::string& cluster_name,
                           const std::string& key) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("CLUSTER.DEL");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_push(const std::string& cluster_name,
                            const std::string& data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("CLUSTER.PUSH");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_push(const std::string& cluster_name,
                            std::int64_t data) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(3);
    this->builder.add_string("CLUSTER.PUSH");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_pop(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.POP");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}
LexiType Lexi::cluster_keys(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.KEYS");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_values(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.VALUES");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_entries(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.ENTRIES");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::cluster_drop(const std::string& cluster_name) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    ssize_t read_len;
    this->builder.reset();
    this->builder.add_arr(2);
    this->builder.add_string("CLUSTER.DROP");
    this->builder.add_string(cluster_name);
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
    return this->parse(read_buf, read_len);
}

LexiType Lexi::parse(std::uint8_t* read_buf, std::size_t read_buf_len) {
    Lexer l(read_buf, read_buf_len);
    Parser p(l);
    return p.parse();
}

Lexi::~Lexi(void) { this->builder.free_buf(); }
