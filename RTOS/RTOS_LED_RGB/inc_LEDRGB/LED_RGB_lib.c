
#include "pico/stdlib.h"
#include "LED_RGB_lib.h"
#include "hardware/pwm.h"

#define LED_G 11 // Definição do pino do LED verde
#define LED_B 12 // Definição do pino do LED azul
#define LED_R 13 // Definição do pino do LED vermelho

const uint16_t PERIOD = 255;   // Período do PWM (valor máximo do contador)
const float DIVIDER_PWM = 16.0; // Divisor fracional do clock para o PWM
const uint16_t LED_STEP = 100;  // Passo de incremento/decremento para o duty cycle do LED


void setup_pwm(uint LED)
{
    uint slice;
    gpio_set_function(LED, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    slice = pwm_gpio_to_slice_num(LED);    // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(slice, DIVIDER_PWM);    // Define o divisor de clock do PWM
    pwm_set_wrap(slice, PERIOD);           // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(LED, 0);    // Define o nível inicial do PWM para o pino do LED
    pwm_set_enabled(slice, true);          // Habilita o PWM no slice correspondente
}


void led_rgb_pwm_init() {
    setup_pwm(LED_R); // Configura o PWM para o LED vermelho
    setup_pwm(LED_G); // Configura o PWM para o LED verde
    setup_pwm(LED_B); // Configura o PWM para o LED azul

    pwm_set_gpio_level(LED_R, 0); // Define o nível inicial do PWM para o LED vermelho
    pwm_set_gpio_level(LED_G, 0); // Define o nível inicial do PWM para o LED verde
    pwm_set_gpio_level(LED_B, 0); // Define o nível inicial do PWM para o LED azul
}

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

void set_led_color(bool R, bool G, bool B) {
    gpio_put(LED_R, R);   // Configura o estado do LED vermelho
    gpio_put(LED_G, G); // Configura o estado do LED verde
    gpio_put(LED_B, B);     // Configura o estado do LED azul
}

void set_led_pwm_color(int R, int G, int B) {
    pwm_set_gpio_level(LED_R, R); // Define o nível do PWM para o LED vermelho
    pwm_set_gpio_level(LED_G, G); // Define o nível do PWM para o LED verde
    pwm_set_gpio_level(LED_B, B); // Define o nível do PWM para o LED azul
}

void set_led_pwm_color_hexa(uint32_t color) {
    // Extrai os componentes de cor do valor hexadecimal
    uint8_t r = (color >> 16) & 0xFF; // Extrai o componente vermelho
    uint8_t g = (color >> 8) & 0xFF;  // Extrai o componente verde
    uint8_t b = color & 0xFF;         // Extrai o componente azul

    set_led_pwm_color(r, g, b); // Define a cor do LED usando PWM
}