#ifndef LED_RGB_H
#define LED_RGB_H

#include "pico/stdlib.h"

#define LED_R 13 // Definição do pino do LED vermelho
#define LED_G 11 // Definição do pino do LED verde
#define LED_B 12 // Definição do pino do LED azul

void led_rgb_init() {
    gpio_init(LED_R); // Inicializa o LED vermelho
    gpio_init(LED_G); // Inicializa o LED verde
    gpio_init(LED_B); // Inicializa o LED azul
    gpio_set_dir(LED_R, GPIO_OUT); // Define o LED vermelho como saída
    gpio_set_dir(LED_G, GPIO_OUT); // Define o LED verde como saída
    gpio_set_dir(LED_B, GPIO_OUT); // Define o LED azul como saída
    gpio_put(LED_R, 0); // Desliga o LED vermelho
    gpio_put(LED_G, 0); // Desliga o LED verde
    gpio_put(LED_B, 0); // Desliga o LED azul
}

void set_led_color(uint red_pin, uint green_pin,uint blue_pin, bool R, bool G, bool B) {
    gpio_put(red_pin, R);   // Configura o estado do LED vermelho
    gpio_put(green_pin, G); // Configura o estado do LED verde
    gpio_put(blue_pin, B);     // Configura o estado do LED azul
}



#endif