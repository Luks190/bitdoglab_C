#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_WIFI/WIFI_lib.h"

#define WIFI_SSID "SSID"               
#define WIFI_PASS "PASS"
              
#define HOST "jsonplaceholder.typicode.com"

ip_addr_t server_ip; 

int main()
{
    stdio_init_all();
    wifi_init(WIFI_SSID, WIFI_PASS);
    sleep_ms(10000);

    dns_gethostbyname(HOST, &server_ip, DNS_found_callback, NULL); 
    
    while (true) {
        sleep_ms(1000);
    }
}
