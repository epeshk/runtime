//
// Created by Evgeny Peshkov on 4/1/21.
//

#ifndef COREHOST_JETHOST_APP_CONFIG_H
#define COREHOST_JETHOST_APP_CONFIG_H

#include "pal.h"

class jethost_runtime_config {
public:
    static pal::char_t * get_runtime_config();
};

#endif //COREHOST_JETHOST_APP_CONFIG_H
