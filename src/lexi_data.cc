#include "lexi_data.hh"

namespace lexi {
bool lexi_data::operator==(lexi_data& d) {
    bool res = false;
    if (this->type != d.type) {
        return res;
    }
    switch (this->type) {
    case lexi_data_type::Invalid:
        res = true;
        break;
    case lexi_data_type::Simple: {
        auto this_simple = std::get<simple_string>(this->data);
        auto d_simple = std::get<simple_string>(d.data);
        res = this_simple == d_simple;
    } break;
    case lexi_data_type::String: {
        auto& this_string = std::get<std::string>(this->data);
        auto& d_string = std::get<std::string>(d.data);
        res = this_string == d_string;
    } break;
    case lexi_data_type::Int: {
        auto this_int = std::get<int64_t>(this->data);
        auto d_int = std::get<int64_t>(d.data);
        res = this_int == d_int;
    } break;
    case lexi_data_type::Double: {
        auto this_double = std::get<double>(this->data);
        auto d_double = std::get<double>(d.data);
        res = this_double == d_double;
    } break;
    case lexi_data_type::Array: {
        auto& this_vec = std::get<std::vector<lexi_data>>(this->data);
        auto& d_vec = std::get<std::vector<lexi_data>>(d.data);
        res = this_vec == d_vec;
    } break;
    case lexi_data_type::Error: {
        auto& this_string = std::get<std::string>(this->data);
        auto& d_string = std::get<std::string>(d.data);
        res = this_string == d_string;
    } break;
    }
    return res;
}

std::ostream& operator<<(std::ostream& stream, lexi_data& data) {
    switch (data.type) {
    case lexi_data_type::Invalid:
        stream << "";
        break;
    case lexi_data_type::Simple: {
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
    case lexi_data_type::String: {
        std::string& s = std::get<std::string>(data.data);
        stream << "\"" << s << "\"";
    } break;
    case lexi_data_type::Int: {
        int64_t integer = std::get<int64_t>(data.data);
        stream << "(int) " << integer;
    } break;
    case lexi_data_type::Array: {
        std::vector<lexi_data>& arr =
            std::get<std::vector<lexi_data>>(data.data);
        size_t i, len = arr.size();
        for (i = 0; i < len; ++i) {
            if (i != len - 1) {
                lexi_data& cur = arr[i];
                stream << cur;
                stream << '\n';
            }
        }
    } break;
    case lexi_data_type::Error: {
        std::string& e = std::get<std::string>(data.data);
        stream << e;
    } break;
    case lexi_data_type::Double: {
        auto dbl = std::get<double>(data.data);
        stream << dbl;
    } break;
    }
    return stream;
}
} // namespace lexi
