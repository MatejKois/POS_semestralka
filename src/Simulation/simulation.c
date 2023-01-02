#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "simulation.h"

void simulationPause(CANVAS* pCanvas);

void simulationStart(CANVAS* canvas) {
    while (true) {
        canvasPrint(canvas);
        canvasUpdate(canvas);
        sleep(1);
    }
}
