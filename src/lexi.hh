#pragma once

#include "builder.hh"
#include "lexi_data.hh"
#include <cstdint>
#include <string>
#include <string_view>
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
    lexi_data set(const std::string& key, std::string_view value);
    lexi_data set(const std::string& key, int64_t value);
    lexi_data set(const std::string& key, double value);
    lexi_data set(std::string_view key, const std::string& value);
    lexi_data set(std::string_view key, std::string_view value);
    lexi_data set(std::string_view key, int64_t value);
    lexi_data set(std::string_view key, double value);

    lexi_data get(const std::string& key);
    lexi_data get(std::string_view key);

    lexi_data del(const std::string& key);
    lexi_data del(std::string_view key);

    lexi_data push(const std::string& value);
    lexi_data push(std::string_view value);
    lexi_data push(int64_t value);
    lexi_data push(double value);

    lexi_data pop();

    lexi_data enque(const std::string& value);
    lexi_data enque(std::string_view value);
    lexi_data enque(int64_t value);
    lexi_data enque(double value);

    lexi_data deque();

    lexi_data zset(const std::string& value);
    lexi_data zset(std::string_view value);
    lexi_data zset(int64_t value);
    lexi_data zset(double value);

    lexi_data zhas(const std::string& value);
    lexi_data zhas(std::string_view value);
    lexi_data zhas(int64_t value);
    lexi_data zhas(double value);

    lexi_data zdel(const std::string& value);
    lexi_data zdel(std::string_view value);
    lexi_data zdel(int64_t value);
    lexi_data zdel(double value);

  private:
    int sfd;
    uint32_t addr;
    uint16_t port;
    builder b;
    std::vector<uint8_t> read_buf;

    lexi_data parse(size_t len);

    ssize_t read_from_db();
    ssize_t write_to_db(std::vector<uint8_t>& buf);

    std::string get_error(const std::string& err);
};
} // namespace lexi
