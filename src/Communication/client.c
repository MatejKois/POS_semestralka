#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "client.h"

int client(char* hostname, int portNo, CANVAS* canvas, bool save) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[1000];

    server = gethostbyname(hostname); //pretransformuje na ip adresu
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(portNo);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return 3;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return 4;
    }

    if (save) {
        bzero(buffer, 1000);
        strcat(buffer, "save\n");
        printf("Please enter a filename: ");
        char str[20];
        scanf("%s", str);
        strcat(buffer, str);
        strcat(buffer, "\n");
        sprintf(str, "%d\n", canvas->sizeY);
        strcat(buffer, str);
        sprintf(str, "%d\n", canvas->sizeX);
        strcat(buffer, str);
        for (int y = 0; y < canvas->sizeY; ++y) {
            for (int x = 0; x < canvas->sizeX; ++x) {
                if (canvas->cells[y][x]) {
                    strcat(buffer, "1");
                } else {
                    strcat(buffer, "0");
                }
            }
            strcat(buffer, "\n");
        }
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }

        printf("Saved!\n");
    } else { //load
        bzero(buffer, 1000);
        strcat(buffer, "load\n");
        printf("Please enter a filename: ");
        char str[20];
        scanf("%s", str);
        strcat(buffer, str);
        strcat(buffer, "\n");

        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            return 5;
        }

        bzero(buffer, 1000);
        n = read(sockfd, buffer, 999);
        if (n < 0) {
            perror("Error reading from socket");
            return 7;
        }

        char strLine[100];
        int indexLast = 0;
        int y, x;

        readLineFromBuffer(buffer, strLine, &indexLast);
        readLineFromBuffer(buffer, strLine, &indexLast);

        readLineFromBuffer(buffer, strLine, &indexLast);
        y = strtol(strLine, NULL, 10);
        readLineFromBuffer(buffer, strLine, &indexLast);
        x = strtol(strLine, NULL, 10);

        canvasInit(x, y, canvas);

        for (int i = 0; i < y; ++i) {
            readLineFromBuffer(buffer, strLine, &indexLast);
            for (int j = 0; j < x; ++j) {
                if (strLine[j] == '1') {
                    canvas->cells[i][j] = true;
                } else {
                    canvas->cells[i][j] = false;
                }
            }
        }

        canvasPrint(canvas);
        printf("Loaded!\n");
    }

    close(sockfd);

    return 0;
}

void readLineFromBuffer(char* buffer, char* strLine, int* indexLast) {
    int i = 0;
    while (true) {
        *indexLast = *indexLast + 1;
        if (buffer[*indexLast] == '\n') {
            strLine[i] = '\0';
            break;
        }
        strLine[i] = buffer[*indexLast];
        i++;
    }
}
