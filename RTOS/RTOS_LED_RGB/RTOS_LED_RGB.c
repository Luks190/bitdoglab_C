#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_LEDRGB/LED_RGB_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h"

void fadeLED(){
    bool m_R = 1, m_G = 1, m_B = 1;
    int cont_R = 0, cont_G = 0, cont_B = 0;

    while(true){
        if(cont_R > 255 || cont_R < 0){
            m_R = !m_R;
        }

        if(cont_G > 255 || cont_G < 0){
            m_G = !m_G;
        }

        if(cont_B > 255 || cont_B < 0){
            m_G = !m_G;
        }

        if(m_R){
            set_led_pwm_color(cont_R++, cont_G, cont_B);
        }else{
            set_led_pwm_color(cont_R--, cont_G, cont_B);
        }

        if(m_G){
            cont_G = cont_G + 10;
            set_led_pwm_color(cont_R, cont_G, cont_B);
        }else{
            cont_G = cont_G - 10;
            set_led_pwm_color(cont_R, cont_G, cont_B);
        }

        if(m_B){
            cont_B = cont_B + 30;
            set_led_pwm_color(cont_R, cont_G, cont_B);
        }else{
            cont_B = cont_B - 30;
            set_led_pwm_color(cont_R, cont_G, cont_B);
        }

        vTaskDelay(100);
    }
}


int main()
{
    stdio_init_all();
    led_rgb_pwm_init();

    xTaskCreate(fadeLED, "fade LEDs", 128, NULL, 1, NULL);

    vTaskStartScheduler();
    
}
