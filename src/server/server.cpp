#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.hpp"
#include "log.hpp"

// To keep compatibility with macOS
#if !defined(TCP_KEEPIDLE) && defined(TCP_KEEPALIVE)
#define TCP_KEEPIDLE TCP_KEEPALIVE
#endif

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

    clientKeepaliveConfig.keepcnt = 5;
    clientKeepaliveConfig.keepidle = 7200;
    clientKeepaliveConfig.keepintvl = 1;

    Log::i("Server started with port # %d", this->portNo);
}

Server::~Server() {
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (clientThreads[i].joinable()) {
            clientThreads[i].join();
            close(newSocketFds[i]);
        }
    }
    close(socketFd);
}

void Server::start() {
    struct sockaddr_in clientAddr;

    if (listen(socketFd, MAX_CONNECTIONS) < 0)
        Log::e("Failed to set it up to listen state");
    Log::i("Listening started...");

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        serverStorageAddrSize = sizeof serverStorage;
        newSocketFds[i] = accept(socketFd, (struct sockaddr *) &serverStorage, &serverStorageAddrSize);
        clientThreads[i] = std::thread(clientThreadHandler, i, newSocketFds);
        setTcpKeepaliveCfg(newSocketFds[i]);

        if (getsockname(newSocketFds[i], (struct sockaddr *) &clientAddr, &serverStorageAddrSize) > 0)
            Log::e("Error to get an information of the new client");
        else {
            Log::i("Accept a new client: %s", inet_ntoa(clientAddr.sin_addr));
            Log::i("Current user counts: %d", getCurrentActiveUsers());
        }
    }
}

bool Server::isSocketOpened() {
    return socketFd >= 0;
}

int Server::getCurrentActiveUsers() {
    int cnt = 0;

    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (newSocketFds[i] < 0)
            continue;
        else if (newSocketFds[i] != 0)
            cnt++;
    }

    return cnt;
}

/** Set the keepalive options on the socket
* This also enables TCP keepalive on the socket
*
* @param fd file descriptor
* @param fd file descriptor
* @return 0 on success -1 on failure
*/
void Server::setTcpKeepaliveCfg(int newSocketFd) {
    // TODO: Set them to make sure avoiding zombie socket
    // TODO: but don't know how it works exactly
    // TODO: so that it needs to be looked into again later
    int ret, optVal = 1;

    if (ret = setsockopt(newSocketFd,
        SOL_SOCKET, SO_KEEPALIVE,
        &optVal, sizeof optVal); ret != 0)
        Log::e("Cannot set TCP keepalive to be activated");
    Log::d("Activate TCP keepalive");

    if (ret = setsockopt(newSocketFd,
        IPPROTO_TCP, TCP_KEEPCNT,
        &clientKeepaliveConfig.keepcnt,
        sizeof clientKeepaliveConfig.keepcnt); ret != 0)
        Log::e("TCP_KEEPCNT cannot be set");
    Log::d("Set TCP_KEEPCNT to %d", clientKeepaliveConfig.keepcnt);

    if (ret = setsockopt(newSocketFd,
        IPPROTO_TCP, TCP_KEEPIDLE,
        &clientKeepaliveConfig.keepidle,
        sizeof clientKeepaliveConfig.keepidle); ret != 0)
        Log::e("TCP_KEEPIDLE/TCP_KEEPALIVE cannot be set");
    Log::d("Set TCP_KEEPIDLE/TCP_KEEPALIVE to %d", clientKeepaliveConfig.keepidle);

    if (ret = setsockopt(newSocketFd,
        IPPROTO_TCP, TCP_KEEPINTVL,
        &clientKeepaliveConfig.keepintvl,
        sizeof clientKeepaliveConfig.keepintvl); ret != 0)
        Log::e("TCP_KEEPINTVL cannot be set");
    Log::d("Set TCP_KEEPINTVL to %d", clientKeepaliveConfig.keepintvl);
}

void clientThreadHandler(int clientIndex, int *newSocketFds) {
    int recvRet;
    int newSocketFd = newSocketFds[clientIndex];
    char clientMessage[EACH_MSG_SIZE], buffer[EACH_MSG_SIZE];
    char *message, dummyData;

    while (recv(newSocketFd, &dummyData, 1, MSG_PEEK) != 0) {
        recvRet = recv(newSocketFd, clientMessage, EACH_MSG_SIZE, 0);

        if (recvRet < 0) {
            Log::e("Receiving data error by $d socket", newSocketFd);
            break;
        } else {
            message = (char *) malloc(sizeof clientMessage + 1);
            strcpy(message, clientMessage);
            strcat(message, "\n");
            strcpy(buffer, message);
            free(message);

            for (int i = 0; i < MAX_CONNECTIONS; i++) {
                if (i != clientIndex && newSocketFds[i] > 0) {
                    if (send(newSocketFds[i], buffer, sizeof buffer, 0) < 0) {
                        Log::e("Sending data error by $d socket", newSocketFds[i]);
                    }
                }
            }
        }
    }

    Log::i("Client %d disconnected", newSocketFd);
    newSocketFds[clientIndex] = -1;
    close(newSocketFd);
}
}
