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

    lexi::lexi_data set = c.set(std::string("foo"), std::string("bar"));
    std::cout << set << '\n';

    lexi::lexi_data set_int = c.set(std::string("baz"), (int64_t)1337);
    std::cout << set_int << '\n';

    lexi::lexi_data get = c.get(std::string("foo"));
    std::cout << get << '\n';

    lexi::lexi_data get_int = c.get(std::string("baz"));
    std::cout << get_int << '\n';

    lexi::lexi_data del = c.del(std::string("foo"));
    std::cout << del << '\n';

    lexi::lexi_data push = c.push(std::string("foo"));
    std::cout << push << '\n';

    lexi::lexi_data pop = c.pop();
    std::cout << pop << '\n';

    lexi::lexi_data enque = c.enque(std::string("foo"));
    std::cout << enque << '\n';

    lexi::lexi_data deque = c.deque();
    std::cout << deque << '\n';

    lexi::lexi_data zset = c.zset(std::string("foo"));
    std::cout << zset << '\n';

    lexi::lexi_data zhas = c.zhas(std::string("foo"));
    std::cout << zhas << '\n';

    lexi::lexi_data zdel = c.zdel(std::string("foo"));
    std::cout << zdel << '\n';

    return 0;
}
