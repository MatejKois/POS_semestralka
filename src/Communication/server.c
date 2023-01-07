#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int server(int argc, char* argv[]) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[1000];

    if (argc < 2) {
        fprintf(stderr, "usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr)); //vycisti strukturu

    serv_addr.sin_family = AF_INET; //internetove sockety
    serv_addr.sin_addr.s_addr = INADDR_ANY; //maska - zodpoveda pocuvaniu celeho internetu
    serv_addr.sin_port = htons(
            atoi(argv[1])); //cislo portu na pocuvanie, htons prevedie format cisla z little na big endian
    //                                   SOCK_STREAM - TCP, SOCK_DGRAM - UDP
    sockfd = socket(AF_INET, SOCK_STREAM, 0); //systemove volanie, vracia identifikacne cislo socketu

    if (sockfd < 0) {
        perror("Error creating socket");
        return 1;
    }
    //nabindovanie struktury na socket - poviem mu kde bude pracovat a na akom porte
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return 2;
    }
    //                    5 - kolko klientov sa moze pripojit na socket v jednom okamihu (velkost frontu neobsluzenych klientov)
    listen(sockfd, 5); //vytvorenie pasivneho socketu - po pripojeni nan vznikne vlastny socket pre komunikovanie

    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr,
                       &cli_len); //tu program zastavi a caka kym sa niekto pripoji

    if (newsockfd < 0) {
        perror("ERROR on accept");
        return 3;
    }

    bzero(buffer, 1000);
    n = read(newsockfd, buffer, 999); //nacitanie do buffera
    if (n < 0) {
        perror("Error reading from socket");
        return 5;
    }

    char filename[100];
    int i = 5;
    while (1) {
        if (buffer[i] == '\n') {
            filename[i - 5] = '.';
            filename[i - 4] = 't';
            filename[i - 3] = 'x';
            filename[i - 2] = 't';
            filename[i - 1] = '\0';
            break;
        }
        filename[i - 5] = buffer[i];
        ++i;
    }

    if (buffer[0] == 's') { //save
        FILE* out = fopen(filename, "w");
        fprintf(out, buffer);
        fclose(out);
    } else { //load
        FILE* in = fopen(filename, "r");
        bzero(buffer, 1000);

        int j = 0;
        while (!feof(in)) {
            char c = getc(in);
            buffer[j] = c;
            ++j;
        }
        buffer[j + 1] = '\0';

        n = write(newsockfd, buffer, 999);
        if (n < 0) {
            perror("Error writing to socket");
            return 6;
        }

        fclose(in);
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}
