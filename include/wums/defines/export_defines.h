#pragma once

#include <stdint.h>

#define EXPORT_MAXIMUM_NAME_LENGTH     50
typedef struct export_data_t {
    uint32_t type;
    char name[EXPORT_MAXIMUM_NAME_LENGTH];
    uint32_t address;
} export_data_t;