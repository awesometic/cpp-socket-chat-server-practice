#pragma once

namespace socketchatserver {

class Log {
    public:
        static void v(const char *fmt, ...);
        static void d(const char *fmt, ...);
        static void i(const char *fmt, ...);
        static void w(const char *fmt, ...);
        static void e(const char *fmt, ...);
};
}
