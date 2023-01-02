#include <stdio.h>
#include "Canvas/canvas.h"

int main() {
    CANVAS canvas;
    canvasInit(10, 10, &canvas);

    canvas.cells[2][4] = true;
    canvas.cells[3][5] = true;
    canvas.cells[4][5] = true;
    canvas.cells[4][4] = true;
    canvas.cells[4][3] = true;

    canvasPrint(&canvas);
    canvasUpdate(&canvas);
    canvasPrint(&canvas);
    canvasUpdate(&canvas);
    canvasPrint(&canvas);
    canvasUpdate(&canvas);
    canvasPrint(&canvas);
    canvasUpdate(&canvas);

    canvasDispose(&canvas);
    return 0;
}