#include "../src/lexi.hh"
#include <iostream>

void print(LexiType& res) {
    switch (res.type) {
    case LexiTypeT::Simple: {
        std::string simple = std::get<std::string>(res.data);
        std::cout << simple << "\n";
    } break;
    case LexiTypeT::Bulk: {
        std::string bulk = std::get<std::string>(res.data);
        std::cout << bulk << "\n";
    } break;
    case LexiTypeT::Int: {
        std::int64_t integer = std::get<std::int64_t>(res.data);
        std::cout << integer << "\n";
    } break;
    default:
        std::cout << static_cast<int>(res.type) << "\n";
        break;
    }
}

int main() {
    Lexi lexi = Lexi("127.0.0.1", 6969);
    LexiType res;
    lexi.connect();

    res = lexi.ping();
    print(res);

    res = lexi.set("vince", "is cool");
    print(res);

    res = lexi.get("vince");
    print(res);

    res = lexi.set("vince", 42069);
    print(res);

    res = lexi.del("vince");
    print(res);

    res = lexi.cluster_new("fam");
    print(res);

    res = lexi.cluster_set("fam", "vince", "is cool");
    print(res);

    res = lexi.cluster_get("fam", "vince");
    print(res);

    res = lexi.cluster_set("fam", "vince", 42069);
    print(res);

    res = lexi.cluster_get("fam", "vince");
    print(res);

    res = lexi.cluster_del("fam", "vince");
    print(res);

    res = lexi.cluster_drop("fam");
    print(res);
    return 0;
}
