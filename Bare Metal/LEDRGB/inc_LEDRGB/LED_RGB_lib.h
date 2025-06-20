#ifndef LED_RGB_H
#define LED_RGB_H

void led_rgb_init(); // Função para inicializar os LEDs RGB

void led_rgb_pwm_init();

void set_led_color(bool R, bool G, bool B); // Função para definir a 

void set_led_pwm_color(int R, int G, int B);

void set_led_pwm_color_hexa(uint32_t color); // Função para definir a cor do LED usando um valor hexadecimal

#endif