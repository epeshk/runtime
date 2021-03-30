//
// Created by evgeny.peshkov on 19.03.2021.
//

#ifndef COREHOST_JETHOST_PROPERTIES_H
#define COREHOST_JETHOST_PROPERTIES_H

#include <pal.h>
#include "hostpolicy/coreclr.h"

enum jethost_property {
    JETHOST_DOTNET_ROOT,
    JETHOST_APP_PATHS,
    JETHOST_APP_NI_PATHS,
    JETHOST_FX_VERSION,
    JETHOST_EMULATE_RUNTIMECONFIG,
    JETHOST_DEPSFILE,
    JETHOST_ENTRYPOINT_ASSEMBLY_NAME,
    JETHOST_ENTRYPOINT_TYPE_NAME,
    JETHOST_ENTRYPOINT_METHOD_NAME,
    JETHOST_ENTRYPOINT_METHOD_SIGNATURE,
    Last
};

namespace {
    const pal::char_t *JetHostPropertyNameMapping[] =
    {
        _X("JETHOST_DOTNET_ROOT"),
        _X("JETHOST_APP_PATHS"),
        _X("JETHOST_APP_NI_PATHS"),
        _X("JETHOST_FX_VERSION"),
        _X("JETHOST_EMULATE_RUNTIMECONFIG"),
        _X("JETHOST_DEPSFILE"),
        _X("JETHOST_ENTRYPOINT_ASSEMBLY_NAME"),
        _X("JETHOST_ENTRYPOINT_TYPE_NAME"),
        _X("JETHOST_ENTRYPOINT_METHOD_NAME"),
        _X("JETHOST_ENTRYPOINT_METHOD_SIGNATURE")
    };

    static_assert((sizeof(JetHostPropertyNameMapping) / sizeof(*JetHostPropertyNameMapping)) == static_cast<size_t>(jethost_property::Last), "Invalid property count");

}

class jethost_properties {
public: // static
    static const pal::char_t* property_to_string(jethost_property key);
    static bool initialize(const int argc, const pal::char_t **argv);
    static void set_config(const char *blob);
    static void log_properties();
    static bool try_get(jethost_property key, const pal::char_t **value);
    static bool try_get(jethost_property key, pal::string_t *value);

    static bool try_get_path(jethost_property key, pal::string_t *value);
    static bool get_flag(jethost_property key);

private:
    static coreclr_property_bag_t _storage;
    static const char* _blob;
};

#endif //COREHOST_JETHOST_PROPERTIES_H
