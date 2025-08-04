#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "inc_WIFI/WIFI_lib.h"
#include "inc_MQTT/MQTT_lib.h"
#include "FreeRTOS.h"
#include "task.h"

char mqtt_buffer[100];

void vtaskPublish(){
    for(;;){
        mqtt_comm_publish("ha/desafio09/lucas.santos/teste", (uint8_t *)"Mensagem de teste", strlen("Mensagem de teste"), 0);
        vTaskDelay(pdMS_TO_TICKS(200)); // Delay de 5 segundos entre publicações
    }
}

void vtaskSubscribe(){
    mqtt_comm_subscribe("ha/desafio09/lucas.santos/temp");
    char subs_buffer[20] = {0}; // Limpa o buffer de recebimento
    for(;;){
        received_data_cb(subs_buffer);
        printf("dado recebido: %s\n", subs_buffer);

        vTaskDelay(pdMS_TO_TICKS(200)); // Delay de 100 ms entre verificações
    }
}

void main(void){
    stdio_init_all();
    sleep_ms(3000);
    
    wifi_init("brisa-4009564", "fugd9i9k");
    mqtt_setup("lucas", "mqtt.iot.natal.br", "desafio09", "desafio09.laica", mqtt_buffer);

    xTaskCreate(vtaskPublish, "PublishTask", 256, NULL, 1, NULL);
    xTaskCreate(vtaskSubscribe, "SubscribeTask", 256, NULL, 1, NULL);

    vTaskStartScheduler(); // Inicia o agendador do FreeRTOS
}
