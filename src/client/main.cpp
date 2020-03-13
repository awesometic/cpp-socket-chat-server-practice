#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");

    while (true) {
        printf("Enter the message (/q to quit): ");
        fgets(buffer, sizeof buffer, stdin);

        if (!strcmp(buffer, "q\n"))
            break;

        if (send(sockfd, buffer, sizeof buffer, 0) < 0) {
            fprintf(stderr," ERROR failed to send a message\n");
            break;
        }

        if (recv(sockfd, buffer, sizeof buffer, 0) <= 0) {
            fprintf(stderr, "ERROR disconnected to server");
        }

        printf("Message from server: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
