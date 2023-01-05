#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "../Canvas/canvas.h"
#include "../Simulation/simulation.h"

void menuClient() {
    printf("Welcome to game of life!\n");
    printf("1 - create layout\n");
    printf("2 - download from server\n");
    char buff[10];
    scanf("%s", buff);
    if (strcmp(buff, "2") == 0) {

    } else {
        printf("Enter Y dimension: ");
        scanf("%s", buff);
        int y = strtol(buff, NULL, 10);
        printf("Enter X dimension: ");
        scanf("%s", buff);
        int x = strtol(buff, NULL, 10);

        CANVAS canvas;
        canvasInit(x, y, &canvas);

        printf("Please enter cell coordinates, one by one:\n");
        while (1) {
            printf("Y: ");
            scanf("%s", buff);
            y = strtol(buff, NULL, 10);

            printf("X: ");
            scanf("%s", buff);
            x = strtol(buff, NULL, 10);

            if (y < 0 || x < 0 || y > canvas.sizeY - 1 || x > canvas.sizeX - 1) {}
            canvas.cells[y][x] = true;

            printf("Continue? y/n: ");
            scanf("%s", buff);
            if (strcmp(buff, "n") == 0) {
                break;
            }
        }

        printf("Press any key to start the simulation: ");
        scanf("%s", buff);

        simulationStart(&canvas);

        canvasDispose(&canvas);
    }
}

void menuPause() {

}
