#include "multi.hh"
#include "cmd.hh"
#include "lexer.hh"
#include "parser.hh"
#include <variant>

Multi::Multi(Builder& builder, Socket& sock) : builder(builder), sock(sock) {
    this->builder.reset();
}

Multi Multi::add_set(const std::string& key, const std::string& value) {
    Cmd cmd = {CmdType::Set, SetCmd{ValueType::String, value, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_set(const std::string& key, std::int64_t value) {
    Cmd cmd = {CmdType::Set, SetCmd{ValueType::Int, value, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_get(const std::string& key) {
    Cmd cmd = {CmdType::Get, GetCmd{key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_del(const std::string& key) {
    Cmd cmd = {CmdType::Del, DelCmd{key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_push(const std::string& value) {
    Cmd cmd = {CmdType::Push, PushCmd{ValueType::String, value}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_push(std::int64_t value) {
    Cmd cmd = {CmdType::Push, PushCmd{ValueType::Int, value}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_pop(void) {
    Cmd cmd = {CmdType::Pop, std::monostate()};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_enque(const std::string& value) {
    Cmd cmd = {CmdType::Enque, EnqueCmd{ValueType::String, value}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_enque(std::int64_t value) {
    Cmd cmd = {CmdType::Enque, EnqueCmd{ValueType::Int, value}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_deque(void) {
    Cmd cmd = {CmdType::Deque, std::monostate()};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_keys(void) {
    Cmd cmd = {CmdType::Keys, std::monostate()};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_values(void) {
    Cmd cmd = {CmdType::Values, std::monostate()};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_entries(void) {
    Cmd cmd = {CmdType::Entries, std::monostate()};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_new(const std::string& name) {
    Cmd cmd = {CmdType::ClusterNew, ClusterNewCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_set(const std::string& name, const std::string& key,
                             const std::string& value) {
    Cmd cmd = {CmdType::ClusterSet,
               ClusterSetCmd{name, Value{ValueType::String, value}, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_set(const std::string& name, const std::string& key,
                             std::int64_t value) {
    Cmd cmd = {CmdType::ClusterSet,
               ClusterSetCmd{name, Value{ValueType::Int, value}, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_get(const std::string& name, const std::string& key) {
    Cmd cmd = {CmdType::ClusterGet, ClusterGetCmd{name, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_del(const std::string& name, const std::string& key) {
    Cmd cmd = {CmdType::ClusterDel, ClusterDelCmd{name, key}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_push(const std::string& name,
                              const std::string& value) {
    Cmd cmd = {CmdType::ClusterPush,
               ClusterPushCmd{name, Value{ValueType::String, value}}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_push(const std::string& name, std::int64_t value) {
    Cmd cmd = {CmdType::ClusterPush,
               ClusterPushCmd{name, Value{ValueType::Int, value}}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_pop(const std::string& name) {
    Cmd cmd = {CmdType::ClusterPop, ClusterPopCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_keys(const std::string& name) {
    Cmd cmd = {CmdType::ClusterKeys, ClusterKeysCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_values(const std::string& name) {
    Cmd cmd = {CmdType::ClusterValues, ClusterValuesCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_entries(const std::string& name) {
    Cmd cmd = {CmdType::ClusterEntries, ClusterEntriesCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

Multi Multi::add_cluster_drop(const std::string& name) {
    Cmd cmd = {CmdType::ClusterDrop, ClusterDropCmd{name}};
    this->cmds.push_back(cmd);
    return *this;
}

int Multi::build_cmd(void) {
    std::size_t len = this->cmds.size();
    this->builder.add_arr(len);
    for (auto cmd : this->cmds) {
        switch (cmd.type) {
        case CmdType::Ping:
            this->builder.add_ping();
            break;
        case CmdType::Set: {
            SetCmd set_cmd = std::get<SetCmd>(cmd.data);
            this->builder.add_arr(3);
            this->builder.add_string("SET");
            this->builder.add_string(set_cmd.key);
            this->add_value(set_cmd.val_type, set_cmd.val);
        } break;
        case CmdType::Get: {
            GetCmd get_cmd = std::get<GetCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("GET");
            this->builder.add_string(get_cmd.key);
        } break;
        case CmdType::Del: {
            DelCmd del_cmd = std::get<DelCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("DEL");
            this->builder.add_string(del_cmd.key);
        } break;
        case CmdType::Push: {
            PushCmd push_cmd = std::get<PushCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("PUSH");
            this->add_value(push_cmd.val_type, push_cmd.val);
        } break;
        case CmdType::Pop:
            this->builder.add_string("POP");
            break;
        case CmdType::Enque: {
            EnqueCmd enque_cmd = std::get<EnqueCmd>(cmd.data);
            this->builder.add_string("ENQUE");
            this->add_value(enque_cmd.val_type, enque_cmd.val);
        } break;
        case CmdType::Deque:
            this->builder.add_string("DEQUE");
            break;
        case CmdType::Keys:
            this->builder.add_string("KEYS");
            break;
        case CmdType::Values:
            this->builder.add_string("VALUES");
            break;
        case CmdType::Entries:
            this->builder.add_string("ENTRIES");
            break;
        case CmdType::ClusterNew: {
            ClusterNewCmd cluster_new_cmd = std::get<ClusterNewCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.NEW");
            this->builder.add_string(cluster_new_cmd.name);
        } break;
        case CmdType::ClusterSet: {
            ClusterSetCmd cluster_set_cmd = std::get<ClusterSetCmd>(cmd.data);
            this->builder.add_arr(4);
            this->builder.add_string("CLUSTER.SET");
            this->builder.add_string(cluster_set_cmd.name);
            this->builder.add_string(cluster_set_cmd.key);
            this->add_value(cluster_set_cmd.val_type, cluster_set_cmd.val);
        } break;
        case CmdType::ClusterGet: {
            ClusterGetCmd cluster_get_cmd = std::get<ClusterGetCmd>(cmd.data);
            this->builder.add_arr(3);
            this->builder.add_string("CLUSTER.GET");
            this->builder.add_string(cluster_get_cmd.name);
            this->builder.add_string(cluster_get_cmd.key);
        } break;
        case CmdType::ClusterDel: {
            ClusterDelCmd cluster_del_cmd = std::get<ClusterDelCmd>(cmd.data);
            this->builder.add_arr(3);
            this->builder.add_string("CLUSTER.DEL");
            this->builder.add_string(cluster_del_cmd.name);
            this->builder.add_string(cluster_del_cmd.key);
        } break;
        case CmdType::ClusterPush: {
            ClusterPushCmd cluster_push_cmd =
                std::get<ClusterPushCmd>(cmd.data);
            this->builder.add_arr(3);
            this->builder.add_string("CLUSTER.PUSH");
            this->builder.add_string(cluster_push_cmd.name);
            this->add_value(cluster_push_cmd.val_type, cluster_push_cmd.val);
        } break;
        case CmdType::ClusterPop: {
            ClusterPopCmd cluster_pop_cmd = std::get<ClusterPopCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.POP");
            this->builder.add_string(cluster_pop_cmd.name);
        } break;
        case CmdType::ClusterKeys: {
            ClusterKeysCmd cluster_keys_cmd =
                std::get<ClusterKeysCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.KEYS");
            this->builder.add_string(cluster_keys_cmd.name);
        } break;
        case CmdType::ClusterValues: {
            ClusterValuesCmd cluster_values_cmd =
                std::get<ClusterValuesCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.VALUES");
            this->builder.add_string(cluster_values_cmd.name);
        } break;
        case CmdType::ClusterEntries: {
            ClusterEntriesCmd cluster_entries_cmd =
                std::get<ClusterEntriesCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.ENTRIES");
            this->builder.add_string(cluster_entries_cmd.name);
        } break;
        case CmdType::ClusterDrop: {
            ClusterDropCmd cluster_drop_cmd =
                std::get<ClusterDropCmd>(cmd.data);
            this->builder.add_arr(2);
            this->builder.add_string("CLUSTER.DROP");
            this->builder.add_string(cluster_drop_cmd.name);
        } break;
        }
    }
    return 0;
}

int Multi::add_value(ValueType value_type,
                     std::variant<std::int64_t, const std::string> value) {
    switch (value_type) {
    case ValueType::Int: {
        std::int64_t val = std::get<std::int64_t>(value);
        return this->builder.add_int(val);
    };
    case ValueType::String: {
        const std::string val = std::get<const std::string>(value);
        return this->builder.add_string(val);
    };
    }
    return -1;
}

LexiType Multi::run(void) {
    std::uint8_t* buf;
    std::size_t buf_len;
    ssize_t send_res, read_len;
    std::tuple<std::uint8_t*, ssize_t> read_res;
    std::uint8_t* read_buf;
    this->build_cmd();

    buf = this->builder.out();
    buf_len = this->builder.length();

    send_res = this->sock.send(buf, buf_len);
    if (send_res == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    read_res = this->sock.receive();
    read_buf = std::get<0>(read_res);
    read_len = std::get<1>(read_res);
    if (read_buf == nullptr) {
        return {LexiTypeT::Err, std::monostate()};
    }
    if (read_len == -1) {
        return {LexiTypeT::Err, std::monostate()};
    }
    return this->parse(read_buf, read_len);
}

LexiType Multi::parse(std::uint8_t* read_buf, std::size_t read_buf_len) {
    Lexer l(read_buf, read_buf_len);
    Parser p(l);
    return p.parse();
}
