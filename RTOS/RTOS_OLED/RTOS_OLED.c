#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_OLED/ssd1306.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"
#include "task.h"

ssd1306_t disp;

// Declare the semaphore handle globally
SemaphoreHandle_t xSemaphoreOLED;
QueueHandle_t xQueueCounter;

void vtaskShowInOLED(void *pvParameters) {

    for(;;){

        int received_count = 0;
        if(xSemaphoreTake(xSemaphoreOLED, portMAX_DELAY) == pdTRUE) {
            ssd1306_clear(&disp);
            if(xQueueReceive(xQueueCounter, &received_count, 0) == pdTRUE){
                show_number(&disp, received_count);
            }
            ssd1306_show(&disp);
        }
    }
}

void vtaskCounter(void *pvParameters) {
    int counter = 0;
    bool direction = true;
    for(;;){
        if(direction) {
            counter++;
            if(counter >= 100) {
                direction = false;
            }
        } else {
            counter--;
            if(counter <= 0) {
                direction = true;
            }
        }

        if(xQueueSend(xQueueCounter, &counter, portMAX_DELAY) != pdPASS) {
            printf("Failed to send counter value to queue\n");
        }

        xSemaphoreGive(xSemaphoreOLED);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main()
{
    stdio_init_all();
    ssd1306_init_display(&disp);

    xQueueCounter = xQueueCreate(5, sizeof(int));
    xSemaphoreOLED = xSemaphoreCreateBinary();
    if(xSemaphoreOLED != NULL && xQueueCounter != NULL) {
        
        xTaskCreate(vtaskShowInOLED, "showOLED", 256, NULL, 1, NULL);
        xTaskCreate(vtaskCounter, "counter", 128, NULL, 1, NULL);

        vTaskStartScheduler(); 
    }    

}
