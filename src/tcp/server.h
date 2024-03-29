#ifndef CV_SERVER_H
#define CV_SERVER_H

#include <stdio.h>
#include <string.h> /* memset() */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#define PORT    "32001" /* Port to listen on */
#define BACKLOG 10  /* Passed to listen() */

class server {

public:
    void handle(int newsock)
    {
        /* recv(), send(), close() */
    }

    int start()
    {
        int sock;
        struct addrinfo hints, *res;
        int reuseaddr = 1; /* True */

        /* Get the address info */
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(NULL, PORT, &hints, &res) != 0) {
            perror("getaddrinfo");
            return 1;
        }

        /* Create the socket */
        sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock == -1) {
            perror("socket");
            return 1;
        }

        /* Enable the socket to reuse the address */
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
            perror("setsockopt");
            return 1;
        }

        /* Bind to the address */
        if (bind(sock, res->ai_addr, res->ai_addrlen) == -1) {
            perror("bind");
            return 1;
        }

        /* Listen */
        if (listen(sock, BACKLOG) == -1) {
            perror("listen");
            return 1;
        }

        freeaddrinfo(res);

        /* Main loop */
        while (1) {
            socklen_t size = sizeof(struct sockaddr_in);
            struct sockaddr_in their_addr;
            int newsock = accept(sock, (struct sockaddr*)&their_addr, &size);

            if (newsock == -1) {
                perror("accept");
            }
            else {
                printf("Got a connection from %s on port %d\n",
                       inet_ntoa(their_addr.sin_addr), htons(their_addr.sin_port));
                handle(newsock);
            }
        }

        close(sock);

        return 0;
    }
};


#endif //CV_SERVER_H
