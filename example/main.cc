#include "../src/lexi.hh"
#include <iostream>
#include <memory>

void print(const LexiType& res) {
    switch (res.type) {
    case LexiTypeT::Simple: {
        std::shared_ptr<std::string> simple =
            std::get<std::shared_ptr<std::string>>(res.data);
        std::cout << *simple << "\n";
    } break;
    case LexiTypeT::Bulk: {
        std::shared_ptr<std::string> bulk =
            std::get<std::shared_ptr<std::string>>(res.data);
        std::cout << *bulk << "\n";
    } break;
    case LexiTypeT::Int: {
        std::int64_t integer = std::get<std::int64_t>(res.data);
        std::cout << integer << "\n";
    } break;
    case LexiTypeT::Array: {
        std::vector<LexiType> vec = std::get<std::vector<LexiType>>(res.data);
        for (auto val : vec) {
            print(val);
        }
    } break;
    case LexiTypeT::Err: {
        std::shared_ptr<std::string> err =
            std::get<std::shared_ptr<std::string>>(res.data);
        std::cout << *err << "\n";
    } break;
    default:
        std::cout << static_cast<int>(res.type) << "\n";
        break;
    }
}

int main() {
    Lexi lexi("127.0.0.1", 6969);
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

    res = lexi.keys();
    print(res);

    res = lexi.values();
    print(res);

    res = lexi.entries();
    print(res);

    res = lexi.del("vince");
    print(res);

    res = lexi.push("is cool");
    print(res);

    res = lexi.push("vince");
    print(res);

    res = lexi.pop();
    print(res);

    res = lexi.pop();
    print(res);

    res = lexi.enque("vince");
    print(res);

    res = lexi.enque("is cool");
    print(res);

    res = lexi.deque();
    print(res);

    res = lexi.deque();
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

    res = lexi.cluster_keys("fam");
    print(res);

    res = lexi.cluster_values("fam");
    print(res);

    res = lexi.cluster_entries("fam");
    print(res);

    res = lexi.cluster_del("fam", "vince");
    print(res);

    res = lexi.cluster_push("fam", "is cool");
    print(res);

    res = lexi.cluster_push("fam", "vince");
    print(res);

    res = lexi.cluster_push("fam", 42069);
    print(res);

    res = lexi.cluster_pop("fam");
    print(res);

    res = lexi.cluster_pop("fam");
    print(res);

    res = lexi.cluster_pop("fam");
    print(res);

    res = lexi.cluster_drop("fam");
    print(res);

    res = lexi.multi()
        .add_set("vince", "is the coolest")
        .add_get("vince")
        .run();
    print(res);
    return 0;
}
