#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

enum class LexiTypeT {
    Simple,
    Bulk,
    Err,
    Int,
    Array,
    Invalid,
};

struct LexiType {
    LexiTypeT type;
    std::variant<std::monostate, std::string, std::int64_t, std::vector<LexiType>> data;
};
