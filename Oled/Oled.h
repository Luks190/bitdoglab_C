#ifndef _inc_Oled
#define _inc_Oled

#include <stdio.h>
#include "ssd1306.h"

// Inicializa o display SSD1306 via I2C
void ssd1306_init_display(ssd1306_t *disp) {
    i2c_init(i2c1, 400000);  // Inicializa a comunicação I2C com 400kHz
    gpio_set_function(14, GPIO_FUNC_I2C);  // Configura o pino 14 como I2C
    gpio_set_function(15, GPIO_FUNC_I2C);  // Configura o pino 15 como I2C
    gpio_pull_up(14);  // Ativa pull-up no pino 14
    gpio_pull_up(15);  // Ativa pull-up no pino 15

    disp->external_vcc = false;  // Define que o display usa alimentação interna
    ssd1306_init(disp, 128, 64, 0x3C, i2c1);  // Inicializa o display SSD1306
    ssd1306_clear(disp);  // Limpa o display
    ssd1306_show(disp);  // Atualiza o display
}

#endif