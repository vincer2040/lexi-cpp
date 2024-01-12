#include "lexi_data.hh"

namespace lexi {
std::ostream& operator<<(std::ostream& stream, lexi_data& data) {
    switch (data.type) {
    case lexi_data_type::Invalid:
        stream << "";
        break;
    case lexi_data_type::Simple:{
        simple_string simple = std::get<simple_string>(data.data);
        switch (simple) {
        case simple_string::Ok:
            stream << "ok";
            break;
        case simple_string::None:
            stream << "none";
            break;
        case simple_string::Pong:
            stream << "pong";
            break;
        }
    } break;
    case lexi_data_type::String:{
        std::string& s = std::get<std::string>(data.data);
        stream << s;
    } break;
    case lexi_data_type::Int:{
        int64_t integer = std::get<int64_t>(data.data);
        stream << integer;
    } break;
    case lexi_data_type::Array:{
        std::vector<lexi_data>& arr = std::get<std::vector<lexi_data>>(data.data);
        size_t i, len = arr.size();
        for (i = 0; i < len; ++i) {
            if (i != len - 1) {
                lexi_data& cur = arr[i];
                stream << cur;
                stream << '\n';
            }
        }
    } break;
    case lexi_data_type::Error:{
        std::string& e = std::get<std::string>(data.data);
        stream << e;
    } break;
    }
    return stream;
}
} // namespace lexi
