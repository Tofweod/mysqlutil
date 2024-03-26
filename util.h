#ifndef MY_MYSQL_UTIL
#define MY_MYSQL_UTIL

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

Connection_Info *set_host(Connection_Info *conn_info, const char *host);
Connection_Info *set_user(Connection_Info *conn_info, const char *user);
Connection_Info *set_passwd(Connection_Info *conn_info, const char *passwd);
Connection_Info *set_db(Connection_Info *conn_info, const char *db);
Connection_Info *set_port(Connection_Info *conn_info, const unsigned int port);
Connection_Info *set_unix_socket(Connection_Info *conn_info,
                                 const char *unix_socket);
Connection_Info *set_clientflag(Connection_Info *conn_info,
                                const unsigned long clientflag);

MYSQL *connect_by_info(MYSQL *conn, const Connection_Info *conn_info);

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
MYSQL_STMT *set_stmt(MYSQL_STMT *stmt, size_t argc, ...);

/**
 * param->buffer just points to value,thus need to ensure value dont change
 * before executing stmt
 * parameter length is especially for MYSQL_TYPE_BLOB to get the blob data's
 * length,otherwise just set this to 0
 */
int set_param(enum enum_field_types type, const void *value,
              unsigned long length, MYSQL_BIND *param);

/**
 * param->buffer just points to value,thus need to ensure value dont change
 * before getting the result
 */
int set_res_param(enum enum_field_types type, void *data, unsigned long length,
                  MYSQL_BIND *param);

#define MAKE_ARG(t, v, ...)                                                    \
    (t_v_pair *)&(t_v_pair) {                                                  \
        t_v_pair_magic_number, (t), (v), __VA_ARGS__                           \
    } // ... for length field

// get 0 when is t_v_pair
#define CHECK_TV_PAIR(pair) (!!((pair)->magic ^ t_v_pair_magic_number))

// log
#define LOG_LEVEL
#define LOG_INFO (1 << 0)
#define LOG_WARN (1 << 1)
#define LOG_ERROR (1 << 2)

#define INFO_STR(str) "\033[0;36m" str "\033[0m"
#define WARN_STR(str) "\033[0;33m" str "\033[0m"
#define ERROR_STR(str) "\033[0;31m" str "\033[0m"

#define PRINT(file, prefix, fmt, ...) fprintf(file, prefix fmt, ##__VA_ARGS__)
#define INFO_PRINT(fmt, ...)                                                   \
    PRINT(stdout, INFO_STR("[ INFO ]: "), fmt, ##__VA_ARGS__)

#define WARN_PRINT(fmt, ...)                                                   \
    PRINT(stderr, WARN_STR("[ WARN ]: "), fmt, ##__VA_ARGS__)
#define ERROR_PRINT(fmt, ...)                                                  \
    PRINT(stderr, ERROR_STR("[ ERROR ]: "), fmt, ##__VA_ARGS__)

typedef struct mysql_struct_frame {
    MYSQL_STMT *stmt;
    // item to iterator in mysql_stmt_fetch
    void *data;
    MYSQL_BIND *params;
} mysql_struct_frame;

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
mysql_struct_frame *
_stmt_struct_register(MYSQL_STMT *stmt, size_t ptr_size, size_t param_size,
                      void (*handler)(void *ptr, MYSQL_BIND *params));

#define stmt_struct_register(stmt, type, p_argc, handler)                      \
    _stmt_struct_register((stmt), sizeof(type), (p_argc), (handler))
/**
 * close stmt_binded_result,release resources in frame
 */
void stmt_struct_unregister(mysql_struct_frame *frame);

#define CHECK_PTR_OF(type, p) (1 ? p : (type)0)

#endif
