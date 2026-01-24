#include <iostream>
#include <sys/types.h>      // socket, bind
#include <sys/socket.h>     // socket, bind, listen, inet_ntoa
#include <netinet/in.h>     // htonl, htons, inet_ntoa
#include <arpa/inet.h>      // inet_ntoa
#include <netdb.h>          // gethostbyname
#include <unistd.h>         // read, write, close, usleep
#include <strings.h>        // bzero
#include <stdio.h>          // perror, fprintf
#include <stdlib.h>         // exit

using namespace std;

int main(int argc, char* argv[]) {
    string server_ip = "127.0.0.1";
    int server_port = 12345;

    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);

    if (inet_aton(server_ip.c_str(), &server_addr.sin_addr) == 0) {
        fprintf(stderr, "Invalid address\n");
        exit(1);
    }

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed\n");
        exit(1);
    }

    printf("Sending 10,000 packets to %s...\n", server_ip.c_str(), server_port);
    for (int i = 0; i < 1000; i++) {
        ssize_t sent_bytes = sendto(sockfd, &i, sizeof(i), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        if (sent_bytes < 0) perror("Sendto failed");

        usleep(1000);
    }

    cout << "Finished sending packets\n";

    close(sockfd);
    return 0;
}