#include "networking.hh"
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

namespace lexi {
int create_tcp_socket(int non_blocking) {
    if (non_blocking) {
        return socket(AF_INET, (SOCK_STREAM | SOCK_NONBLOCK), 0);
    }
    return socket(AF_INET, SOCK_STREAM, 0);
}

int tcp_connect(int socket, uint32_t addr, uint16_t port) {
    struct sockaddr_in sa = {0};
    socklen_t len = sizeof sa;
    int res;
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = htonl(addr);
    sa.sin_port = htons(port);
    res = connect(socket, (struct sockaddr*)(&sa), len);
    return res;
}

uint32_t parse_addr(const char* addr_str) {
    uint32_t addr = 0;
    uint8_t shift = 24;
    size_t len = strlen(addr_str);
    int8_t t;
    size_t i;
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
}
