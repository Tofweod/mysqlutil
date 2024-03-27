#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include "log.h"
#include "util.h"

#define LUA_WORKSPACE

#ifdef LUA_PATH
#define PATH_OF_LUA(file) (LUA_PATH  file)
#else
#define PATH_OF_LUA(file) file
#endif
/**
 * load config file into Connection_Info
 * example config file is in test folder
 */
int load_config(Connection_Info *c, const char *cfg_path);

#endif // ! LUA_UTIL_H
