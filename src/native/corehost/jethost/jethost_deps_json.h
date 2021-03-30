//
// Created by Evgeny Peshkov on 4/5/21.
//

#ifndef COREHOST_JETHOST_DEPS_JSON_H
#define COREHOST_JETHOST_DEPS_JSON_H

#include "deps_format.h"
#include <unordered_map>

class RID {
    const pal::string_t m_rid;
    explicit RID(const pal::string_t rid): m_rid(rid) {}

public:
    static const RID Any;
    static const RID Windows;
    static const RID Linux;
    static const RID OSX;
    static const RID Unix;

    pal::string_t to_string() {return m_rid;}

    bool operator==(const RID& rid) {
        return m_rid == rid.m_rid;
    }
};

const RID RID::Any = RID(_X("any"));
const RID RID::Windows = RID(_X("windows"));
const RID RID::Linux = RID(_X("linux"));
const RID RID::OSX = RID(_X("osx"));
const RID RID::Unix = RID(_X("unix"));

struct assembly_file {
    pal::string_t path;
    pal::string_t version;
};

/// - package_name
///   - library_name.dll
///     - any: NetCore/library_name.dll, 4.0.0.0
///     - unix: NetCore/runtimes/unix/netcoreapp3.0/library_name.dll, 4.0.0.0
///
struct deps_json_package {
    pal::string_t name;
    std::unordered_map<pal::string_t, std::unordered_map<RID, assembly_file>()> libraries;
};

struct jethost_deps_json {
    static void emulate(const deps_json_t& deps_json, const pal::string_t& path);
};

#endif //COREHOST_JETHOST_DEPS_JSON_H
