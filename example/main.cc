#include "../src/lexi.hh"
#include <iostream>

int main() {
    Lexi lexi = Lexi("127.0.0.1", 6969);
    lexi.connect();
    LexiType ping_res = lexi.ping();
    if (ping_res.type == LexiTypeT::Simple) {
        std::string simple = std::get<std::string>(ping_res.data);
        std::cout << simple << "\n";
    } else {
        std::cout << "wtf\n";
    }
    LexiType set_res = lexi.set("vince", "is cool");
    if (set_res.type == LexiTypeT::Simple) {
        std::string simple = std::get<std::string>(set_res.data);
        std::cout << simple << "\n";
    } else {
        std::cout << "wtf\n";
    }
    LexiType get_res = lexi.get("vince");
    if (get_res.type == LexiTypeT::Bulk) {
        std::string bulk = std::get<std::string>(get_res.data);
        std::cout << bulk << "\n";
    } else {
        std::cout << "wtf\n";
    }
    return 0;
}
