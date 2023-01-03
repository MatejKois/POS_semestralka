#include <unistd.h>
#include "simulation.h"

void simulationStart(CANVAS* canvas) {
    canvasPrint(canvas);
    while (canvasUpdate(canvas) > 0) {
        canvasPrint(canvas);
        sleep(1);
    }
}
