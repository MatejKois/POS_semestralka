#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define CONNECTED_MAX_NO 2

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
    //                       SOCK_STREAM - TCP, SOCK_DGRAM - UDP
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

    listen(sockfd,
           CONNECTED_MAX_NO); //vytvorenie pasivneho socketu - po pripojeni nan vznikne vlastny socket pre komunikovanie

    cli_len = sizeof(cli_addr);

    int somethingOpened = 0;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_t condIsNotOpened;
    pthread_cond_init(&condIsNotOpened, NULL);

    ARGS_SERVER_QUIT argsServerQuit = {
            &sockfd,
            &mutex,
            &condIsNotOpened
    };

    pthread_t threadQuit;
    pthread_create(&threadQuit, NULL, &serverQuitThread, &argsServerQuit);

    pthread_t threadsSaveLoad[CONNECTED_MAX_NO];
    int connectedPosition = 0;

    while (1) {
        if (connectedPosition >= CONNECTED_MAX_NO) {
            printf("Server busy! Please hold on...\n");
            for (int i = 0; i < connectedPosition; ++i) {
                pthread_join(threadsSaveLoad[i], NULL);
            }
            connectedPosition = 0;
        }

        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr,
                           &cli_len); //tu program zastavi a caka kym sa niekto pripoji

        ARGS_SERVER_SAVE_LOAD argsServerSaveLoad = {
                &somethingOpened,
                &newsockfd,
                &mutex,
                &condIsNotOpened
        };
        pthread_create(&threadsSaveLoad[connectedPosition], NULL, &serverSaveLoadThread, &argsServerSaveLoad);
        ++connectedPosition;
    }
}

void* serverQuitThread(void* params) {
    ARGS_SERVER_QUIT* args = (ARGS_SERVER_QUIT*)params;

    char buff[10];
    printf("Press 'q' anytime to quit\n");
    scanf("%s", buff);
    printf("Quitting...\n");

    close(*args->sockfdToClose);

    pthread_mutex_destroy(args->mutex);
    pthread_cond_destroy(args->condIsNotOpened);

    exit(0);
}

void* serverSaveLoadThread(void* params) {
    ARGS_SERVER_SAVE_LOAD* args = (ARGS_SERVER_SAVE_LOAD*)params;
    int newsockfdSaveLoad;
    pthread_mutex_lock(args->mutex);
    newsockfdSaveLoad = *args->newsockfd;
    pthread_mutex_unlock(args->mutex);

    int n;
    char buffer[1000];

    if (newsockfdSaveLoad < 0) {
        perror("ERROR on accept");
        close(newsockfdSaveLoad);
        printf("Client disconnected from socket %d\n", newsockfdSaveLoad);
        return NULL;
    }

    printf("Client connected to socket %d\n", newsockfdSaveLoad);

    bzero(buffer, 1000);
    n = read(newsockfdSaveLoad, buffer, 999); //nacitanie do buffera
    if (n < 0) {
        perror("Error reading from socket");
        close(newsockfdSaveLoad);
        printf("Client disconnected from socket %d\n", newsockfdSaveLoad);
        return NULL;
    }

    char filename[100];
    strcat(filename, "savegame/");
    int i = 5;
    while (1) {
        if (buffer[i] == '\n') {
            filename[i + 4] = '\0';
            strcat(filename, ".txt");
            break;
        }
        filename[i + 4] = buffer[i];
        ++i;
    }
    printf("Client on socket %d: request> %s filename '%s'\n", newsockfdSaveLoad, buffer[0] == 's' ? "save" : "load",
           filename);

    pthread_mutex_lock(args->mutex);
    while (*args->somethingOpened == 1) {
        pthread_cond_wait(args->condIsNotOpened, args->mutex);
    }
    *args->somethingOpened = 1;
    pthread_mutex_unlock(args->mutex);

    if (buffer[0] == 's') { //save
        FILE* out = fopen(filename, "w");

        if (!out) {
            printf("Error: file not found\n");
            pthread_mutex_lock(args->mutex);
            *args->somethingOpened = 0;
            pthread_cond_signal(args->condIsNotOpened);
            pthread_mutex_unlock(args->mutex);
            close(newsockfdSaveLoad);
            printf("Client disconnected from socket %d\n", newsockfdSaveLoad);
            return NULL;
        }

        fprintf(out, buffer);

        fclose(out);
    } else { //load
        FILE* in = fopen(filename, "r");
        bzero(buffer, 1000);

        if (!in) {
            printf("Error: file not found\n");
            pthread_mutex_lock(args->mutex);
            *args->somethingOpened = 0;
            pthread_cond_signal(args->condIsNotOpened);
            pthread_mutex_unlock(args->mutex);
            close(newsockfdSaveLoad);
            printf("Client disconnected from socket %d\n", newsockfdSaveLoad);
            return NULL;
        }

        int j = 0;
        while (!feof(in)) {
            char c = getc(in);
            buffer[j] = c;
            ++j;
        }
        buffer[j + 1] = '\0';

        n = write(newsockfdSaveLoad, buffer, 999);
        if (n < 0) {
            perror("Error writing to socket");
            pthread_mutex_lock(args->mutex);
            *args->somethingOpened = 0;
            pthread_cond_signal(args->condIsNotOpened);
            pthread_mutex_unlock(args->mutex);
            close(newsockfdSaveLoad);
            printf("Client disconnected from socket %d\n", newsockfdSaveLoad);
            return NULL;
        }

        fclose(in);
    }

    pthread_mutex_lock(args->mutex);
    *args->somethingOpened = 0;
    pthread_cond_signal(args->condIsNotOpened);
    pthread_mutex_unlock(args->mutex);

    close(newsockfdSaveLoad);
    printf("Client disconnected from socket %d\n", newsockfdSaveLoad);

    return NULL;
}
