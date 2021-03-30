#include "jethost_runtime_config.h"
#include "jethost_properties.h"
#include "pal.h"
#include "framework_info.h"
#include <algorithm>
#include <iterator>

namespace {
    pal::string_t find_highest_version(const pal::string_t& dotnet_root, const pal::string_t& fx_name) {
        std::vector<framework_info> infos;
        framework_info::get_all_framework_infos(dotnet_root, fx_name, &infos);
        if (infos.empty()) {
            trace::error(_X("Not found any %s shared framework version"), &fx_name);
            return nullptr;
        }
        // frameworks already sorted by name and version in get_all_framework_infos implementation
        return infos[infos.size() - 1].version.as_str();
    }

    bool is_version_exists(const pal::string_t& dotnet_root, const pal::string_t& fx_name, const pal::string_t& fx_version) {
        std::vector<framework_info> infos;
        framework_info::get_all_framework_infos(dotnet_root, fx_name, &infos);
        return std::find_if(infos.begin(), infos.end(), [&](const framework_info& info) {return info.version.as_str() == fx_version;}) != std::end(infos);
    }

    std::vector<const pal::char_t*> get_fx_names_on_current_platform() {
#if defined(_WIN32)
        const pal::char_t *wpf = _X("Microsoft.WindowsDesktop.App");
#endif // _WIN32
        const pal::char_t *core = _X("Microsoft.NETCore.App");

        const std::vector<const pal::char_t*> fx_names {
#if defined(_WIN32)
                wpf,
#endif // _WIN32
                core
        };

        return fx_names;
    }
}

pal::char_t* jethost_runtime_config::get_runtime_config() {
    const pal::char_t runtime_opts_template[] = R"({
  "framework": {
    "name": "%s",
    "version": "%s"
  },
  "rollForward": "Disable",
  "configProperties": {
    "DEFAULT_STACK_SIZE": "200000"
  }
})";

    pal::string_t dotnet_root;
    jethost_properties::try_get(JETHOST_DOTNET_ROOT, &dotnet_root);

    for (const pal::char_t *fx_name : get_fx_names_on_current_platform()) {
        auto out = new pal::char_t [1024];
        pal::string_t max_version = find_highest_version(dotnet_root, fx_name);
        const pal::char_t *fx_version = max_version.c_str();
        jethost_properties::try_get(JETHOST_FX_VERSION, &fx_version);
        pal::snwprintf(out, 1024, runtime_opts_template, fx_name, fx_version);
        return out;
    }

    return nullptr;
}