#include "include/json_util.h"
#include "yyjson/yyjson.h"

static void json_printf_arr(FILE *file, yyjson_val *json_arr, int *indent,
                            bool is_root);
static void json_printf_obj(FILE *file, yyjson_val *json_obj, int *indent,
                            bool is_root);
static void json_printf_num(FILE *file, yyjson_val *val);

yyjson_mut_doc *get_json_from_str(void *ptr, struct2json_handler handler) {
    yyjson_mut_doc *doc = yyjson_mut_doc_new(NULL);
    handler(ptr, doc);
    return doc;
}

char *struct2jsonstr(void *ptr, struct2json_handler handler) {
    yyjson_mut_doc *doc = get_json_from_str(ptr, handler);
    char *str = yyjson_mut_write(doc, 0, NULL);
    yyjson_mut_doc_free(doc);
    return str;
}

void json_add_str_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                      const char *key, const char *val) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_val = yyjson_mut_strncpy(doc, val, strlen(val));
    yyjson_mut_obj_add(root, json_key, json_val);
}

void json_add_int_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                      const char *key, const int64_t val) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_val = yyjson_mut_int(doc, val);
    yyjson_mut_obj_add(root, json_key, json_val);
}

void json_add_uint_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                       const char *key, const uint64_t val) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_val = yyjson_mut_uint(doc, val);
    yyjson_mut_obj_add(root, json_key, json_val);
}

void json_add_real_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                       const char *key, const double val) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_val = yyjson_mut_uint(doc, val);
    yyjson_mut_obj_add(root, json_key, json_val);
}

void json_add_bool_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                       const char *key, const bool val) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_val = yyjson_mut_bool(doc, val);
    yyjson_mut_obj_add(root, json_key, json_val);
}

void json_add_arr_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                      const char *key, yyjson_type type, void *arr,
                      size_t arr_len) {
    yyjson_mut_val *json_key = yyjson_mut_strncpy(doc, key, strlen(key));
    yyjson_mut_val *json_arr;
    yyjson_mut_val *res;
    switch (type) {
    case YYJSON_SUBTYPE_SINT:
        json_arr = yyjson_mut_arr_with_sint64(doc, arr, arr_len);
        break;
    case YYJSON_SUBTYPE_UINT:
        json_arr = yyjson_mut_arr_with_uint64(doc, arr, arr_len);
        break;
    case YYJSON_SUBTYPE_REAL:
        json_arr = yyjson_mut_arr_with_real(doc, arr, arr_len);
        break;
    case YYJSON_TYPE_STR:
        json_arr = yyjson_mut_arr_with_strcpy(doc, arr, arr_len);
        break;
    case YYJSON_TYPE_BOOL:
        json_arr = yyjson_mut_arr_with_bool(doc, arr, arr_len);
        break;
    case YYJSON_TYPE_ARR:
        break;
    case YYJSON_TYPE_OBJ:
        json_arr = yyjson_mut_arr(doc);
        for (size_t i = 0; i < arr_len; i++) {
            res = (yyjson_mut_val *)arr + i;
            yyjson_mut_arr_append(json_arr, res);
        }
        break;
    }
    yyjson_mut_obj_add(root, json_key, json_arr);
}

static void json_printf_num(FILE *file, yyjson_val *val) {
    yyjson_subtype subtype = yyjson_get_subtype(val);
    switch (subtype) {
    case YYJSON_SUBTYPE_SINT:
        fprintf(file, "%ld", yyjson_get_sint(val));
        break;
    case YYJSON_SUBTYPE_UINT:
        fprintf(file, "%lu", yyjson_get_uint(val));
        break;
    case YYJSON_SUBTYPE_REAL:
        fprintf(file, "%lf", yyjson_get_real(val));
        break;
    }
}

#define TAB_SIZE 4
#define PRINT_TAB(indent) printf("%*s", indent, "");
static void json_printf_obj(FILE *file, yyjson_val *json_obj, int *indent,
                            bool is_root) {
    static char *bool_val[] = {"false", "true"};
    size_t idx, max;
    yyjson_val *key, *val;
    if (is_root)
        PRINT_TAB(*indent);
    printf("{\n");
    *indent += TAB_SIZE;
    yyjson_obj_foreach(json_obj, idx, max, key, val) {
        PRINT_TAB(*indent);
        yyjson_type type = yyjson_get_type(val);
        switch (type) {
        case YYJSON_TYPE_NUM:
            fprintf(file, "\"%s\":", yyjson_get_str(key));
            json_printf_num(file, val);
            break;
        case YYJSON_TYPE_STR:
            fprintf(file, "\"%s\":\"%s\"", yyjson_get_str(key),
                    yyjson_get_str(val));
            break;
        case YYJSON_TYPE_BOOL:
            fprintf(file, "\"%s\":%s", yyjson_get_str(key),
                    bool_val[yyjson_get_bool(val)]);
            break;
        case YYJSON_TYPE_OBJ:
            fprintf(file, "\"%s\":", yyjson_get_str(key));
            json_printf_obj(file, val, indent, false);
            break;
        case YYJSON_TYPE_ARR:
            fprintf(file, "\"%s\":", yyjson_get_str(key));
            json_printf_arr(file, val, indent, false);
            break;
        }
        if (idx < max - 1)
            fprintf(file, ",");
        fprintf(file, "\n");
    }
    *indent -= TAB_SIZE;
    PRINT_TAB(*indent);
    printf("}");
}

static void json_printf_arr(FILE *file, yyjson_val *json_arr, int *indent,
                            bool is_root) {
    static char *bool_val[] = {"false", "true"};
    size_t idx, max;
    yyjson_val *val;
    if (is_root)
        PRINT_TAB(*indent);
    printf("[\n");
    *indent += TAB_SIZE;
    yyjson_arr_foreach(json_arr, idx, max, val) {
        PRINT_TAB(*indent);
        yyjson_type type = yyjson_get_type(val);
        switch (type) {
        case YYJSON_TYPE_NUM:
            json_printf_num(file, val);
            break;
        case YYJSON_TYPE_STR:
            fprintf(file, "\"%s\"", yyjson_get_str(val));
            break;
        case YYJSON_TYPE_BOOL:
            fprintf(file, "\"%s\"", bool_val[yyjson_get_bool(val)]);
            break;
        case YYJSON_TYPE_OBJ:
            json_printf_obj(file, val, indent, false);
            break;
        case YYJSON_TYPE_ARR:
            json_printf_arr(file, val, indent, false);
            break;
        }
        if (idx < max - 1)
            fprintf(file, ",");
        fprintf(file, "\n");
    }
    *indent -= TAB_SIZE;
    PRINT_TAB(*indent);
    printf("]");
}

void json_printf(FILE *file, yyjson_doc *doc) {
    int indent = 0;
    yyjson_val *root = yyjson_doc_get_root(doc);
    yyjson_type type = yyjson_get_type(root);
    if (type == YYJSON_TYPE_OBJ)
        json_printf_obj(file, root, &indent, true);
    else if (type == YYJSON_TYPE_ARR)
        json_printf_arr(file, root, &indent, true);
    fprintf(file, "\n");
}
