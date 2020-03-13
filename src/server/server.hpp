#pragma once

#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_CONNECTIONS 32
#define EACH_MSG_SIZE   1024

namespace socketchatserver {

class Server {
    public:
        Server(int);
        ~Server();

        bool isSocketOpened();
        void start();
        int currentActiveUsers();

    private:
        int portNo;
        int socketFd;
        int newSocketFd[MAX_CONNECTIONS] = { -1, };
        socklen_t serverStorageAddrSize;
        sockaddr_in serverAddr;
        sockaddr_storage serverStorage;

        std::thread clientThreads[MAX_CONNECTIONS];
};

void clientThreadHandler(int);
}
