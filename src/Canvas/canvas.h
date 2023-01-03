#ifndef POS_SEMESTRALKA_CANVAS_H
#define POS_SEMESTRALKA_CANVAS_H

#include <stdbool.h>

typedef struct canvas {
    int sizeX;
    int sizeY;
    bool** cells;
} CANVAS;

void canvasInit(int sizeX, int sizeY, CANVAS* canvas);

void canvasDispose(CANVAS* canvas);

int canvasCountNeighbors(int y, int x, CANVAS* canvas);

//returns the number of changes occured on update
int canvasUpdate(CANVAS* canvas);

void canvasPrint(CANVAS* canvas);

#endif //POS_SEMESTRALKA_CANVAS_H
