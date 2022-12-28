#include <malloc.h>
#include "canvas.h"

void canvasInit(int sizeX, int sizeY, CANVAS* canvas) {
    canvas->sizeX = sizeX;
    canvas->sizeY = sizeY;
    canvas->cells = malloc(sizeof(bool*) * sizeY);
    for (int y = 0; y < sizeY; ++y) {
        canvas->cells[y] = malloc(sizeof(bool) * sizeX);
    }
}

void canvasDispose(CANVAS* canvas) {
    for (int y = 0; y < canvas->sizeY; ++y) {
        free(canvas->cells[y]);
    }
    free(canvas->cells);
    canvas->sizeX = 0;
    canvas->sizeY = 0;
}

int canvasCountNeighbors(int y, int x, CANVAS* canvas) {
    int stepsY[] = {-1, 0, 0, 1, 1, 0, 0, -1};
    int stepsX[] = {-1, 1, 1, 0, 0, -1, -1, 0};

    int count = 0;

    for (int i = 0; i < 8; ++i) {
        y += stepsY[i];
        x += stepsX[i];
        if (y < 0 || x < 0 || y > canvas->sizeY - 1 || x > canvas->sizeX - 1) {
            continue;
        }
        if (canvas->cells[y][x]) {
            ++count;
        }
    }

    return count;
}

void canvasPrint(CANVAS* canvas) {
    for (int i = 0; i < canvas->sizeY + 1; ++i) {
        printf("---");
    }
    printf("\n yx");

    for (int i = 0; i < canvas->sizeX; ++i) {
        printf(" %d ", i);
    }
    printf("\n");

    for (int y = 0; y < canvas->sizeY; ++y) {
        printf(" %d ", y);
        for (int x = 0; x < canvas->sizeX; ++x) {
            printf("[%s]", canvas->cells[y][x] ? "o" : " ");
        }
        printf("\n");
    }

    for (int i = 0; i < canvas->sizeY + 1; ++i) {
        printf("---");
    }
    printf("\n");
}
