#ifndef TOF_UTIL_SCHEMA_H
#define TOF_UTIL_SCHEMA_H

#include "json_util.h"

typedef struct t_v_pair t_v_pair;
typedef struct Schema *Schema;

extern Schema schema_new(yyjson_val *json_root);

#endif
