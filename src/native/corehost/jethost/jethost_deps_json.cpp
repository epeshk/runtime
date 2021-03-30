//#include "jethost_deps_json.h"
//#include <vector>
//
//namespace {
//    std::vector<deps_json_package> get_packages_from_directory(const pal::string_t& path) {
//        std::vector<deps_json_package> result;
//        if (!pal::directory_exists(path)) {
//            return result;
//        }
//        std::vector<pal::string_t> libs;
//        pal::readdir(path, ".dll", &libs);
//        pal::readdir(path, ".exe", &libs);
//        return result;
//    }
//}
//
//void jethost_deps_json::emulate(const deps_json_t& deps_json, const pal::string_t& path) {
//
//}