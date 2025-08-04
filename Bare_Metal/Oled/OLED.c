#include <pico/stdlib.h>
#include "inc_OLED/ssd1306.h"

#define DELAY 1000

int main() {
    stdio_init_all();
    ssd1306_t disp;
    ssd1306_init_display(&disp);

    // 1. Teste de pixels
    for (int y = 0; y < 64; y += 8) {
        for (int x = 0; x < 128; x += 8) {
            ssd1306_draw_pixel(&disp, x, y);
        }
    }
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 2. Teste de linhas
    ssd1306_clear(&disp);
    for (int i = 0; i < 64; i += 4) {
        ssd1306_draw_line(&disp, 0, i, 127, 63-i);
    }
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 3. Teste de quadrados preenchidos
    ssd1306_clear(&disp);
    for (int i = 0; i < 30; i += 5) {
        ssd1306_draw_square(&disp, i, i, 128-2*i, 64-2*i);
    }
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 4. Teste de limpeza de áreas
    ssd1306_clear_square(&disp, 30, 15, 68, 34);
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 5. Teste de quadrados vazios
    ssd1306_clear(&disp);
    for (int i = 0; i < 20; i += 4) {
        ssd1306_draw_empty_square(&disp, 10+i, 10+i, 108-2*i, 44-2*i);
    }
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 6. Teste de texto básico
    ssd1306_clear(&disp);
    ssd1306_draw_string(&disp, 10, 10, 1, "Texto Simples");
    ssd1306_draw_char(&disp, 100, 40, 2, 'A');
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 7. Teste de quebra de linha
    ssd1306_clear(&disp);
    ssd1306_draw_string_with_break_line(&disp, 0, 0, 1,
        "Este texto longo demonstra a quebra automatica de linha ao atingir a borda do display");
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 8. Teste de centralização
    ssd1306_clear(&disp);
    ssd1306_draw_string_in_center(&disp, 1, "TEXTO CENTRALIZADO");
    ssd1306_show(&disp);
    sleep_ms(DELAY);

    // 9. Teste de inversão
    ssd1306_clear(&disp);
    ssd1306_draw_string_in_center(&disp, 1, "MODO INVERTIDO");
    ssd1306_invert(&disp, 1);
    ssd1306_show(&disp);
    sleep_ms(DELAY/2);
    ssd1306_invert(&disp, 0);
    ssd1306_show(&disp);
    sleep_ms(DELAY/2);

    // 10. Teste de contraste
    for (int c = 0; c < 255; c += 5) {
        ssd1306_contrast(&disp, c);
        ssd1306_show(&disp);
        sleep_ms(10);
    }
    sleep_ms(DELAY);

    // 11. Teste de energia
    ssd1306_poweroff(&disp);
    sleep_ms(DELAY);
    ssd1306_poweron(&disp);

    // Mensagem final
    ssd1306_clear(&disp);
    ssd1306_draw_string_in_center(&disp, 1, "TESTES CONCLUIDOS!");
    ssd1306_show(&disp);

    while (1) tight_loop_contents();
}
