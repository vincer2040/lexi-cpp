#pragma once

#include <cstdint>
#include <string>
#include <sys/types.h>
#include <tuple>

class Socket {
  public:
    Socket(const std::string& addr_str, std::uint16_t port);
    Socket(const char* addr_str, std::uint16_t port);
    int create_connection(void);
    ssize_t send(std::uint8_t* send_buf, std::size_t send_len);
    std::tuple<std::uint8_t*, ssize_t> receive(void);
    ~Socket(void);

  private:
    std::uint32_t addr;
    std::uint16_t port;
    std::size_t read_buf_cap;
    std::uint8_t* read_buf;
    int fd;
    int allocate_read_buf(void);
    int reallocate_buf(std::size_t cur_pos);
};
