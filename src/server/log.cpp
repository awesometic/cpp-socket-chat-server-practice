#include <cstdio>
#include <string>
#include <cstdarg>
#include <cerrno>
#include "log.hpp"

namespace socketchatserver {

void Log::v(const char *fmt, ...) {
    char msg[1024] = { 0, };

    va_list ap;
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    printf("Verbose: %s\n", msg);
}

void Log::d(const char *fmt, ...) {
    char msg[1024] = { 0, };

    va_list ap;
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    printf("Debug: %s\n", msg);
}

void Log::i(const char *fmt, ...) {
    char msg[1024] = { 0, };

    va_list ap;
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    printf("Info: %s\n", msg);
}

void Log::w(const char *fmt, ...) {
    char msg[1024] = { 0, };

    va_list ap;
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    printf("Warning: %s\n", msg);
}

void Log::e(const char *fmt, ...) {
    char msg[1024] = { 0, };

    va_list ap;
    va_start(ap, fmt);
    vsprintf(msg, fmt, ap);
    va_end(ap);

    if (errno == 0) {
        printf("Error: %s\n", msg);
    } else {
        fprintf(stderr, "Error: %s: %s\n", msg, strerror(errno));
    }
}
}
