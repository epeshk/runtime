#include "pal.h"

namespace {
    bool read_assembly_version(const pal::string_t& path, pal::string_t* version) {
        FILE* file = pal::file_open(path, _X("r"));
        if (file == nullptr)
            return false;
return true;
    }
}