#ifndef LUA_UTIL_H
#define LUA_UTIL_H

#include "log.h"
#include "util.h"

#define LUA_WORKSPACE

/**
 * load config file into Connection_Info
 * example config file below:
  return { -- return is unignorable
        host = "0.0.0.0",
        user = "root",
        passwd = "xxxx",
        db = "xxxx",
        port = 80,
  }
*/
int load_config(Connection_Info *c, const char *cfg_path);

#endif // ! LUA_UTIL_H
