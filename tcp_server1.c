#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX 1024
#define PORT 8080

int main() {
    int sockfd, connfd, len;

    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  // instead of SOCK_DGRAM as in UDP
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // add address details for binding
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if ((bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }

    // this actually does not listen but just prepares the queue for 5 incoming connections
    // and returns immeduately (it does not block the program).
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed.\n");
        exit(0);
    }

    len = sizeof(cli);

    // this call blocks as it waits for incoming connection establishment requests from clients
    // sockfd is used to accept incoming connections and after a connection is established with
    // a particluar client, connfd is returned. connfd will be used for communication with that
    // particular client going further while sockfd helps in establishment of new connections.
    // when a secind client connects, another fd (you can call it connfd1 or whatever you like)
    // will be returned by accept and that will be specific to communication between the server
    // and only that client.
    connfd = accept(sockfd, (struct sockaddr *) &cli, &len);
    if (connfd < 0) {
        printf("server accept failed.\n");
        exit(0);
    }

    int n = 0;
    char buffer[MAX];

    // we use recv instead of recvfrom because we use connfd and connfd already knows from where the
    // data is coming from as it is a connection-specific fd (it cannot be used to acceopt data from
    // all clients. it can only be used to accept data from that one particular clients with whom
    // connection has been established).
    n = recv(connfd, buffer, MAX, 0);
    buffer[n] = '\0';

    printf("From the client: \n");
    printf("%s\n", buffer);

    printf("Enter something to send to the client: \n");
    scanf("%[^\n]s", buffer);
    // similarly, we use send instead of sendto as connfd internally knows who the receiver is and there
    // is no need to explicitly mention the address again.
    send(connfd, buffer, strlen(buffer), 0);

    close(connfd);
    close(sockfd);
}
Footer
© 2022 GitHub, Inc.
