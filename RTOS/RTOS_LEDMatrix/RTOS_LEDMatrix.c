#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_ledMatrix/ledMatrix_lib.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define LED_PIN 7

SemaphoreHandle_t xMutex;

void vtaskFade(void *pvParameters){
    bool direction = true;
    while(true){
        
        if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){
            if(direction){
                for(int i = 0; i <= 255; i++){
                    for(int j = 0; j <= 25; j++){
                        npSetLED(j,i,0,0);
                    }

                    npWrite();
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                direction = !direction;
            }else{
                for(int i = 255; i >= 0; i--){
                    for(int j = 0; j <= 25; j++){
                        npSetLED(j,i,0,0);
                    }
                    
                    npWrite();
                    vTaskDelay(pdMS_TO_TICKS(10));
                }

                direction = !direction;
            }

            xSemaphoreGive(xMutex);
            vTaskDelay(pdMS_TO_TICKS(200));
            
        }
    }
}

void vtaskLeftToRight(){

    while(true){
        if(xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE){    
            
            for(int i = 0; i <= 20; i++){
                npClear();
                for(int j = 0; j < 5; j++){
                    setLedInPosition(i%5, j, 0, 0, 255);
                }
                vTaskDelay(pdMS_TO_TICKS(100));
                npWrite();
            }
           
            xSemaphoreGive(xMutex);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}


int main(){

    stdio_init_all();
    npInit(LED_PIN);
    xMutex = xSemaphoreCreateMutex();

    if(xMutex != NULL){
        xTaskCreate(vtaskFade, "fade Task", 256, NULL, 1, NULL);
        xTaskCreate(vtaskLeftToRight, "move Task", 256, NULL, 1, NULL);
        vTaskStartScheduler();    
    }
    
    
}
