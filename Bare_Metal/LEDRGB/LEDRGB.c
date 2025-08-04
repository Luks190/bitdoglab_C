#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "inc_LEDRGB/LED_RGB_lib.h"

#define DELAY 1000
#define PWM_MAX 255

int main() {
    stdio_init_all();

    // Teste GPIO ON/OFF
    led_rgb_init();
    set_led_color(1,0,0); sleep_ms(DELAY); // Vermelho
    set_led_color(0,1,0); sleep_ms(DELAY); // Verde
    set_led_color(0,0,1); sleep_ms(DELAY); // Azul
    set_led_color(1,1,1); sleep_ms(DELAY); // Branco
    set_led_color(0,0,0); sleep_ms(DELAY); // Desliga

    // Teste PWM RGB puro
    led_rgb_pwm_init();
    set_led_pwm_color(PWM_MAX,0,0); sleep_ms(DELAY); // PWM Vermelho
    set_led_pwm_color(0,PWM_MAX,0); sleep_ms(DELAY); // PWM Verde
    set_led_pwm_color(0,0,PWM_MAX); sleep_ms(DELAY); // PWM Azul
    set_led_pwm_color(PWM_MAX,PWM_MAX,PWM_MAX); sleep_ms(DELAY); // PWM Branco
    set_led_pwm_color(0,0,0); sleep_ms(DELAY); // Desliga

    // Teste PWM com hexadecimal (cores conhecidas)
    uint32_t cores[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFF00, 0x00FFFF, 0xFF00FF, 0xFFFFFF, 0x000000};
    
    for(int i=0;i<sizeof(cores)/sizeof(cores[0]);i++) {
        set_led_pwm_color_hexa(cores[i]);
        sleep_ms(DELAY);
    }

    // Teste PWM: Fade de vermelho
    for(int v = 0; v <= PWM_MAX; v+=5) {
        set_led_pwm_color(v,0,0);
        sleep_ms(10);
    }
    for(int v = PWM_MAX; v >= 0; v-=5) {
        set_led_pwm_color(v,0,0);
        sleep_ms(10);
    }

    // Loop infinito desligando
    set_led_pwm_color(0,0,0);

    while (true) {
        break;
    }
}
