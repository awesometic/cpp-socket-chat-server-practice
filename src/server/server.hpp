#pragma once

#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_CONNECTIONS 32
#define EACH_MSG_SIZE   1024

namespace socketchatserver {

struct KeepaliveConfig {
    int keepidle;
    int keepcnt;
    int keepintvl;
};

class Server {
    public:
        Server(int);
        ~Server();

        void start();
        bool isSocketOpened();
        bool isClientAlive(int);
        int getCurrentActiveUsers();
        void setTcpKeepaliveCfg(int);

    private:
        int portNo;
        int socketFd;
        int newSocketFds[MAX_CONNECTIONS] = { -1, };
        struct KeepaliveConfig clientKeepaliveConfig;
        socklen_t serverStorageAddrSize;
        sockaddr_in serverAddr;
        sockaddr_storage serverStorage;

        std::thread clientThreads[MAX_CONNECTIONS];
};

void clientThreadHandler(int, int*);
}
