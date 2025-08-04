#include <stdio.h>                      // Biblioteca para operações de entrada e saída
#include <string.h>                     // Para strlen
#include <stdlib.h>                     // Para atoi
#include "pico/cyw43_arch.h"             // Biblioteca para a arquitetura CYW43XX
#include "lwip/pbuf.h"                  
#include "lwip/tcp.h"                   
#include "lwip/dns.h"                   
#include "lwip/init.h" 
#include "lwip/err.h"                   // Para err_t
#include "lwip/netif.h"                 // Para netif functions
#include "../inc_WIFI/WIFI_lib.h"
#include "inc_HTTP/HTTP_lib.h"            // Biblioteca para operações HTTP

char used_HOST[100] = ""; // Host para requisições HTTP
char used_PORT[5] = "";  // Porta padrão para HTTP
char used_PATH[100] = ""; // Caminho padrão para requisições HTTP
char post_data[512] = ""; // Dados para enviar no POST
size_t buffer_size = 0;   // Tamanho do buffer disponível
char temp_response[2048] = {0}; // Buffer temporário para acumular a resposta completa
size_t response_length = 0;     // Tamanho atual da resposta acumulada

struct tcp_pcb *tcp_client_pcb;
char *buffer_GET = NULL;


static err_t http_connected_get_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t http_recv_GET_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t http_connected_get_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t http_connected_post_callback(void *arg, struct tcp_pcb *tpcb, err_t err);
static err_t http_recv_POST_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static void DNS_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg);


void HTTP_GET(const char *HOST, const char *PATH, const char *PORT, ip_addr_t *server_ip, char *data_buffer, size_t buffer_len) {
    // Copia os parâmetros para as variáveis globais
    snprintf(used_HOST, sizeof(used_HOST), "%s", HOST);
    snprintf(used_PORT, sizeof(used_PORT), "%s", PORT);
    snprintf(used_PATH, sizeof(used_PATH), "%s", PATH);
    buffer_GET = data_buffer; // Armazena o buffer de dados recebido
    buffer_size = buffer_len; // Armazena o tamanho do buffer
    
    // Limpa os buffers
    memset(temp_response, 0, sizeof(temp_response));
    response_length = 0;

    
    printf("Iniciando resolução DNS para: %s\n", HOST);
    printf("Porta: %s\n", PORT);

    err_t dns_result = dns_gethostbyname(HOST, server_ip, DNS_found_callback, (void *)1);

    switch(dns_result) {
        case ERR_OK:
            printf("DNS resolvido imediatamente (cache)\n");
            printf("IP: %s\n", ipaddr_ntoa(server_ip));

            break;
        case ERR_INPROGRESS:
            printf("DNS em progresso... aguardando callback\n");
            break;
        default:
            printf("Erro ao iniciar resolução DNS: %d\n", dns_result);
            break;
    }
}

void HTTP_POST(const char *HOST, const char *PATH, const char *PORT, const char *json_data, ip_addr_t *server_ip, char *data_buffer, size_t buffer_len) {
    // Copia os parâmetros para as variáveis globais
    snprintf(used_HOST, sizeof(used_HOST), "%s", HOST);
    snprintf(used_PORT, sizeof(used_PORT), "%s", PORT);
    snprintf(used_PATH, sizeof(used_PATH), "%s", PATH);
    snprintf(post_data, sizeof(post_data), "%s", json_data);
    buffer_GET = data_buffer; // Armazena o buffer de dados recebido
    buffer_size = buffer_len; // Armazena o tamanho do buffer
    
    // Limpa os buffers
    memset(temp_response, 0, sizeof(temp_response));
    response_length = 0;
    
    printf("Dados: %s\n", json_data);

    err_t dns_result = dns_gethostbyname(HOST, server_ip, DNS_found_callback, (void *)2); // POST = 2

    switch(dns_result) {
        case ERR_OK:
            printf("DNS resolvido imediatamente (cache)\n");
            printf("IP: %s\n", ipaddr_ntoa(server_ip));
            break;
        case ERR_INPROGRESS:
            printf("DNS em progresso... aguardando callback\n");
            break;
        default:
            printf("Erro ao iniciar resolução DNS: %d\n", dns_result);
            break;
    }
}

void DNS_found_callback(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    int requisition = (uint32_t)callback_arg; 
    err_t err;

    if (ipaddr) {                          
        printf("DNS resolvido com sucesso!\n");
        printf("Host: %s\n", name);
        printf("Endereço IP: %s\n", ipaddr_ntoa(ipaddr)); 

        tcp_client_pcb = tcp_new();        
        if (tcp_client_pcb == NULL) {
            printf("Erro: Falha ao criar PCB TCP\n");
            return;
        }

        if(requisition == 1) {
            printf("Conectando para GET...\n");
            err = tcp_connect(tcp_client_pcb, ipaddr, (u16_t)atoi(used_PORT), http_connected_get_callback);
        } else if(requisition == 2) {
            printf("Conectando para POST...\n");
            err = tcp_connect(tcp_client_pcb, ipaddr, (u16_t)atoi(used_PORT), http_connected_post_callback);
        } else {
            printf("Tipo de requisição desconhecido: %d\n", requisition);
            err = tcp_connect(tcp_client_pcb, ipaddr, (u16_t)atoi(used_PORT), NULL);
        }
        
        if (err != ERR_OK) {
            printf("Erro ao conectar TCP: %d\n", err);
            tcp_close(tcp_client_pcb);
        }

    } else {
        printf("Falha na resolução de DNS para: %s\n", name); 
    }
}

static err_t http_connected_get_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {                  
        printf("Erro na conexão TCP\n");  
        return err;                       
    }

    char request[256];
    snprintf(request, sizeof(request),
        "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        used_PATH, used_HOST);

    
    
    // Envia a requisição HTTP para o servidor
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);                     
    tcp_recv(tpcb, http_recv_GET_callback);   

    return ERR_OK;                        
}


static err_t http_connected_post_callback(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {                  
        printf("Erro na conexão TCP\n");  
        return err;                       
    }

    // Calcula o tamanho dos dados
    size_t content_length = strlen(post_data);
    
    char request[512];
    snprintf(request, sizeof(request),
        "POST %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        used_PATH, used_HOST, content_length, post_data);

    
    
    // Envia a requisição HTTP POST para o servidor
    tcp_write(tpcb, request, strlen(request), TCP_WRITE_FLAG_COPY);
    tcp_output(tpcb);                     
    tcp_recv(tpcb, http_recv_POST_callback);   

    return ERR_OK;                        
}


static err_t http_recv_GET_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {                      
        tcp_close(tpcb);                  
        printf("Conexão TCP fechada pelo servidor\n");
        return ERR_OK;                    
    }

    char *payload = (char *)p->payload;   
    
    // Acumula os dados recebidos no buffer temporário
    if (response_length + p->len < sizeof(temp_response)) {
        memcpy(temp_response + response_length, payload, p->len);
        response_length += p->len;
        temp_response[response_length] = '\0';
        
        printf("Recebido pacote de %d bytes (total: %d bytes)\n", p->len, response_length);
    } else {
        printf("Erro: Buffer temporário muito pequeno!\n");
    }
    
    // Procura pelo final dos headers HTTP (linha vazia: \r\n\r\n)
    char *json_start = strstr(temp_response, "\r\n\r\n");
    
    if (json_start != NULL) {
        // Pula os 4 caracteres \r\n\r\n para chegar ao corpo
        json_start += 4;
        
        // Calcula o tamanho do JSON
        size_t json_length = strlen(json_start);
        
        // Copia apenas o JSON para o buffer do usuário
        if (buffer_GET != NULL && json_length < buffer_size) {
            strncpy(buffer_GET, json_start, json_length);
            buffer_GET[json_length] = '\0';
            
        } else {
            printf("Erro: Buffer muito pequeno para o JSON (necessário: %d, disponível: %d)\n", 
                   json_length, buffer_size);
        }
    }

    pbuf_free(p);                         
    return ERR_OK;                        
}

static err_t http_recv_POST_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    if (p == NULL) {                      
        tcp_close(tpcb);                  
        printf("Conexão TCP fechada pelo servidor (POST)\n");
        return ERR_OK;                    
    }

    char *payload = (char *)p->payload;   
    
    // Acumula os dados recebidos no buffer temporário
    if (response_length + p->len < sizeof(temp_response)) {
        memcpy(temp_response + response_length, payload, p->len);
        response_length += p->len;
        temp_response[response_length] = '\0';
        
        printf("POST - Recebido pacote de %d bytes (total: %d bytes)\n", p->len, response_length);
    } else {
        printf("Erro: Buffer temporário muito pequeno!\n");
    }
    
    // Procura pelo final dos headers HTTP (linha vazia: \r\n\r\n)
    char *response_start = strstr(temp_response, "\r\n\r\n");
    
    if (response_start != NULL) {
        // Pula os 4 caracteres \r\n\r\n para chegar ao corpo da resposta
        response_start += 4;
        
        // Calcula o tamanho da resposta
        size_t response_len = strlen(response_start);
        
        // Copia a resposta para o buffer do usuário
        if (buffer_GET != NULL && response_len < buffer_size) {
            strncpy(buffer_GET, response_start, response_len);
            buffer_GET[response_len] = '\0';
            
        } else {
            printf("Erro: Buffer muito pequeno para a resposta (necessário: %d, disponível: %d)\n", 
                   response_len, buffer_size);
        }
    }

    pbuf_free(p);                         
    return ERR_OK;                        
}