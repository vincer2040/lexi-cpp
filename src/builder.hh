#pragma once

#include <cstdint>
#include <string>

class Builder {
    public:
        Builder(void);
        int add_string(std::string str);
        int add_int(std::int64_t integer);
        int add_arr(std::size_t len);
        const std::size_t length(void);
        std::uint8_t* out(void);
        void reset(void);
        void free_buf(void);
    private:
        std::size_t ins;
        std::size_t cap;
        void inline add_byte(std::uint8_t byte);
        void inline add_end(void);
        void inline add_len(std::string len);
        int realloc_buf(std::size_t needed);
        std::uint8_t* buf;
};
