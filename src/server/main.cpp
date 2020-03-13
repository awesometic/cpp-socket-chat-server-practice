#include <cstdio>
#include <cstdlib>
#include "server.hpp"
#include "log.hpp"

using namespace socketchatserver;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        Log::e("No port provided");
        return 0;
    }

    Server server = Server(atoi(argv[1]));
    if (!server.isSocketOpened()) {
        Log::e("The socket seems cannot be bound");
        return 0;
    }

    server.start();

    return 0;
}
