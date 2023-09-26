#pragma once

#include <cstdint>
#include <string>
#include <variant>
#include <vector>

enum class TokenT {
    BulkType,
    ArrayType,
    Len,
    Bulk,
    Int,
    Simple,
    Err,
    Retcar,
    NewL,
    Illegal,
    Eof,
};

struct Token {
    TokenT type;
    std::variant<std::monostate, std::string, std::vector<std::uint8_t>> literal;
};
