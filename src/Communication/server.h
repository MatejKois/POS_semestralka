#ifndef CV9_SERVER_H
#define CV9_SERVER_H

#include <pthread.h>

typedef struct argsServerSaveLoad {
    int* somethingOpened;
    int* newsockfd;
    pthread_mutex_t* mutex;
    pthread_cond_t* condIsNotOpened;
} ARGS_SERVER_SAVE_LOAD;

typedef struct argsServerQuit {
    int* sockfdToClose;
    pthread_mutex_t* mutex;
    pthread_cond_t* condIsNotOpened;
} ARGS_SERVER_QUIT;

int server(int argc, char* argv[]);

static void* serverQuitThread(void* params);

static void* serverSaveLoadThread(void* params);

#endif //CV9_SERVER_H
