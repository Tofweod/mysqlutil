#include "../util.h"
#include "../lua_util.h"
#include <mysql/field_types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_info(Connection_Info *c_info) {
    printf("%s-%s-%s-%s-%u-%s-%lu\n", c_info->host, c_info->user,
           c_info->passwd, c_info->db, c_info->port, c_info->unix_socket,
           c_info->clientflag);
}


struct Actor_role {
    char mid[20];
    char aid[20];
    char role[20];
};

void Actor_role_handler(void *ptr, MYSQL_BIND *res) {
    struct Actor_role *ar = ptr;
    set_res_param(MYSQL_TYPE_STRING, ar->mid, 20, &res[0]);
    set_res_param(MYSQL_TYPE_STRING, ar->aid, 10, &res[1]);
    set_res_param(MYSQL_TYPE_STRING, ar->role, 20, &res[2]);
}

int main() {

    MYSQL *conn;

    conn = mysql_init(NULL);

    Connection_Info c_info;

    memset(&c_info, 0, sizeof(c_info));

    load_config(&c_info,"./config.lua");
    // set_host(&c_info, "0.0.0.0");
    // set_user(&c_info, "root");
    // set_passwd(&c_info, "2002madlife");
    // set_db(&c_info, "dbcourse");
    // set_port(&c_info, 55306);
    // set_unix_socket(&c_info, NULL);
    // set_clientflag(&c_info, 0);

    print_info(&c_info);

    if (connect_by_info(conn, &c_info) == NULL) {
        ERROR_PRINT("connection error:%s\n", mysql_error(conn));
        exit(1);
    }

    // if (mysql_real_connect(conn, "0.0.0.0", "root", "2002madlife",
    // "dbcourse",
    //                        55306, NULL, 0) == NULL) {
    //     fprintf(stderr, "connection error:%s\n", mysql_error(conn));
    //     exit(1);
    // }

    MYSQL_STMT *stmt;

    stmt = mysql_stmt_init(conn);

    const char *query = "select * from actor_role where MID=?";

    mysql_stmt_prepare(stmt, query, strlen(query));

    char p1[] = "100";

    set_stmt(stmt, 1, MAKE_ARG(MYSQL_TYPE_STRING, p1));

    // NOTE:here query option change to '200'
    // p1[0] = '2';

    mysql_stmt_execute(stmt);

    // p1[0] = '2';
    // MYSQL_BIND res[3];
    // memset(res, 0, sizeof(res));
    //
    // char res_str[3][20];
    //
    // for (int i = 0; i < 3; i++) {
    //     set_res_param(MYSQL_TYPE_STRING, res_str[i], 20, &res[i]);
    // }
    //
    // mysql_stmt_bind_result(stmt, res);
    //
    // mysql_stmt_store_result(stmt);
    //
    // while (mysql_stmt_fetch(stmt) == 0) {
    //     // res_str[0][0] = '2';
    //     printf("%s\t%s\t%s\n", res_str[0], res_str[1], res_str[2]);
    // }
    //
    // mysql_stmt_free_result(stmt);

    mysql_struct_frame *f =
        stmt_struct_register(stmt, struct Actor_role, 3, Actor_role_handler);

    // INFO_PRINT("%s %d %lu\n", (char *)f->params->buffer,
    // f->params->buffer_type,
    //            f->params->buffer_length);

    // ar for iteration
    struct Actor_role *ar = f->data;
    int status;
    while ((status = mysql_stmt_fetch(stmt)) != MYSQL_NO_DATA) {
        if (status == MYSQL_DATA_TRUNCATED)
#ifdef DEBUG
        {
            INFO_PRINT("data next line has been truncated\n");
        }
#else
            ;
#endif /* ifdef DEBUG */
        printf("%s\t%s\t%s\n", ar->aid, ar->mid, ar->role);
    }

    stmt_struct_unregister(f);

    mysql_stmt_close(stmt);

    mysql_close(conn);

    return 0;
}
