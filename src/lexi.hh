#pragma once

#include "lexi_data.hh"
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <vector>

namespace lexi {
class client {
  public:
    client(const char* addr, uint16_t port);
    ~client();
    void connect();
    lexi_data authenticate(const std::string& username,
                           const std::string& password);

    lexi_data set(const std::string& key, const std::string& value);
    lexi_data set(const std::string& key, int64_t value);
    lexi_data set(const std::string& key, double value);
    lexi_data get(const std::string& key);
    lexi_data del(const std::string& key);
  private:
    int sfd;
    uint32_t addr;
    uint16_t port;
    std::vector<uint8_t> read_buf;

    lexi_data parse(size_t len);

    ssize_t read_from_db();
    ssize_t write_to_db(std::vector<uint8_t>& buf);

    std::string get_error(const std::string& err);
};
} // namespace lexi
