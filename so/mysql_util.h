#ifndef TOF_MYSQL_UTIL
#define TOF_MYSQL_UTIL

#include <mysql/field_types.h>
#include <mysql/mysql.h>

typedef struct Connection_Info {
    const char *host;
    const char *user;
    const char *passwd;
    const char *db;
    unsigned int port;
    const char *unix_socket;
    unsigned long clientflag;
} Connection_Info;

extern Connection_Info *set_host(Connection_Info *conn_info, const char *host);
extern Connection_Info *set_user(Connection_Info *conn_info, const char *user);
extern Connection_Info *set_passwd(Connection_Info *conn_info,
                                   const char *passwd);
extern Connection_Info *set_db(Connection_Info *conn_info, const char *db);
extern Connection_Info *set_port(Connection_Info *conn_info,
                                 const unsigned int port);
extern Connection_Info *set_unix_socket(Connection_Info *conn_info,
                                        const char *unix_socket);
extern Connection_Info *set_clientflag(Connection_Info *conn_info,
                                       const unsigned long clientflag);

extern MYSQL *connect_by_info(MYSQL *conn, const Connection_Info *conn_info);

typedef struct t_v_pair {
#define t_v_pair_magic_number 0x3547fb
    unsigned long magic;
    enum enum_field_types type;
    const void *value;
    unsigned long length;
} t_v_pair;

/**
 * use set_param to set inner param, need to ensure value dont change before
 * executing stmt
 * use t_v_pair for set_param,use MAKE_ARG to build t_v_pair
 */
extern MYSQL_STMT *set_stmt(MYSQL_STMT *stmt, size_t argc, ...);

/**
 * param->buffer just points to value,thus need to ensure value dont change
 * before executing stmt
 * parameter length is especially for MYSQL_TYPE_BLOB to get the blob data's
 * length,otherwise just set this to 0
 */
extern int set_param(enum enum_field_types type, const void *value,
                     unsigned long length, MYSQL_BIND *param);

/**
 * param->buffer just points to value,thus need to ensure value dont change
 * before getting the result
 */
extern int set_res_param(enum enum_field_types type, void *data,
                         unsigned long length, MYSQL_BIND *param);

#define MAKE_ARG(t, v, ...)                                                    \
    (t_v_pair *)&(t_v_pair) {                                                  \
        t_v_pair_magic_number, (t), (v), 0##__VA_ARGS__                           \
    } // ... for length field

// get 0 when is t_v_pair
#define CHECK_TV_PAIR(pair) (!!((pair)->magic ^ t_v_pair_magic_number))

/**
 * store struct of result stmt in data field
 */
typedef struct mysql_struct_frame {
    MYSQL_STMT *stmt;
    // item to iterator in mysql_stmt_fetch
    void *data;
    MYSQL_BIND *params;
} mysql_struct_frame;

typedef void (*struct_handler)(void *ptr,MYSQL_BIND* binds);

/**
 * use handler to set_param and register result into ptr
 * a example follows:
 * assume result fields are mid(varchar),aid(varchar),role(varchar)
 *
 *
 * struct to store info is:
 * struct Actor_role {
 *  char mid[20];
 *  char aid[20];
 *  char role[20];
 * };
 *
 *
 * it's handler should be such type:
 * void Actor_role_handler(void *ptr, MYSQL_BIND *res) {
 *  struct Actor_role *ar = ptr;
 *  set_res_param(MYSQL_TYPE_STRING, ar->mid, 20, &res[0]);
 *  set_res_param(MYSQL_TYPE_STRING, ar->aid, 20, &res[1]);
 *  set_res_param(MYSQL_TYPE_STRING, ar->role, 20, &res[2]);
 * }
 * in the handler the res parameter is offered by register function,all you need
 * to concern is to allocate field for element in res
 *
 *
 * after register the result of a row is store in ptr
 * NOTE: undefined behavior for multi register in a stmt
 */
extern mysql_struct_frame *
_stmt_struct_register(MYSQL_STMT *stmt, size_t ptr_size, size_t param_size,
                      struct_handler handler);

#define stmt_struct_register(stmt, type, p_argc, handler)                      \
    _stmt_struct_register((stmt), sizeof(type), (p_argc), (handler))
/**
 * close stmt_binded_result,release resources in frame
 */
extern void stmt_struct_unregister(mysql_struct_frame *frame);

#define CHECK_PTR_OF(type, p) (1 ? p : (type)0)

/**
* other modules
*/

#ifdef TOF_UTIL_USE_LUA
#include "lua_util.h"
#endif // TOF_UTIL_USE_LUA

#ifdef TOF_UTIL_SCHEMA
#include "util_schema.h"
#endif

#endif
