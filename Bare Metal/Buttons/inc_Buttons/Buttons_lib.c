#include "pico/stdlib.h"
#include "inc_Buttons/Buttons_lib.h"

void initButton(int gpio) {
    gpio_init(gpio); 
    gpio_set_dir(gpio, GPIO_IN);
}

void initButtonWithPullUp(int gpio) {
    gpio_init(gpio); 
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio); 
}

bool readButtonWithDebounce(uint gpio, bool *last_state) {

    if(!gpio_get(gpio)){
         
        if(!(*last_state)){ 
            *last_state = true;
            sleep_ms(1); 
            return true;
        }

    }else{
        *last_state = false;
    }
    return false;
    
}