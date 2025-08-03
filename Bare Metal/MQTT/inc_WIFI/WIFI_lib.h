#ifndef INC_WIFI_LIB
#define INC_WIFI_LIB

#include "lwip/dns.h" 

void wifi_init(const char *WIFI_SSID, const char *WIFI_PASS);

bool wifi_is_connected();

void wifi_restart();



#endif