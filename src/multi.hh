#pragma once

#include "builder.hh"
#include "cmd.hh"
#include "lexitype.hh"
#include "socket.hh"
#include <cstdint>
#include <string>
#include <vector>

class Multi {
  public:
    Multi(Builder& builder, Socket& sock);
    Multi add_set(const std::string& key, const std::string& value);
    Multi add_set(const std::string& key, std::int64_t value);
    Multi add_get(const std::string& key);
    Multi add_del(const std::string& key);
    Multi add_push(const std::string& value);
    Multi add_push(std::int64_t value);
    Multi add_pop(void);
    Multi add_enque(const std::string& value);
    Multi add_enque(std::int64_t value);
    Multi add_deque(void);
    Multi add_keys(void);
    Multi add_values(void);
    Multi add_entries(void);
    Multi add_cluster_new(const std::string& name);
    Multi add_cluster_set(const std::string& name, const std::string& key,
                          const std::string& value);
    Multi add_cluster_set(const std::string& name, const std::string& key,
                          std::int64_t value);
    Multi add_cluster_get(const std::string& name, const std::string& key);
    Multi add_cluster_del(const std::string& name, const std::string& key);
    Multi add_cluster_push(const std::string& name, const std::string& value);
    Multi add_cluster_push(const std::string& name, std::int64_t value);
    Multi add_cluster_pop(const std::string& name);
    Multi add_cluster_keys(const std::string& name);
    Multi add_cluster_values(const std::string& name);
    Multi add_cluster_entries(const std::string& name);
    Multi add_cluster_drop(const std::string& name);
    LexiType run(void);

  private:
    Builder& builder;
    Socket& sock;
    std::vector<Cmd> cmds;
    int build_cmd(void);
    int add_value(ValueType value_type, std::variant<std::int64_t, const std::string> value);
    LexiType parse(std::uint8_t* read_buf, std::size_t read_buf_len);
};
