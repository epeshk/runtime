// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include "pal.h"
#include "error_codes.h"
#include "trace.h"
#include "utils.h"
#include "hostfxr_resolver.h"
#include "jethost_properties.h"

#if !defined(FEATURE_LIBHOST)
#define CURHOST_TYPE    _X("dotnet")
#define CUREXE_PKG_VER  HOST_PKG_VER
#define CURHOST_EXE
#endif

#if defined(CURHOST_EXE)

int exe_start(const int argc, const pal::char_t* argv[])
{
    pal::string_t host_path;
    if (!pal::get_own_executable_path(&host_path) || !pal::realpath(&host_path))
    {
        trace::error(_X("Failed to resolve full path of the current executable [%s]"), host_path.c_str());
        return StatusCode::CoreHostCurHostFindFailure;
    }

    pal::string_t app_path;
    pal::string_t app_root;
    bool requires_hostfxr_startupinfo_interface = false;

    pal::string_t own_name = strip_executable_ext(get_filename(host_path));

    app_root.assign(host_path);
    app_path.assign(get_directory(app_root));
    append_path(&app_path, own_name.c_str());
    app_path.append(_X(".dll"));

    if (!jethost_properties::initialize(argc, argv))
        return StatusCode::CoreHostIncompatibleConfig;

    hostfxr_resolver_t fxr{app_root};

    // Obtain the entrypoints.
    int rc = fxr.status_code();
    if (rc != StatusCode::Success)
    {
        return rc;
    }

    {
        auto hostfxr_main_startupinfo = fxr.resolve_main_startupinfo();
        if (hostfxr_main_startupinfo != nullptr)
        {
            const pal::char_t* host_path_cstr = host_path.c_str();
            const pal::char_t* dotnet_root_cstr = fxr.dotnet_root().empty() ? nullptr : fxr.dotnet_root().c_str();
            const pal::char_t* app_path_cstr = app_path.empty() ? nullptr : app_path.c_str();

            trace::info(_X("Invoking fx resolver [%s] hostfxr_main_startupinfo"), fxr.fxr_path().c_str());
            trace::info(_X("Host path: [%s]"), host_path.c_str());
            trace::info(_X("Dotnet path: [%s]"), fxr.dotnet_root().c_str());
            trace::info(_X("App path: [%s]"), app_path.c_str());

            auto set_error_writer = fxr.resolve_set_error_writer();
            propagate_error_writer_t propagate_error_writer_to_hostfxr(set_error_writer);

            rc = hostfxr_main_startupinfo(argc, argv, host_path_cstr, dotnet_root_cstr, app_path_cstr);
        }
    }

    return rc;
}

#if defined(_WIN32)
int __cdecl wmain(const int argc, const pal::char_t* argv[])
#else
int main(const int argc, const pal::char_t* argv[])
#endif
{
    trace::setup();

    if (trace::is_enabled())
    {
        trace::info(_X("--- Invoked %s [version: %s, commit hash: %s] main = {"), CURHOST_TYPE, _STRINGIFY(CUREXE_PKG_VER), _STRINGIFY(REPO_COMMIT_HASH));
        for (int i = 0; i < argc; ++i)
        {
            trace::info(_X("%s"), argv[i]);
        }
        trace::info(_X("}"));
    }

    int exit_code = exe_start(argc, argv);

    // Flush traces before exit - just to be sure
    trace::flush();

    return exit_code;
}

#endif

#if defined(FEATURE_JETHOST)
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _WIN32
__attribute__((__visibility__("default")))
#endif
int libmain(const int argc, const pal::char_t *argv[], const char config[]) {
    jethost_properties::set_config(config);
#if defined(_WIN32)
    return wmain(argc, argv);
#else
    return main(argc, argv);
#endif
}

#ifdef __cplusplus
} // extern "C"
#endif
#endif