#pragma once

#include "builder.hh"
#include "lexitype.hh"
#include "socket.hh"
#include <cstdint>
#include <string>

class Lexi {
  public:
    Lexi(const std::string& addr_str, std::uint16_t port);
    Lexi(const char* addr_str, std::uint16_t port);
    int connect(void);
    LexiType ping(void);
    LexiType set(const std::string& key, const std::string& data);
    LexiType set(const std::string& key, std::int64_t data);
    LexiType get(const std::string& key);
    LexiType del(const std::string& key);
    LexiType push(const std::string& data);
    LexiType push(std::int64_t data);
    LexiType pop(void);
    LexiType cluster_new(const std::string& cluster_name);
    LexiType cluster_set(const std::string& cluster_name,
                         const std::string& key, const std::string& data);
    LexiType cluster_set(const std::string& cluster_name,
                         const std::string& key, std::int64_t value);
    LexiType cluster_get(const std::string& cluster_name,
                         const std::string& key);
    LexiType cluster_del(const std::string& cluster_name,
                         const std::string& key);
    LexiType cluster_push(const std::string& cluster_name,
                          const std::string& data);
    LexiType cluster_push(const std::string& cluster_name, std::int64_t data);
    LexiType cluster_pop(const std::string& cluster_name);
    LexiType cluster_drop(const std::string& cluster_name);
    ~Lexi(void);

  private:
    Builder builder;
    Socket sock;
    LexiType parse(std::uint8_t* read_buf, std::size_t read_buf_len);
};
