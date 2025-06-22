#include <stdio.h>                      // Biblioteca para operações de entrada e saída
#include "pico/cyw43_arch.h"             // Biblioteca para a arquitetura CYW43XX
#include "lwip/pbuf.h"                  
#include "lwip/tcp.h"                   
#include "lwip/dns.h"                   
#include "lwip/init.h"      
#include "../cJSON/cJSON.h"            
#include "WIFI_lib.h"

#define HOST "jsonplaceholder.typicode.com"
#define PORT 80 

struct tcp_pcb *tcp_client_pcb;


void extract_json(char *text);

// Função para inicializar a conexão Wi-Fi
void wifi_init(char *WIFI_SSID, char *WIFI_PASS) {

    if (cyw43_arch_init()) {               
        printf("Falha ao iniciar Wi-Fi\n");  
        while (true) {}  
    }

    cyw43_arch_enable_sta_mode();          

    // Tenta conectar à rede Wi-Fi especificada
    while (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK)) { 
        printf("falha o conectar o WI-FI\n");
    }   
} 


static err_t http_recv_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {                      
        tcp_close(tpcb);                  
        return ERR_OK;                    
    }

    char *payload = (char *)p->payload;   
    extract_json(payload);

    pbuf_free(p);                         
    return ERR_OK;                        
}


static err_t http_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {                  
        printf("Erro na conexão TCP\n");  
        return err;                       
    }

    char request[256];
    snprintf(request, sizeof(request),
        "GET /todos/1 HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        HOST);

    
    
    // Envia a requisição HTTP para o servidor
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);                     
    tcp_recv(tpcb, http_recv_callback);   

    return ERR_OK;                        
}

void DNS_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {                          
        printf("Endereço IP: %s\n", ipaddr_ntoa(ipaddr)); 
        tcp_client_pcb = tcp_new();        
        tcp_connect(tcp_client_pcb, ipaddr, PORT, http_connected_callback);  
    } else {
        printf("Falha na resolução de DNS\n"); 
    }
}

void parse_json(const char *json_str) {
    
    cJSON *json = cJSON_Parse(json_str); 
    if (json == NULL) {  
        printf("Erro ao analisar JSON!\n");
        return;
    }

    printf("\nsucess");

    cJSON_Delete(json); 
}


void extract_json(char *text) {
    char *json_start = strrchr(text, '{');  
    if (json_start) {
        printf("%s", json_start);
        parse_json(json_start);  
    } else {
        printf("JSON não encontrado\n");
    }
}



