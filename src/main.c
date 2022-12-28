#include <stdio.h>
#include "Canvas/canvas.h"

int main() {
    CANVAS canvas;
    canvasInit(10, 10, &canvas);

    canvas.cells[3][2] = true;
    canvas.cells[4][3] = true;
    canvas.cells[4][2] = true;
    canvasPrint(&canvas);

    canvasDispose(&canvas);
    return 0;
}
