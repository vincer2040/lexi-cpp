#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace lexi {
class builder {
  public:
    builder();
    std::vector<uint8_t>& out();
    builder& reset();
    builder& add_ping();
    builder& add_string(const char* str, size_t str_len);
    builder& add_string(const std::string& str);
    builder& add_string(std::string_view& str);
    builder& add_arr(size_t len);
    builder& add_int(int64_t integer);
    builder& add_double(double dbl);

  private:
    std::vector<uint8_t> buf;
    void add_len(size_t len);
    void add_end();
};
} // namespace lexi
