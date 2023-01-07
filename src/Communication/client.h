#ifndef CV9_CLIENT_H
#define CV9_CLIENT_H

#include <stdbool.h>
#include "../Canvas/canvas.h"

int client(char* hostname, int portNo, CANVAS* canvas, bool save);

void readLineFromBuffer(char* buffer, char* strLine, int* indexLast);

#endif //CV9_CLIENT_H
