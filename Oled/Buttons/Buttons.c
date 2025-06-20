#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_Buttons/Buttons_lib.h"

#define BTN_A 5
#define BTN_B 6
#define BTN_SW 22

int main() {
    stdio_init_all();

    bool last_state_A = false;
    bool last_state_SW = false;

    initButtonWithPullUp(BTN_A);
    initButtonWithPullUp(BTN_B);
    initButtonWithPullUp(BTN_SW);

    while (true) {

        if (readButtonWithDebounce(BTN_A, &last_state_A)) {
            printf("Botão A pressionado!\n");
        }

        if(!gpio_get(BTN_B)){
            printf("Botão B pressionado!\n");
        }

        if (readButtonWithDebounce(BTN_SW, &last_state_SW)) {
            printf("Botão do Joystick Pressionado!\n");
        }

        sleep_ms(1); 
    }
}
