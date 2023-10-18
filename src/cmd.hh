#pragma once

#include <cstdint>
#include <string>
#include <variant>

enum class CmdType {
    Ping,
    Set,
    Get,
    Del,
    Push,
    Pop,
    Enque,
    Deque,
    Keys,
    Values,
    Entries,
    ClusterNew,
    ClusterSet,
    ClusterGet,
    ClusterDel,
    ClusterPush,
    ClusterPop,
    ClusterKeys,
    ClusterValues,
    ClusterEntries,
    ClusterDrop,
};

enum class ValueType {
    String,
    Int,
};

struct Value {
    ValueType val_type;
    std::variant<std::int64_t, const std::string> val;
};

struct SetCmd : public Value {
    const std::string& key;
};

struct GetCmd {
    const std::string& key;
};

struct DelCmd : public GetCmd {};

struct PushCmd : public Value {};

struct EnqueCmd : public PushCmd {};

struct ClusterNewCmd {
    const std::string& name;
};

struct ClusterSetCmd : public ClusterNewCmd, public SetCmd {};

struct ClusterGetCmd : public ClusterNewCmd, public GetCmd {};

struct ClusterDelCmd : public ClusterGetCmd {};

struct ClusterPushCmd : public ClusterNewCmd, public PushCmd {};

struct ClusterPopCmd : public ClusterNewCmd {};

struct ClusterKeysCmd : public ClusterNewCmd {};

struct ClusterValuesCmd : public ClusterNewCmd {};

struct ClusterEntriesCmd : public ClusterNewCmd {};

struct ClusterDropCmd : public ClusterNewCmd {};

struct Cmd {
    CmdType type;
    std::variant<SetCmd, GetCmd, DelCmd, PushCmd, EnqueCmd, ClusterNewCmd,
                 ClusterSetCmd, ClusterGetCmd, ClusterDelCmd, ClusterPushCmd,
                 ClusterPopCmd, ClusterKeysCmd, ClusterValuesCmd,
                 ClusterEntriesCmd, ClusterDropCmd, std::monostate>
        data;
};
