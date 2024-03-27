#include "lua_util.h"
#include "log.h"
#include "util.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <stdlib.h>

int load_config(Connection_Info *c, const char *cfg_path) {
    lua_State *l = luaL_newstate();
    luaL_openlibs(l);
    lua_pushstring(l, cfg_path);
    lua_setglobal(l, "config-path");

    if (luaL_dofile(l, PATH_OF_LUA("load.lua")) !=
        LUA_OK) {
        ERROR_PRINT("Error: %s\n", lua_tostring(l, -1));
        return 1;
    }

    if (!lua_istable(l, -1)) {
        ERROR_PRINT("Error:Return value is not a table in config file\n");
        return 1;
    }

    lua_getfield(l, -1, "host");
    const char *host = lua_tostring(l, -1);
    set_host(c, host);
    lua_pop(l, 1);

    lua_getfield(l, -1, "user");
    const char *user = lua_tostring(l, -1);
    set_user(c, user);
    lua_pop(l, 1);

    lua_getfield(l, -1, "passwd");
    const char *passwd = lua_tostring(l, -1);
    set_passwd(c, passwd);
    lua_pop(l, 1);

    lua_getfield(l, -1, "db");
    const char *db = lua_tostring(l, -1);
    set_db(c, db);
    lua_pop(l, 1);

    lua_getfield(l, -1, "port");
    int port = lua_tointeger(l, -1);
    set_port(c, port);
    lua_pop(l, 1);

    lua_getfield(l, -1, "unix_socket");
    const char *unix_socket = lua_isnil(l, -1) ? NULL : lua_tostring(l, -1);
    set_unix_socket(c, unix_socket);
    lua_pop(l, 1);

    lua_getfield(l, -1, "clientflag");
    unsigned long clientflag = lua_isnil(l, -1) ? 0 : lua_tointeger(l, -1);
    set_clientflag(c, clientflag);
    lua_pop(l, 1);

    lua_close(l);

#ifdef DEBUG
    INFO_PRINT("host is %s\n", host);
    INFO_PRINT("user is %s\n", user);
    INFO_PRINT("passwd is %s\n", passwd);
    INFO_PRINT("db is %s\n", db);
    INFO_PRINT("port is %d\n", port);
    INFO_PRINT("unix_socket is %s\n", unix_socket);
    INFO_PRINT("clientflag is %lu\n", clientflag);

#endif /* ifdef DEBUG */

    return 0;
}
