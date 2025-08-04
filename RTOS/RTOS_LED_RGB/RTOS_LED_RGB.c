#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_LEDRGB/LED_RGB_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "hardware/pwm.h"

void vfadeLED_task(void *params) {
    bool m_R = true, m_G = true, m_B = true;
    int cont_R = 0, cont_G = 0, cont_B = 0;

    while (1) {
        // Verificação de limites e inversão das direções
        if (cont_R >= 255) m_R = false;
        else if (cont_R <= 0) m_R = true;

        if (cont_G >= 255) m_G = false;
        else if (cont_G <= 0) m_G = true;

        if (cont_B >= 255) m_B = false;
        else if (cont_B <= 0) m_B = true;

        // Atualiza os valores RGB conforme a direção
        cont_R += (m_R ? 1 : -1);
        cont_G += (m_G ? 2 : -2);
        cont_B += (m_B ? 4 : -4);

        // Define a nova cor no LED RGB
        set_led_pwm_color(cont_R, cont_G, cont_B);

        // Delay da tarefa
        vTaskDelay(20);
    }
}

int main() {
    stdio_init_all();
    led_rgb_pwm_init();  // Inicializa a sua biblioteca LED

    // Criação da tarefa que faz o LED piscar com fade
    xTaskCreate(vfadeLED_task, "FadeRGB", 256, NULL, 1, NULL);

    // Inicia o agendador do FreeRTOS
    vTaskStartScheduler();

    while (true);  // Nunca deve chegar aqui
}
