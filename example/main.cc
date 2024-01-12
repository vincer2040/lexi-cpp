#include "../src/lexi.hh"
#include <iostream>
#include <stdexcept>

int main() {
    lexi::client c("127.0.0.1", 6969);
    try {
        c.connect();
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << '\n';
        exit(1);
    }
    lexi::lexi_data auth_res = c.authenticate("root", "root");
    std::cout << auth_res << '\n';
    return 0;
}
