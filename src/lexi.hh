#pragma once

#include "builder.hh"
#include "lexitype.hh"
#include "socket.hh"
#include <cstdint>
#include <string>

class Lexi {
  public:
    Lexi(std::string addr_str, std::uint16_t port);
    int connect(void);
    LexiType ping(void);
    LexiType set(std::string key, std::string data);
    LexiType set(std::string key, std::int64_t data);
    LexiType get(std::string key);
    ~Lexi(void);

  private:
    Builder builder;
    Socket sock;
};
