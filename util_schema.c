#include "include/util_schema.h"
#define STB_DS_IMPLEMENTATION
#define STBDS_NO_SHORT_NAMES
#include "include/stb/stb_ds.h"

typedef struct Schema {
    t_v_pair *fileds;
} *Schema;


Schema schema_new(yyjson_val *json_root) {
    return NULL;
}

