#pragma once

#include <ostream>
#include <string>
#include <variant>
#include <vector>

namespace lexi {
enum class lexi_data_type {
    Invalid,
    Simple,
    String,
    Int,
    Array,
    Error,
};

enum class simple_string {
    Ok,
    None,
    Pong,
};

using lexi_data_t = std::variant<std::monostate, simple_string, std::string,
                                 int64_t, std::vector<struct lexi_data>>;

struct lexi_data {
    lexi_data_type type;
    lexi_data_t data;
};

std::ostream& operator<<(std::ostream& stream, lexi_data& data);
} // namespace lexi
