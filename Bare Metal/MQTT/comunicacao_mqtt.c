#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "inc_MQTT/MQTT_lib.h"
#include "inc_WIFI/WIFI_lib.h"

#define ADC_TEMPERATURE_CHANNEL 4
const char WIFI_SSID[] = "brisa-4009564";
const char WIFI_PASSWORD[] = "fugd9i9k";

char mqtt_buffer[100];
const char broker_ip[] = "200.137.1.176";
const char user_mqtt[] = "desafio09";
const char pass_mqtt[] = "desafio09.laica";
const char topic[] = "ha/desafio09/lucas.santos/teste1";
const char teste_topic[] = "ha/desafio09/lucas.santos/teste2";


char message[15] = "22";
char received_data[100] = {0}; 

uint64_t last_time_publish = 0;
uint64_t last_time_subscribe = 0;
uint64_t last_time_unsubscribe = 0;

int main()
{
    stdio_init_all();
    wifi_init("brisa-4009564", "fugd9i9k");
    mqtt_setup("Lucas", broker_ip, user_mqtt, pass_mqtt, mqtt_buffer);

    sleep_ms(3000); // Aguarda a conexão Wi-Fi
    mqtt_comm_subscribe(teste_topic);
    mqtt_comm_subscribe(topic);    

    while (true) {

        if(get_absolute_time() - last_time_publish > 500 * 1000) {
        mqtt_comm_publish(teste_topic, (const uint8_t*)message, strlen(message), 0);
            
            last_time_publish = get_absolute_time();
        }

        if(get_absolute_time() - last_time_subscribe > 200 * 1000) {
            received_data_cb(received_data);

            printf("Dados recebidos: %s\n", received_data);

            last_time_subscribe = get_absolute_time();
        }

        
    }
}
