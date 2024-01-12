#pragma once

#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <sys/socket.h>

namespace lexi {

int create_tcp_socket(int non_blocking);
int tcp_connect(int socket, uint32_t addr, uint16_t port);

uint32_t parse_addr(const char* addr_str);

} // namespace lexi
