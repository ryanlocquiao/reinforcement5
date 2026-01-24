#include <iostream>
#include <sys/types.h>      // socket, bind
#include <sys/socket.h>     // socket, bind, listen, inet_ntoa
#include <netinet/in.h>     // htonl, htons, inet_ntoa
#include <arpa/inet.h>      // inet_ntoa
#include <netdb.h>          // gethostbyname
#include <unistd.h>         // read, write, close
#include <strings.h>        // bzero
#include <netinet/tcp.h>    // SO_REUSEADDR
#include <sys/uio.h>        // writev
#include <stdio.h>          // perror, printf
#include <stdlib.h>         // atoi, exit

using namespace std;

const unsigned int BUF_SIZE = 65535;

int main(int argc, char* argv[]) {
    int server_port = (argc > 1) ? atoi(argv[1]) : 12345;

    struct sockaddr_in server_addr;
    bzero((char*)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);

    // Changed: SOCK_STREAM (TCP) --> SOCK_DGRAM (UDP)
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed\n");
        exit(1);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        perror("Setsockopt failed\n");

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed\n");
        exit(1);
    }

    printf("UDP Server listening on port %d...\n", server_port);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int received_seq_num;

        ssize_t bytesRead = recvfrom(server_fd, &received_seq_num, sizeof(received_seq_num), 0, (struct sockaddr*)&client_addr, &client_len);

        if (bytesRead > 0) {
            cout << "Received Packet Sequence Number: " << received_seq_num << endl;
            if (received_seq_num == 999) {
                cout << "Final packet received. Shutting down server.\n";
                break;
            }
        } else {
            perror("Recvfrom error\n");
        }
    }

    close(server_fd);
    return 0;
}