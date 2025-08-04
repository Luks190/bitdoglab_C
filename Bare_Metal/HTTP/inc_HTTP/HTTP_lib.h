#ifndef INC_HTTP_LIB
#define INC_HTTP_LIB

#include "lwip/dns.h" 

void HTTP_GET(const char *HOST, const char *PATH, const char *PORT, ip_addr_t *server_ip, char *data_buffer, size_t buffer_len);

void HTTP_POST(const char *HOST, const char *PATH, const char *PORT, const char *json_data, ip_addr_t *server_ip, char *data_buffer, size_t buffer_len);

#endif