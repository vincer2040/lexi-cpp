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
    LexiType del(std::string key);
    LexiType cluster_new(std::string cluster_name);
    LexiType cluster_set(std::string cluster_name, std::string key,
                         std::string data);
    LexiType cluster_set(std::string cluster_name, std::string key,
                         std::int64_t value);
    LexiType cluster_get(std::string cluster_name, std::string key);
    LexiType cluster_del(std::string cluster_name, std::string key);
    LexiType cluster_drop(std::string cluster_name);
    ~Lexi(void);

  private:
    Builder builder;
    Socket sock;
};
