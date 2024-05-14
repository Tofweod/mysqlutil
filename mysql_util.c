#include "include/mysql_util.h"
#include "include/log.h"
#include <mysql/field_types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void stmt_error_handler(const char *err_msg, MYSQL_STMT *stmt,
                               MYSQL_BIND *params);

Connection_Info *set_host(Connection_Info *conn_info, const char *host) {
    if (!host) {
        ERROR_PRINT(stderr, "set host to NULL\n");
        return conn_info;
    }
    size_t len = strlen(host);
    conn_info->host = malloc(len);
    strncpy((void *)conn_info->host, host, len);
    ((char *)conn_info->host)[len] = '\0';
    return conn_info;
}

Connection_Info *set_user(Connection_Info *conn_info, const char *user) {
    if (!user) {
        ERROR_PRINT(stderr, "set host to NULL\n");
        return conn_info;
    }
    size_t len = strlen(user);
    conn_info->user = (char *)malloc(len);
    strncpy((void *)conn_info->user, user, len);
    ((char *)conn_info->user)[len] = '\0';
    return conn_info;
}

Connection_Info *set_passwd(Connection_Info *conn_info, const char *passwd) {
    if (!passwd) {
        ERROR_PRINT(stderr, "set host to NULL\n");
        return conn_info;
    }
    size_t len = strlen(passwd);
    conn_info->passwd = (char *)malloc(len);
    strncpy((void *)conn_info->passwd, passwd, len);
    ((char *)conn_info->passwd)[len] = '\0';
    return conn_info;
}

Connection_Info *set_db(Connection_Info *conn_info, const char *db) {
    if (!db) {
        ERROR_PRINT(stderr, "set host to NULL\n");
        return conn_info;
    }
    size_t len = strlen(db);
    conn_info->db = malloc(len);
    strncpy((void *)conn_info->db, db, len);
    ((char *)conn_info->db)[len] = '\0';
    return conn_info;
}

Connection_Info *set_port(Connection_Info *conn_info, const unsigned int port) {
    conn_info->port = port;
    return conn_info;
}

Connection_Info *set_unix_socket(Connection_Info *conn_info,
                                 const char *unix_socket) {
    if (!unix_socket) {
        conn_info->unix_socket = NULL;
        return conn_info;
    }
    size_t len = strlen(unix_socket);
    conn_info->unix_socket = malloc(len);
    strncpy((void *)conn_info->unix_socket, unix_socket, len);
    ((char *)conn_info->unix_socket)[len] = '\0';
    return conn_info;
}

Connection_Info *set_clientflag(Connection_Info *conn_info,
                                const unsigned long clientflag) {
    conn_info->clientflag = clientflag;
    return conn_info;
}

MYSQL *connect_by_info(MYSQL *conn, const Connection_Info *conn_info) {
    return mysql_real_connect(conn, conn_info->host, conn_info->user,
                              conn_info->passwd, conn_info->db, conn_info->port,
                              conn_info->unix_socket, conn_info->clientflag);
}

void stmt_error_handler(const char *err_msg, MYSQL_STMT *stmt,
                        MYSQL_BIND *params) {
    ERROR_PRINT(stderr, "%s\n", err_msg);
    mysql_stmt_close(stmt);
    free(params);
    stmt = NULL;
}

MYSQL_STMT *set_stmt(MYSQL_STMT *stmt, size_t argc, ...) {

    MYSQL_BIND *params = (MYSQL_BIND *)calloc(sizeof(MYSQL_BIND), argc);

    va_list args;
    va_start(args, argc);

    for (size_t i = 0; i < argc; i++) {
        t_v_pair *tv = va_arg(args, t_v_pair *);
        if (CHECK_TV_PAIR(tv) != 0) {
            stmt_error_handler("input wrong arg pair", stmt, params);
            goto STMT_ERROR;
        }
        if (set_param(tv->type, tv->value, tv->length, &params[i]) != 0) {
            printf("type is %d\n", tv->type);
            stmt_error_handler("unknown mysql type", stmt, params);
            goto STMT_ERROR;
        }
    }
    va_end(args);
    if (mysql_stmt_bind_param(stmt, params)) {
        stmt_error_handler("bind params error", stmt, params);
        goto STMT_ERROR;
    }
    free(params);
STMT_ERROR:
    return stmt;
}

int set_param(enum enum_field_types type, const void *data,
              unsigned long length, MYSQL_BIND *param) {
    param->buffer_type = type;
    param->buffer = (void *)data;
    // TODO: complete all type handler
    switch (type) {
    case MYSQL_TYPE_DECIMAL:
        break;
    case MYSQL_TYPE_TINY:
        break;
    case MYSQL_TYPE_SHORT:
        break;
    case MYSQL_TYPE_LONG:
        break;
    case MYSQL_TYPE_FLOAT:
        break;
    case MYSQL_TYPE_DOUBLE:
        break;
    case MYSQL_TYPE_NULL:
        break;
    case MYSQL_TYPE_TIMESTAMP:
        break;
    case MYSQL_TYPE_LONGLONG:
        break;
    case MYSQL_TYPE_INT24:
        break;
    case MYSQL_TYPE_DATE:
        break;
    case MYSQL_TYPE_TIME:
        break;
    case MYSQL_TYPE_DATETIME:
        break;
    case MYSQL_TYPE_YEAR:
        break;
    case MYSQL_TYPE_NEWDATE: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_VARCHAR:
        break;
    case MYSQL_TYPE_BIT:
        break;
    case MYSQL_TYPE_TIMESTAMP2:
        break;
    case MYSQL_TYPE_DATETIME2: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_TIME2: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_TYPED_ARRAY: /**< Used for replication only */
        break;
    case MYSQL_TYPE_INVALID:
        break;
    case MYSQL_TYPE_BOOL: /**< Currently just a placeholder */
        break;
    case MYSQL_TYPE_JSON:
        break;
    case MYSQL_TYPE_NEWDECIMAL:
        break;
    case MYSQL_TYPE_ENUM:
        break;
    case MYSQL_TYPE_SET:
        break;
    case MYSQL_TYPE_TINY_BLOB:
        break;
    case MYSQL_TYPE_MEDIUM_BLOB:
        break;
    case MYSQL_TYPE_LONG_BLOB:
        break;
    case MYSQL_TYPE_BLOB:
        param->buffer_length = length;
        break;
    case MYSQL_TYPE_VAR_STRING:
        break;
    case MYSQL_TYPE_STRING:
        // param->buffer = malloc(strlen(data));
        // memcpy(param->buffer, data, strlen(data));
        param->buffer_length = strlen(data);
        break;
    case MYSQL_TYPE_GEOMETRY:
        break;
    defalut:
        return -1;
    }
    return 0;
}

int set_res_param(enum enum_field_types type, void *data, unsigned long length,
                  MYSQL_BIND *param) {
    param->buffer_type = type;
    param->buffer = data;
    // TODO: complete all type handler
    switch (type) {
    case MYSQL_TYPE_DECIMAL:
        break;
    case MYSQL_TYPE_TINY:
        break;
    case MYSQL_TYPE_SHORT:
        break;
    case MYSQL_TYPE_LONG:
        break;
    case MYSQL_TYPE_FLOAT:
        break;
    case MYSQL_TYPE_DOUBLE:
        break;
    case MYSQL_TYPE_NULL:
        break;
    case MYSQL_TYPE_TIMESTAMP:
        break;
    case MYSQL_TYPE_LONGLONG:
        break;
    case MYSQL_TYPE_INT24:
        break;
    case MYSQL_TYPE_DATE:
        break;
    case MYSQL_TYPE_TIME:
        break;
    case MYSQL_TYPE_DATETIME:
        break;
    case MYSQL_TYPE_YEAR:
        break;
    case MYSQL_TYPE_NEWDATE: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_VARCHAR:
        break;
    case MYSQL_TYPE_BIT:
        break;
    case MYSQL_TYPE_TIMESTAMP2:
        break;
    case MYSQL_TYPE_DATETIME2: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_TIME2: /**< Internal to MySQL. Not used in protocol */
        break;
    case MYSQL_TYPE_TYPED_ARRAY: /**< Used for replication only */
        break;
    case MYSQL_TYPE_INVALID:
        break;
    case MYSQL_TYPE_BOOL: /**< Currently just a placeholder */
        break;
    case MYSQL_TYPE_JSON:
        break;
    case MYSQL_TYPE_NEWDECIMAL:
        break;
    case MYSQL_TYPE_ENUM:
        break;
    case MYSQL_TYPE_SET:
        break;
    case MYSQL_TYPE_TINY_BLOB:
        break;
    case MYSQL_TYPE_MEDIUM_BLOB:
        break;
    case MYSQL_TYPE_LONG_BLOB:
        break;
    case MYSQL_TYPE_BLOB:
        break;
    case MYSQL_TYPE_VAR_STRING:
        break;
    case MYSQL_TYPE_STRING:
        param->buffer_length = length;
        break;
    case MYSQL_TYPE_GEOMETRY:
        break;
    defalut:
        return -1;
    }
    return 0;
}

mysql_struct_frame *
_stmt_struct_register(MYSQL_STMT *stmt, size_t ptr_size, size_t param_size,
                      struct_handler handler) {
    if (!handler) {
        ERROR_PRINT(stderr, "no handler set\n");
        return NULL;
    }
    mysql_struct_frame *frame = malloc(sizeof(mysql_struct_frame));
    frame->data = malloc(ptr_size);
    frame->params = calloc(sizeof(MYSQL_BIND), param_size);
    frame->stmt = stmt;
    handler(frame->data, frame->params);
    mysql_stmt_bind_result(stmt, frame->params);
    return frame;
}

void stmt_struct_unregister(mysql_struct_frame *frame) {
    mysql_stmt_free_result(frame->stmt);
    free(frame->data);
    free(frame->params);
    free(frame);
}
