#ifndef TOF_JSON_UTIL_H
#define TOF_JSON_UTIL_H

#include "yyjson/yyjson.h"
#include <mysql/mysql.h>
#include <stdbool.h>
#include <stdint.h>

// Serialization
typedef void (*struct2json_handler)(void *ptr, yyjson_mut_doc *json);

extern yyjson_mut_doc *get_json_from_str(void *ptr,
                                         struct2json_handler handler);
extern char *struct2jsonstr(void *ptr, struct2json_handler handler);

// utils for build json
extern void json_add_str_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                             const char *key, const char *val);
extern void json_add_int_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                             const char *key, const int64_t val);
extern void json_add_uint_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                              const char *key, const uint64_t val);
extern void json_add_real_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                              const char *key, const double val);
extern void json_add_bool_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                              const char *key, const bool val);
extern void json_add_arr_obj(yyjson_mut_doc *doc, yyjson_mut_val *root,
                             const char *key, yyjson_type type, void *arr,
                             size_t arr_len);
// Deserialization
typedef void (*json2struct_handler)(void *ptr, yyjson_doc *json);

extern void *get_pstruct_from_json(yyjson_doc *json,
                                   json2struct_handler handler);
extern void *get_pstruct_from_str(const char *json_str,
                                  json2struct_handler handler);

// Format
extern void json_printf(FILE *file, yyjson_doc *doc);
#endif // !TOF_JSON_UTIL_H
