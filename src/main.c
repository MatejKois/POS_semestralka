#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "Menu/menu.h"
#include "Communication/server.h"

int main(int argc, char** argv) {
    // ./POS_semestralka server 10000
    // ./POS_semestralka client 10000

    char* newArgv[argc - 1];
    newArgv[0] = argv[0];
    for (int i = 2; i < argc; ++i) {
        newArgv[i - 1] = argv[i];
    } // odstranenie argumentu 1 z argv - nepotrebny

    if (strcmp(argv[1], "server") == 0) {
        server(argc, newArgv);
    } else if (strcmp(argv[1], "client") == 0) {
        menuClient(argv[2]);
    } else {
        fprintf(stderr, "Wrong arguments\n");
    }

    return 0;
}
