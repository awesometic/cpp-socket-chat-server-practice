#pragma once

#include <netinet/in.h>
#include <sys/socket.h>

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
};
}
