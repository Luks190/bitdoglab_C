#include <string.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "inc_OLED/ssd1306.h"


#define DELAY 40

int main() {
    stdio_init_all();
    ssd1306_t disp;
    ssd1306_init_display(&disp);

    // 1. Efeito de Fade-in com quadrados
    for (int size = 2; size < 64; size += 4) {
        ssd1306_clear(&disp);
        ssd1306_draw_square(&disp, (128-size)/2, (64-size)/2, size, size);
        ssd1306_show(&disp);
        sleep_ms(DELAY);
    }
    sleep_ms(300);

    // 2. Animação de linha "ping-pong"
    for (int x = 0; x < 128; x += 4) {
        ssd1306_clear(&disp);
        ssd1306_draw_line(&disp, 0, 32, x, 32);
        ssd1306_show(&disp);
        sleep_ms(10);
    }
    for (int x = 128; x >= 0; x -= 4) {
        ssd1306_clear(&disp);
        ssd1306_draw_line(&disp, 127, 32, x, 32);
        ssd1306_show(&disp);
        sleep_ms(10);
    }
    sleep_ms(300);

    // 3. Texto centralizado com efeito de "digitação"
    const char* msg = "Raspberry Pi Pico";
    for (int i = 1; i <= (int)strlen(msg); ++i) {
        ssd1306_clear(&disp);
        ssd1306_draw_string(&disp, 20, 28, 2, msg);
        ssd1306_draw_string(&disp, 20, 48, 1, "SSD1306 OLED TESTE");
        ssd1306_show(&disp);
        sleep_ms(60);
    }
    sleep_ms(700);

    // 4. Quadrado vazio animado nas bordas
    for (int i = 0; i < 32; i += 2) {
        ssd1306_clear(&disp);
        ssd1306_draw_empty_square(&disp, i, i/2, 128-2*i, 64-i);
        ssd1306_show(&disp);
        sleep_ms(25);
    }
    sleep_ms(400);

    // 5. Inversão rápida (efeito flash)
    for (int i = 0; i < 3; ++i) {
        ssd1306_invert(&disp, 1);
        ssd1306_show(&disp);
        sleep_ms(100);
        ssd1306_invert(&disp, 0);
        ssd1306_show(&disp);
        sleep_ms(100);
    }

    // 6. "Estrelas" piscando aleatoriamente
    for (int t = 0; t < 40; ++t) {
        ssd1306_clear(&disp);
        for (int s = 0; s < 20; ++s) {
            int x = rand() % 128;
            int y = rand() % 64;
            ssd1306_draw_pixel(&disp, x, y);
        }
        ssd1306_show(&disp);
        sleep_ms(50);
    }

    // 7. Mensagem final
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 10, 28, 2, "FIM!");
    ssd1306_show(&disp);

    while (1) tight_loop_contents();
}
