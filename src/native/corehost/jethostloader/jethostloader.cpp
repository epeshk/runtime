#include "pal.h"
#include "trace.h"
#include "utils.h"

#if defined(_WIN32)
typedef int __cdecl (*entry_point_fn) (const int, const pal::char_t**, const char*);
#else
typedef int (*entry_point_fn) (const int, const pal::char_t**, const char*);
#endif

const char config[4096] = "JETHOST_CONFIG_HERE";

int exe_start(const int argc, const pal::char_t* argv[]) {
    pal::string_t path;
    if (!pal::get_own_executable_path(&path))
        return 1;
    path = get_directory(path);
    append_path(&path, _X("jethost"));
    pal::string_t libjethost_path = path + pal::exe_suffix();
    pal::dll_t dll;
    if (!pal::load_library(&libjethost_path, &dll))
    {
        trace::println(_X("Couldn't load library from %s"), libjethost_path.c_str());
        return 1;
    }
    auto entry_point = reinterpret_cast<entry_point_fn>(pal::get_symbol(dll, "libmain"));
    trace::println(_X("%ll"), entry_point);
    return entry_point(argc, argv, config);
}

#if defined(_WIN32)
int __cdecl wmain(const int argc, const pal::char_t* argv[])
#else
int main(const int argc, const pal::char_t* argv[])
#endif
{
    trace::setup();
    int exit_code = exe_start(argc, argv);
    trace::println(_X("Exit code: %d"), exit_code);
    trace::flush();
    return exit_code;
}

