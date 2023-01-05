#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "simulation.h"

static void* simulationPrint(void* params) {
    ARGS_SIMULATION* args = (ARGS_SIMULATION*)params;

    printf("Running simulation... Press any key to pause\n");

    while (*args->changesCount > 0) {
        sleep(1);
        pthread_mutex_lock(args->mutex);
        while (*args->isPaused == 1) {
            pthread_cond_wait(args->condIsNotPaused, args->mutex);
        }
        pthread_mutex_unlock(args->mutex);
        canvasPrint(args->canvas);
        *args->changesCount = canvasUpdate(args->canvas);
    }

    return NULL;
}

static void* simulationPause(void* params) {
    ARGS_SIMULATION* args = (ARGS_SIMULATION*)params;

    while (*args->changesCount > 0) {
        char buff[100];
        scanf("%s", buff);
        pthread_mutex_lock(args->mutex);
        *args->isPaused = 1;
        pthread_mutex_unlock(args->mutex);

        printf("Paused...press any key to continue: ");
        scanf("%s", buff);

        pthread_mutex_lock(args->mutex);
        *args->isPaused = 0;
        pthread_cond_signal(args->condIsNotPaused);
        pthread_mutex_unlock(args->mutex);
    }
    return NULL;
}

void simulationStart(CANVAS* canvas) {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_t condIsNotPaused;
    pthread_cond_init(&condIsNotPaused, NULL);
    pthread_t threadPrint;
    pthread_t threadPause;

    int* isPaused = malloc(sizeof(int));
    int* changesCount = malloc(sizeof(int));
    *changesCount = 5;

    ARGS_SIMULATION args = {
            isPaused,
            changesCount,
            canvas,
            &mutex,
            &condIsNotPaused
    };

    pthread_create(&threadPrint, NULL, &simulationPrint, &args);
    pthread_create(&threadPause, NULL, &simulationPause, &args);

    pthread_join(threadPrint, NULL);
    pthread_join(threadPause, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&condIsNotPaused);

    free(isPaused);
    free(changesCount);
}
