#include "../src/lexi.hh"
#include <iostream>
#include <stdexcept>

int main() {
    lexi::client c("127.0.0.1", 6969);
    try {
        c.connect();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << '\n';
        return 1;
    }
    lexi::lexi_data auth_res = c.authenticate("root", "root");
    std::cout << auth_res << '\n';

    lexi::lexi_data set = c.set("foo", "bar");
    std::cout << set << '\n';

    lexi::lexi_data set_int = c.set("baz", 1337);
    std::cout << set_int << '\n';

    lexi::lexi_data get = c.get("foo");
    std::cout << get << '\n';

    lexi::lexi_data get_int = c.get("baz");
    std::cout << get_int << '\n';

    lexi::lexi_data del = c.del("foo");
    std::cout << del << '\n';
    return 0;
}
