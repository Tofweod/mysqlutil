#ifndef TOF_LOG_H
#define TOF_LOG_H

// log
#define LOG_LEVEL
#define LOG_INFO (1 << 0)
#define LOG_WARN (1 << 1)
#define LOG_ERROR (1 << 2)

#define INFO_STR(str) "\033[0;36m" str "\033[0m"
#define WARN_STR(str) "\033[0;33m" str "\033[0m"
#define ERROR_STR(str) "\033[0;31m" str "\033[0m"

#define PRINT(file, fmt, ...) fprintf(file, fmt, ##__VA_ARGS__)
#define INFO_PRINT(file, fmt, ...)                                             \
    PRINT(file, "%s:%d " INFO_STR("[ INFO ]: ") fmt, __FILE__, __LINE__,       \
          ##__VA_ARGS__)

#define WARN_PRINT(file, fmt, ...)                                             \
    PRINT(file, "%s:%d " WARN_STR("[ WARN ]: ") fmt, __FILE__, __LINE__,       \
          ##__VA_ARGS__)

#define ERROR_PRINT(file, fmt, ...)                                            \
    PRINT(file, "%s:%d " ERROR_STR("[ ERROR ]: ") fmt, __FILE__, __LINE__,     \
          ##__VA_ARGS__)

#endif
