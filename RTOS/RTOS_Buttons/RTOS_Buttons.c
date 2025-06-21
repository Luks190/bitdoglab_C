#include "pico/stdlib.h"
#include "inc_Buttons/Buttons_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

#define LED_B 12
#define LED_G 11
#define LED_R 13

void read_buttons()
{
    bool last_state_A = false, last_state_B = false;

    for (;;)
    {
        if(readButtonWithDebounce(5, &last_state_A)){
            gpio_put(LED_G, !gpio_get(LED_G)); // Toggle green LED
        }

        if(readButtonWithDebounce(6, &last_state_B)){
            gpio_put(LED_B, !gpio_get(LED_B)); // Toggle blue LED
        }

        if(!gpio_get(22)){
            gpio_put(LED_R, !gpio_get(LED_R)); // Toggle red LED
        }

        vTaskDelay(10);
    }
}

void main()
{
    
    stdio_init_all();
    initButtonWithPullUp(5);
    initButtonWithPullUp(6);
    initButtonWithPullUp(22);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    xTaskCreate(read_buttons, "buttons task", 128, NULL, 1, NULL);

    vTaskStartScheduler();
}