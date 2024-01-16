#include "lexi.hh"
#include "builder.hh"
#include "networking.hh"
#include "parser.hh"
#include <cerrno>
#include <cstring>
#include <string>
#include <unistd.h>

namespace lexi {
client::client(const char* addr, uint16_t port)
    : sfd(-1), addr(parse_addr(addr)), port(port),
      read_buf(std::vector<uint8_t>()) {
    this->sfd = create_tcp_socket(0);
    if (this->sfd == -1) {
        std::string err = this->get_error("failed to create socket");
        throw std::runtime_error(err);
    }
    this->read_buf.resize(4096);
}

client::~client() { close(this->sfd); }

void client::connect() {
    int c = tcp_connect(this->sfd, this->addr, this->port);
    if (c == -1) {
        std::string err = this->get_error("failed to connect");
        throw std::runtime_error(err);
    }
}

lexi_data client::authenticate(const std::string& username,
                               const std::string& password) {
    builder b;
    builder& x =
        b.add_arr(3).add_string("AUTH", 4).add_string(username).add_string(
            password);
    std::vector<uint8_t>& buf = x.out();
    ssize_t w = this->write_to_db(buf);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::set(const std::string& key, const std::string& value) {
    builder b;
    builder& x =
        b.add_arr(3).add_string("SET", 3).add_string(key).add_string(value);
    std::vector<uint8_t>& out = x.out();
    ssize_t w = this->write_to_db(out);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::set(const std::string& key, int64_t value) {
    builder b;
    builder& x =
        b.add_arr(3).add_string("SET", 3).add_string(key).add_int(value);
    std::vector<uint8_t>& out = x.out();
    ssize_t w = this->write_to_db(out);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::set(const std::string& key, double value) {
    builder b;
    builder& x =
        b.add_arr(3).add_string("SET", 3).add_string(key).add_double(value);
    std::vector<uint8_t>& out = x.out();
    ssize_t w = this->write_to_db(out);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::get(const std::string& key) {
    builder b;
    builder& x = b.add_arr(2).add_string("GET", 3).add_string(key);
    std::vector<uint8_t>& out = x.out();
    ssize_t w = this->write_to_db(out);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::del(const std::string& key) {
    builder b;
    builder& x = b.add_arr(2).add_string("DEL", 3).add_string(key);
    std::vector<uint8_t>& out = x.out();
    ssize_t w = this->write_to_db(out);
    if (w == -1) {
        std::string err = this->get_error("failed to write");
        throw std::runtime_error(err);
    }
    ssize_t r = this->read_from_db();
    if (r == -1) {
        std::string err = this->get_error("failed to read");
        throw std::runtime_error(err);
    }
    lexi_data parsed = this->parse(r);
    return parsed;
}

lexi_data client::parse(size_t len) {
    parser p(this->read_buf, len);
    return p.parse();
}

ssize_t client::write_to_db(std::vector<uint8_t>& buf) {
    ssize_t amt = write(this->sfd, buf.data(), buf.size());
    return amt;
}

ssize_t client::read_from_db() {
    memset(this->read_buf.data(), 0, 4096);
    ssize_t amt = read(this->sfd, this->read_buf.data(), 4096);
    return amt;
}

std::string client::get_error(const std::string& err) {
    std::string res = err + " " + "(errno " + std::to_string(errno) + ") " +
                      std::string(strerror(errno));
    return res;
}
} // namespace lexi
