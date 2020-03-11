#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "server.hpp"
#include "log.hpp"

/**
 * Steps of TCP/IP socket communication - Server
 * 1. socket()
 * 2. bind()
 * 3. listen()
 * 4. accept() when a client does connect()
 * 5. read() / write() when a client does the same thing
 * 6. close()
 */

namespace socketchatserver {

Server::Server(int portNo) {
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd < 0) {
        Log::e("Cannot open socket.");
    }

    bzero((char *) &(serverAddr), sizeof(serverAddr));
    this->portNo = portNo;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(this->portNo);
    if (bind(socketFd, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
        Log::e("Cannot binding the socket file description.");

    pthreadLock = PTHREAD_MUTEX_INITIALIZER;
}

Server::~Server() {
    close(newSocketFd);
    close(socketFd);
}

bool Server::isSocketOpened() {
    return socketFd < 0 ? false : true;
}

void Server::listenStart() {
    if (listen(socketFd, MAX_CONNECTIONS) < 0)
        Log::e("Failed to set it up to listen state.");
}

void Server::acceptStart() {
    clientLength = sizeof(clientAddr);
    newSocketFd = accept(socketFd, (struct sockaddr *) &clientAddr, (socklen_t *) &clientLength);
    if (newSocketFd < 0)
        Log::e("Cannot start accept.");
}

void Server::socketRead() {
    char buffer[256];

    bzero(buffer, 256);
    if (read(newSocketFd, buffer, 255) < 0)
        Log::e("Reading from the socket.");

    printf("From client: %s\n", buffer);
}

void Server::socketWrite() {
    char buffer[256];

    bzero(buffer, 256);
    if (write(newSocketFd, "I got your message", 18) < 0)
        Log::e("Writing to the socket.");
}

void* Server::clientThread(int *socketFd) {
    int newSocket = *socketFd;
    char clientMessage[EACH_MSG_SIZE], buffer[EACH_MSG_SIZE];
    char *message;

    recv(newSocket, clientMessage, EACH_MSG_SIZE, 0);

    pthread_mutex_lock(&pthreadLock);
    message = (char *) malloc(sizeof(clientMessage) + 20);
    strcpy(message, "Hello Client : ");
    strcat(message, clientMessage);
    strcat(message, "\n");
    strcpy(buffer, message);
    free(message);
    pthread_mutex_unlock(&pthreadLock);

    sleep(1);
    send(newSocket, buffer,13,0);
    printf("Exit socketThread \n");
    close(newSocket);
    pthread_exit(NULL);
}
}
