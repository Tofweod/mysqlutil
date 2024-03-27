#ifndef LOG_H
#define LOG_H

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


#endif
