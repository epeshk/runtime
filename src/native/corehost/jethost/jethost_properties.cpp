#include "jethost_properties.h"
#include "utils.h"

namespace {
    bool validate() {
        pal::string_t out;
        if (!jethost_properties::try_get(JETHOST_DOTNET_ROOT, &out)) {
            trace::error("JETHOST_DOTNET_ROOT wasn't set");
            return false;
        }

        return true;
    }
}

coreclr_property_bag_t jethost_properties::_storage = coreclr_property_bag_t();
const char* jethost_properties::_blob;

bool jethost_properties::try_get(jethost_property key, const pal::char_t **value) {
    return _storage.try_get(property_to_string(key), value);
}

bool jethost_properties::try_get(jethost_property key, pal::string_t *value) {
    const pal::char_t* val;

    if (_storage.try_get(property_to_string(key), &val)) {
        value->assign(val);
        return true;
    }

    return false;
}

void jethost_properties::log_properties() {
    trace::verbose(_X("JetHost properties:"));
    _storage.log_properties();
    trace::verbose(_X("===="));
}

const pal::char_t *jethost_properties::property_to_string(jethost_property key) {
    int idx = static_cast<int>(key);
    assert(0 <= idx && idx < static_cast<int>(jethost_property::Last));
    return JetHostPropertyNameMapping[idx];
}

bool jethost_properties::initialize(const int argc, const pal::char_t **argv) {
    for (int i = 0; i < static_cast<int>(jethost_property::Last); ++i) {
        const pal::char_t* name = property_to_string(static_cast<jethost_property>(i));
        pal::string_t value;
        if (pal::getenv(name, &value))
            _storage.add(name, value.c_str());
    }

    for (int i = 0; i < argc; ++i) {
        const pal::string_t arg = argv[i];

        if (starts_with(arg, _X("-JETHOST_"), true) || starts_with(arg, _X("/JETHOST_"), true)) {
            size_t pos = arg.find(_X('='));
            pal::string_t key = arg.substr(1, pos-1);
            pal::string_t value = pos == std::string::npos ? _X("1") : arg.substr(pos+1);
            if (value.empty()) {
                trace::warning("JetHost property %s provided as valued but without value and will be ignored", key.c_str());
                continue;
            }
            trace::println(_X("CMD JetHost property %s=%s"), key.c_str(), value.c_str());
            _storage.add(key.c_str(), value.c_str());
        }
    }

    return validate();
}

bool jethost_properties::try_get_path(jethost_property key, pal::string_t *value){
    return try_get(key, value) && pal::realpath(value, false);
}

bool jethost_properties::get_flag(jethost_property key) {
    const pal::char_t *value;
    return try_get(key, &value)
      && pal::strcmp(value, _X("0")) != 0
      && pal::strcasecmp(value, _X("false")) != 0;
}

void jethost_properties::set_config(const char *blob) {
    jethost_properties::_blob = blob;
}
