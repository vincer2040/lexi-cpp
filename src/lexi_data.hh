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
    Double,
    Array,
    Error,
};

enum class simple_string {
    Ok,
    None,
    Pong,
};

using lexi_data_t =
    std::variant<std::monostate, simple_string, std::string, int64_t, double,
                 std::vector<struct lexi_data>>;

struct lexi_data {
    lexi_data_type type;
    lexi_data_t data;
    bool operator==(const lexi_data& d);
};

std::ostream& operator<<(std::ostream& stream, lexi_data& data);
} // namespace lexi
