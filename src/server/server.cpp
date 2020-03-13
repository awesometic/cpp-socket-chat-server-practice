#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.hpp"
#include "log.hpp"

namespace socketchatserver {

Server::Server(int portNo) {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        Log::e("Cannot open socket");
    }

    bzero((char *) &serverAddr, sizeof serverAddr);
    this->portNo = portNo;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(this->portNo);
    if (bind(socketFd, (struct sockaddr *) &serverAddr, sizeof serverAddr) < 0)
        Log::e("Cannot binding the socket file description");

    Log::i("Server started with port # %d", this->portNo);
}

Server::~Server() {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (clientThreads[i].joinable()) {
            clientThreads[i].join();
            close(newSocketFd[i]);
        }
    }
    close(socketFd);
}

bool Server::isSocketOpened() {
    return socketFd < 0 ? false : true;
}

void Server::start() {
    struct sockaddr_in clientAddr;

    if (listen(socketFd, MAX_CONNECTIONS) < 0)
        Log::e("Failed to set it up to listen state");
    Log::i("Listening started...");

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        serverStorageAddrSize = sizeof serverStorage;
        newSocketFd[i] = accept(socketFd, (struct sockaddr *) &serverStorage, &serverStorageAddrSize);
        clientThreads[i] = std::thread(clientThreadHandler, newSocketFd[i]);

        if (getsockname(newSocketFd[i], (struct sockaddr *) &clientAddr, &serverStorageAddrSize) > 0)
            Log::e("Error to get an information of the new client");
        else {
            Log::i("Accept a new client: %s", inet_ntoa(clientAddr.sin_addr));
        }
    }
}

void clientThreadHandler(int socketFd) {
    int newSocketFd = socketFd;
    int recvRet;
    char clientMessage[EACH_MSG_SIZE], buffer[EACH_MSG_SIZE];
    char *message, dummyData;

    while ((recvRet = recv(newSocketFd, clientMessage, EACH_MSG_SIZE, 0))) {
        if (recvRet < 0) {
            Log::e("Receiving data error by $d socket", newSocketFd);
            break;
        } else {
            message = (char *) malloc(sizeof clientMessage + 1);
            strcpy(message, clientMessage);
            strcat(message, "\n");
            strcpy(buffer, message);
            free(message);

            if (send(newSocketFd, buffer, sizeof buffer, 0) < 0) {
                Log::e("Sending data error by $d socket", newSocketFd);
                break;
            }
        }

        if (recv(newSocketFd, &dummyData, 1, MSG_PEEK) == 0) {
            Log::i("Client %d disconnected", newSocketFd);
            break;
        }
    }

    close(newSocketFd);
}
}
