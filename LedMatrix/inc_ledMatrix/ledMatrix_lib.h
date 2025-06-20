#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include "pico/stdlib.h"


void npInit(uint pin);

void npWrite();

uint8_t reverse_byte(uint8_t b);

void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b);

void setLedInPosition(int X, int Y, const uint8_t r, const uint8_t g, const uint8_t b);

void setLedInPositionWithHexa(int X, int Y, const uint32_t color);

void npClear();



#endif