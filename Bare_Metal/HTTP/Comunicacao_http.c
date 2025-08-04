#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "inc_WIFI/WIFI_lib.h"
#include "inc_HTTP/HTTP_lib.h"


const char *WIFI_SSID = "brisa-4009564";  
const char *WIFI_PASS = "fugd9i9k"; 

char data_get_buffer[2048] = {0}; 
char data_post_buffer[2048] = {0}; // Buffer para armazenar dados recebidos do POST
ip_addr_t server_ip;

int main()
{
    stdio_init_all();
    wifi_init(WIFI_SSID, WIFI_PASS);

    // Inicia a comunicação HTTP
    memset(data_get_buffer, 0, sizeof(data_get_buffer));
    HTTP_GET("jsonplaceholder.typicode.com", "/todos/3", "80", &server_ip, data_get_buffer, sizeof(data_get_buffer));
    printf("Waiting for DNS resolution...\n\n\n");
    
    uint64_t time_wait = get_absolute_time();
    
    while(data_get_buffer[0] == 0) { // Wait for 1 second
        // Processa eventos da pilha TCP/IP (necessário para DNS assíncrono)
        cyw43_arch_poll();
        
    }
    
    printf("\n=== DADOS NO BUFFER GET ===\n");
    printf("%s\n", data_get_buffer);
    printf("=== FIM DOS DADOS ===\n");

    memset(data_post_buffer, 0, sizeof(data_post_buffer));
    HTTP_POST("api.restful-api.dev", "/objects", "80", "{\"name\": \"Apple MacBook Pro 16\","
   "\"data\": {"
      "\"year\": 2019,"
      "\"price\": 1849,"
      "\"CPU model\": \"Intel Core i9\","
      "\"Hard disk size\": \"2 TB\""
   "}"
   "}", &server_ip, data_post_buffer, sizeof(data_post_buffer));

    while(data_post_buffer[0] == 0) { // Wait for 1 second
        // Processa eventos da pilha TCP/IP (necessário para DNS assíncrono)
        cyw43_arch_poll();
    }
    printf("\n=== DADOS NO BUFFER POST ===\n");
    printf("%s\n", data_post_buffer);
    printf("=== FIM DOS DADOS ===\n");


    while (true) {
        // Processa eventos da pilha TCP/IP (necessário para DNS assíncrono)
        
        sleep_ms(10);
        
       
    }
}
