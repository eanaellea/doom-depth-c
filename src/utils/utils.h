#ifndef DOOM_DEPTH_C_UTILS_H
#define DOOM_DEPTH_C_UTILS_H

#include "../game_window/game_window.h"

#define min(a, b) ((a) < (b) ? (a) : (b))

#include "array.h"

char* custom_itoa(int value, char* str, int base);

char* custom_strupr(char* str);

void delay(ui_type_t ui_type, int ms);

#endif //DOOM_DEPTH_C_UTILS_H
