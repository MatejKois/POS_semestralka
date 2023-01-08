#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"
#include "../Simulation/simulation.h"
#include "../Communication/client.h"

static int menuPortNumber = 10000;

void menuClient(char* host, char* port) {
    while (1) {
        menuPortNumber = strtol(port, NULL, 10);
        CANVAS canvas;
        printf("Welcome to game of life!\n");
        printf("1 - create layout\n");
        printf("2 - download from server\n");
        printf("3 - quit\n");
        char buff[10];
        scanf("%s", buff);
        if (strcmp(buff, "2") == 0) {
            if (client(host, menuPortNumber, &canvas, false) == 0) {
                printf("Press any key to start the simulation: ");
                scanf("%s", buff);
                simulationStart(&canvas);
                canvasDispose(&canvas);
            }
        } else if (strcmp(buff, "1") == 0) {
            int y = 0;
            int x = 0;
            while (1) {
                printf("Enter Y dimension: ");
                scanf("%s", buff);
                y = strtol(buff, NULL, 10);
                printf("Enter X dimension: ");
                scanf("%s", buff);
                x = strtol(buff, NULL, 10);

                if (y > 30 || x > 30) {
                    printf("Too large! (Enter max 30 for both dimensions)...\n");
                } else {
                    break;
                }
            }

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
        } else {
            break;
        }
    }
}

void menuPause(CANVAS* canvas) {
    printf("Paused...\nPress s to save your progress or any other key to continue: ");
    char buff[10];
    scanf("%s", buff);
    if (strcmp(buff, "s") == 0) {
        client("frios2.fri.uniza.sk", menuPortNumber, canvas, true);
    }
}
