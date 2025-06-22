#ifndef INC_WIFI_LIB
#define INC_WIFI_LIB

#include "lwip/dns.h" 

void wifi_init(char *WIFI_SSID, char *WIFI_PASS);

void DNS_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

#endif