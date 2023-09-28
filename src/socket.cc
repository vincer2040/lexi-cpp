#include "socket.hh"
#include <fcntl.h>
#include <memory.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <sys/socket.h>
#include <unistd.h>

static std::uint32_t parse_addr(std::string addr_str);
static std::uint32_t parse_addr(const char* addr_str, std::size_t len);

Socket::Socket(const std::string& addr_str, std::uint16_t port) {
    this->addr = parse_addr(addr_str);
    this->port = port;
    this->read_buf = NULL;
    this->read_buf_cap = 0;
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(const char* addr_str, std::uint16_t port) {
    this->addr = parse_addr(addr_str, strlen(addr_str));
    this->port = port;
    this->read_buf = NULL;
    this->read_buf_cap = 0;
    this->fd = socket(AF_INET, SOCK_STREAM, 0);
}

int Socket::create_connection(void) {
    struct sockaddr_in a;
    socklen_t al;
    a.sin_family = AF_INET;
    a.sin_port = htons(this->port);
    a.sin_addr.s_addr = htonl(this->addr);
    al = sizeof a;
    return connect(this->fd, ((struct sockaddr*)(&a)), al);
}

ssize_t Socket::send(std::uint8_t* send_buf, std::size_t send_len) {
    return write(this->fd, send_buf, send_len);
}

std::tuple<std::uint8_t*, ssize_t> Socket::receive(void) {
    size_t pos = 0;
    if (!this->read_buf) {
        if (this->allocate_read_buf() == -1) {
            return std::make_tuple(nullptr, -1);
        }
    } else {
        memset(this->read_buf, 0, this->read_buf_cap);
    }
    // this is a pretty hacky way to read everything from the socket
    while (true) {
        ssize_t read_amt =
            read(this->fd, this->read_buf + pos, this->read_buf_cap);
        if (read_amt == -1) {
            return std::make_tuple(nullptr, -1);
            break;
        }
        pos += read_amt;
        if (read_amt == 4096) {
            // we may have more to read
            this->reallocate_buf(pos);
            continue;
        }
        break;
    }
    return std::make_tuple(this->read_buf, pos);
}

Socket::~Socket(void) {
    if (this->read_buf) {
        free(this->read_buf);
    }
}

int Socket::allocate_read_buf(void) {
    std::size_t cap = 4096;
    this->read_buf = (std::uint8_t*)calloc(cap, sizeof(std::uint8_t));
    if (this->read_buf == NULL) {
        return -1;
    }
    this->read_buf_cap = cap;
    return 0;
}

int Socket::reallocate_buf(std::size_t cur_pos) {
    std::size_t needed = this->read_buf_cap + 4096;
    void* tmp = realloc(this->read_buf, needed);
    if (tmp == NULL) {
        return -1;
    }
    this->read_buf = (std::uint8_t*)tmp;
    memset(this->read_buf + cur_pos, 0, 4096);
    return 0;
}

static std::uint32_t parse_addr(std::string addr_str) {
    std::uint32_t addr;
    std::int8_t t;
    std::size_t i, len = addr_str.length();
    std::uint8_t shift;
    addr = 0;
    shift = 24;
    t = 0;
    for (i = 0; i < len; i++) {
        char at;
        at = addr_str[i];
        if (at == '.') {
            addr ^= (((uint32_t)(t)) << shift);
            t = 0;
            shift -= 8;
            continue;
        }
        t = (t * 10) + (at - '0');
    }
    addr ^= ((uint32_t)(t));
    return addr;
}

static std::uint32_t parse_addr(const char* addr_str, std::size_t len) {
    std::uint32_t addr;
    std::int8_t t;
    std::size_t i;
    std::uint8_t shift;
    addr = 0;
    shift = 24;
    t = 0;
    for (i = 0; i < len; i++) {
        char at;
        at = addr_str[i];
        if (at == '.') {
            addr ^= (((uint32_t)(t)) << shift);
            t = 0;
            shift -= 8;
            continue;
        }
        t = (t * 10) + (at - '0');
    }
    addr ^= ((uint32_t)(t));
    return addr;
}
