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
    if (p == NULL) {                      // Se o pacote de dados recebido for NULL, significa que a conexão foi fechada
        tcp_close(tpcb);                  // Fecha a conexão TCP
        return ERR_OK;                    // Retorna com sucesso
    }

    char *payload = (char *)p->payload;   // Converte o buffer do pacote para uma string
    extract_json(payload);

    pbuf_free(p);                         // Libera o buffer do pacote
    return ERR_OK;                        // Retorna com sucesso
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
    tcp_output(tpcb);                     // Envia os dados pela rede
    tcp_recv(tpcb, http_recv_callback);   // Registra o callback para tratar a resposta

    return ERR_OK;                        // Retorna com sucesso
}

void DNS_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {                          // Se o endereço IP foi resolvido com sucesso
        printf("Endereço IP: %s\n", ipaddr_ntoa(ipaddr)); // Exibe o endereço IP resolvido
        tcp_client_pcb = tcp_new();        // Cria uma nova estrutura de controle de conexão TCP
        tcp_connect(tcp_client_pcb, ipaddr, PORT, http_connected_callback);  // Conecta ao servidor
    } else {
        printf("Falha na resolução de DNS\n"); // Caso a resolução de DNS falhe
    }
}

void parse_json(const char *json_str) {
    // Converte a string JSON recebida em um objeto cJSON
    cJSON *json = cJSON_Parse(json_str); 
    if (json == NULL) {  // Se houver erro ao analisar o JSON, exibe uma mensagem e retorna
        printf("Erro ao analisar JSON!\n");
        return;
    }

    printf("\nsucess");

    cJSON_Delete(json); // Libera a memória alocada para o JSON
}

// Função para extrair o JSON de uma resposta HTTP
void extract_json(char *text) {
    char *json_start = strrchr(text, '{');  // encontra a última ocorrência de '{'
    if (json_start) {
        printf("%s", json_start);
        parse_json(json_start);  // função que você já deve ter para analisar o JSON
    } else {
        printf("JSON não encontrado\n");
    }
}



