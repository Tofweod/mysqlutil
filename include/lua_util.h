#ifndef TOF_LUA_UTIL_H
#define TOF_LUA_UTIL_H


typedef struct Connection_Info Connection_Info;

/**
 * load config file into Connection_Info
 * example config file is in test folder
 */
extern int load_config(Connection_Info *c, const char *cfg_path);

#endif
