#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Menu/menu.h"
#include "Communication/server.h"
#include "Communication/client.h"

int main(int argc, char** argv) {
//    CANVAS canvas;
//    canvasInit(10, 10, &canvas);
//
//    canvas.cells[2][4] = true;
//    canvas.cells[3][5] = true;
//    canvas.cells[4][5] = true;
//    canvas.cells[4][4] = true;
//    canvas.cells[4][3] = true;
//
//    simulationStart(&canvas);
//
//    canvasDispose(&canvas);

    // Ako jeden projekt:
    // ./cv9 server 10000
    // ./cv9 client 10000

    char* newArgv[argc - 1];
    newArgv[0] = argv[0];
    for (int i = 2; i < argc; ++i) {
        newArgv[i - 1] = argv[i];
    } // odstranenie argumentu 1 z argv - nepotrebny

    if (strcmp(argv[1], "server") == 0) {
        server(argc, newArgv);
    } else if (strcmp(argv[1], "client") == 0) {
        menuClient();
    } else {
        fprintf(stderr, "Wrong arguments\n");
    }

    return 0;
}
