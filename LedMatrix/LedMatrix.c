#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_ledMatrix/ledMatrix_lib.h"


#define LED_COUNT 25 // Número de LEDs na matriz
#define LED_PIN 7    // Pino GPIO conectado à matriz de LEDs
#define DELAY 100 // ms para efeitos de fade

int main() {
    stdio_init_all();

    npInit(LED_PIN);

    //Acende todos os LEDs de vermelho
    for (int i = 0; i < LED_COUNT; i++) {
        npSetLED(i, 255, 0, 0);
    }
    npWrite();
    sleep_ms(1000);

    //Fade de intensidade (brilho) em verde
    for (int brightness = 0; brightness <= 255; brightness += 5) {
        for (int i = 0; i < LED_COUNT; i++) {
            npSetLED(i, 0, brightness, 0);
        }
        npWrite();
        sleep_ms(DELAY);
    }
    for (int brightness = 255; brightness >= 0; brightness -= 5) {
        for (int i = 0; i < LED_COUNT; i++) {
            npSetLED(i, 0, brightness, 0);
        }
        npWrite();
        sleep_ms(DELAY);
    }

    //Fade de cor (vermelho para azul)
    for (int t = 0; t <= 255; t += 5) {
        for (int i = 0; i < LED_COUNT; i++) {
            npSetLED(i, 255 - t, 0, t);
        }
        npWrite();
        sleep_ms(DELAY);
    }
    for (int t = 255; t >= 0; t -= 5) {
        for (int i = 0; i < LED_COUNT; i++) {
            npSetLED(i, 255 - t, 0, t);
        }
        npWrite();
        sleep_ms(DELAY);
    }

    //Apaga todos os LEDs
    npClear();
    npWrite();
    sleep_ms(1000);

    //Acende LEDs em zigue-zague usando setLedInPosition
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            if( y % 2 == 0) {
                // Linha par: acende LEDs da esquerda para a direita
                setLedInPosition(x, y, 0, 0, 255);
            } else {
                // Linha ímpar: acende LEDs da direita para a esquerda
                setLedInPosition(4 - x, y, 0, 0, 255);
            }
            npWrite();
            sleep_ms(100);
        }
    }

    // Limpa tudo novamente
    npClear();
    npWrite();

    // Define algumas cores em hexadecimal
    uint32_t colors[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF, 0x000000};
    int num_colors = sizeof(colors) / sizeof(colors[0]);

    // Mostra cada cor em toda a matriz, uma de cada vez
    for (int c = 0; c < num_colors; c++) {
        for (int y = 0; y < 5; y++) {
            for (int x = 0; x < 5; x++) {
                setLedInPositionWithHexa(x, y, colors[c]);
            }
        }
        npWrite();
        sleep_ms(500); 
    }


    // Limpa tudo novamente
    npClear();
    npWrite();

    // Loop infinito
    while (1) {
        tight_loop_contents();
    }
}
