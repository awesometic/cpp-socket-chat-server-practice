#pragma once

#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_CONNECTIONS 32
#define EACH_MSG_SIZE   1024

namespace socketchatserver {

class Server {
    public:
        Server(int portNo);
        ~Server();

        bool isSocketOpened();
        void listenStart();
        void acceptStart();
        void socketRead();
        void socketWrite();

    private:
        int portNo;
        int clientLength;
        int socketFd, newSocketFd;
        sockaddr_in serverAddr, clientAddr;
        pthread_mutex_t pthreadLock;

        void* clientThread(int *socketFd);
};
}
