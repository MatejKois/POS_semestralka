#ifndef POS_SEMESTRALKA_SIMULATION_H
#define POS_SEMESTRALKA_SIMULATION_H

#include "../Canvas/canvas.h"
#include <pthread.h>

typedef struct argsSimulation {
    int* isPaused;
    int* changesCount;
    CANVAS* canvas;
    pthread_mutex_t* mutex;
    pthread_cond_t* condIsNotPaused;
} ARGS_SIMULATION;

static void* simulationPrint(void* params);

static void* simulationPause(void* params);

void simulationStart(CANVAS* canvas);

#endif //POS_SEMESTRALKA_SIMULATION_H
