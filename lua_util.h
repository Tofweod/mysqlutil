#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include "log.h"
#include "util.h"

#define LUA_WORKSPACE

int load_config(Connection_Info *c, const char *cfg_path);

#endif // ! LUA_UTIL_H
