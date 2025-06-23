#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include "inc_OLED/ssd1306.h"


#define DELAY 40

int main() {
    stdio_init_all();
    ssd1306_t disp;
    ssd1306_init_display(&disp);




    
    // 7. Mensagem final
    ssd1306_clear(&disp);
    ssd1306_draw_string_in_center(&disp, 1, "talvezasdadas");
    ssd1306_show(&disp);


    while (1) tight_loop_contents();
}

