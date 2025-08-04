#include <stdio.h> 
#include "pico/stdio.h"
#include "pico/stdlib.h" 
#include "lwipopts.h"            
#include "lwip/apps/mqtt.h"   
#include "lwip/dns.h"
#include "MQTT_lib.h"
#include "../inc_WIFI/WIFI_lib.h" 


/* Variável global estática para armazenar a instância do cliente MQTT
 * 'static' limita o escopo deste arquivo */
static mqtt_client_t *client;
bool connected = false;

const char *used_client_id;
const char *used_broker_ip;
const char *used_user_mqtt;
const char *used_pass_mqtt;
const char *used_topic = NULL;
char used_mqtt_buffer[100];

// Variáveis para controle da resolução DNS assíncrona
static bool dns_resolved = false;
static bool dns_error = false;
static ip_addr_t resolved_ip;

// Função para verificar se uma string é um endereço IP válido
bool is_ip_address(const char *str) {
    if (str == NULL) return false;
    
    int dots = 0;
    int digits = 0;
    int num = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
            digits++;
            
            // Número muito grande ou muitos dígitos
            if (num > 255 || digits > 3) {
                return false;
            }
        } else if (str[i] == '.') {
            // Verificações básicas
            if (digits == 0 || dots >= 3) {
                return false;
            }
            dots++;
            num = 0;
            digits = 0;
        } else {
            // Caractere inválido para IP
            return false;
        }
    }
    
    return (dots == 3 && digits > 0 && digits <= 3 && num <= 255);
}


void DNS_cb(const char *name, const ip_addr_t *ipaddr, void *callback_arg) {
    if (ipaddr) {                          
        // Armazena o IP resolvido
        resolved_ip = *ipaddr;
        dns_resolved = true;
        dns_error = false;
    } else {
        dns_error = true;
        dns_resolved = false;
    }
}

char *dns_converter(char *HOST) {
    static char ip_string[16]; // Buffer estático para retornar o IP como string
    
    // Reinicia as flags de controle
    dns_resolved = false;
    dns_error = false;
    
    err_t dns_result = dns_gethostbyname(HOST, &resolved_ip, DNS_cb, NULL);
    while(dns_result == ERR_INPROGRESS) {
        if (dns_result == ERR_OK) {
            // DNS já estava em cache - resolvido imediatamente
            snprintf(ip_string, sizeof(ip_string), "%s", ip4addr_ntoa(&resolved_ip));
            return ip_string;

        } else if (dns_result == ERR_INPROGRESS) {
            // Resolução DNS em progresso - espera o callback
            
            // Espera até 5 segundos pela resolução
            uint64_t start_time = get_absolute_time();
            while (!dns_resolved && !dns_error && (get_absolute_time() - start_time) < 500000) {
                ;
            }
            
            if (dns_resolved) {
                snprintf(ip_string, sizeof(ip_string), "%s", ip4addr_ntoa(&resolved_ip));
                
                return ip_string;
            } else if (dns_error) {
                
                return NULL;
            } else {
                
                return NULL;
            }
        } else {
            
            return NULL;
        }
    }
}

// Callback de status de conexão MQTT
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    bool *connected = (bool*)arg;
    if (status == MQTT_CONNECT_ACCEPTED) {
        *connected = true;
    } else {
        *connected = false;
    }
}

/* Função para verificar se o cliente MQTT está conectado
 * Retorna:
 *   - true se conectado, false caso contrário */
bool mqtt_is_connected(){
    if (mqtt_client_is_connected(client)) return true;
    else return false;
}

// Callback de recebimento de dados MQTT
void mqtt_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    char *rec_data = (char *)arg;

    sniprintf(rec_data, 100, "%.*s", len, data);
}


/* Função para configurar e iniciar a conexão MQTT
 * Parâmetros:
 *   - client_id: identificador único para este cliente
 *   - broker_ip: endereço IP do broker como string (ex: "192.168.1.1")
 *   - user: nome de usuário para autenticação (pode ser NULL)
 *   - pass: senha para autenticação (pode ser NULL) */
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass, char *text_buffer) {

    used_client_id = client_id;
    used_broker_ip = broker_ip;
    used_user_mqtt = user;
    used_pass_mqtt = pass;

    // if (client != NULL && mqtt_client_is_connected(client)) {
    //     return;
    // }
    ip_addr_t broker_addr;  // Estrutura para armazenar o IP do broker
    sniprintf(text_buffer, 100,"Conectando ao broker\n");
    
    // Verifica se é IP ou DNS
    if (is_ip_address(broker_ip)) {
        // É um endereço IP - converte diretamente
        if (!ip4addr_aton(broker_ip, &broker_addr)) {
            sniprintf(text_buffer, 100, "Erro no IP\n");
            return;
        }
    } else {
        // É um DNS - tenta resolver
        char *resolved_ip = dns_converter((char *)broker_ip);
        if (resolved_ip == NULL) {
            sniprintf(text_buffer, 100, "Erro na resolução DNS\n");
            return;
        }
        // Converte o IP resolvido
        if (!ip4addr_aton(resolved_ip, &broker_addr)) {
            sniprintf(text_buffer, 100, "Erro no IP resolvido\n");
            return;
        }
    }

    if (client != NULL) {
        mqtt_disconnect(client);
        mqtt_client_free(client);
        client = NULL;
    }

    // Cria uma nova instância do cliente MQTT
    client = mqtt_client_new();
    if (client == NULL) {
        sniprintf(text_buffer, 100,"Falha no cliente\n");
        return;
    }


    // Configura as informações de conexão do cliente
    struct mqtt_connect_client_info_t ci = {
        .client_id = client_id,  // ID do cliente
        .client_user = user,     // Usuário (opcional)
        .client_pass = pass      // Senha (opcional)
    };

    // Inicia a conexão com o broker
    // Parâmetros:
    //   - client: instância do cliente
    //   - &broker_addr: endereço do broker
    //   - 1883: porta padrão MQTT
    //   - mqtt_connection_cb: callback de status
    //   - &conected: argumento para o callback
    //   - &ci: informações de conexão
    mqtt_client_connect(client, &broker_addr, 1883, mqtt_connection_cb, &connected, &ci);
}


void mqtt_unsub_request_cb(void *arg, err_t result) {
    // Callback silencioso - sem printf
}

// Callback de inscrição no tópico
void mqtt_sub_request_cb(void *arg, err_t result) {
    // Callback silencioso - sem printf
}

/* Callback de confirmação de publicação
 * Chamado quando o broker confirma recebimento da mensagem (para QoS > 0)
 * Parâmetros:
 *   - arg: argumento opcional
 *   - result: código de resultado da operação */
static void mqtt_pub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        // Publicação bem-sucedida - sem printf
        
    } else {
        // Falha na publicação - sem printf
    }
}

/* Função para publicar dados em um tópico MQTT
 * Parâmetros:
 *   - topic: nome do tópico (ex: "sensor/temperatura")
 *   - data: payload da mensagem (bytes)
 *   - len: tamanho do payload */
void mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len, int retain) {

    while(!wifi_is_connected()){
        wifi_restart();
    }

    while(!mqtt_is_connected()) {

        mqtt_setup(used_client_id, used_broker_ip, used_user_mqtt, used_pass_mqtt, used_mqtt_buffer);

        uint64_t last_time = get_absolute_time();
        while(get_absolute_time() - last_time < 1000000) {
            ;
        }    
    }

    // Envia a mensagem MQTT
    err_t status = mqtt_publish(
        client,              // Instância do cliente
        topic,               // Tópico de publicação
        data,                // Dados a serem enviados
        len,                 // Tamanho dos dados
        0,                   // QoS 0 (nenhuma confirmação)
        retain,              // 0 Não reter mensagem, 1 Reter mensagem
        mqtt_pub_request_cb, // Callback de confirmação
        NULL                 // Argumento para o callback
    );
}

/* Função para cancelar a inscrição de um tópico MQTT
 * Parâmetros:
 *   - topico: nome do tópico a ser cancelado */
void mqtt_comm_unsubscribe(const char *topico) {
    
    err_t status = mqtt_sub_unsub(
        client,              // Instância do cliente
        topico,             // Tópico a ser inscrito
        0,                   // QoS 0 (sem confirmação)
        mqtt_unsub_request_cb, // Callback de inscrição
        NULL,                // Argumento para o callback
        0                    // 1 para inscrição, 0 para cancelamento
    );
}

/* Função para inscrever em um tópico MQTT
 * Parâmetros:
 *   - client: instância do cliente MQTT
 *   - topico: nome do tópico a ser inscrito */
void mqtt_comm_subscribe(const char *topico) {

    while(!wifi_is_connected()){
        wifi_restart();
    }

    while(!mqtt_is_connected()) {
        mqtt_setup(used_client_id, used_broker_ip, used_user_mqtt, used_pass_mqtt, used_mqtt_buffer);

        uint64_t last_time = get_absolute_time();
        while(get_absolute_time() - last_time < 1000000) {
            ;
        }    
    }

    if(used_topic != NULL){
        mqtt_comm_unsubscribe(used_topic);
        used_topic = topico;
    }else {
        used_topic = topico;
    }
    

    err_t status = mqtt_sub_unsub(
        client,              // Instância do cliente
        topico,             // Tópico a ser inscrito
        0,                   // QoS 0 (sem confirmação)
        mqtt_sub_request_cb, // Callback de inscrição
        NULL,                // Argumento para o callback
        1                    // 1 para inscrição, 0 para cancelamento
    );
}

void received_data_cb(void *rec_data) {
    
    while(!wifi_is_connected()){
        wifi_restart();
    }

    while(!mqtt_is_connected()) {
        mqtt_setup(used_client_id, used_broker_ip, used_user_mqtt, used_pass_mqtt, used_mqtt_buffer);

        uint64_t last_time = get_absolute_time();
        while(get_absolute_time() - last_time < 1000000) {
            ;
        }    
    }

    mqtt_set_inpub_callback(client, NULL, mqtt_data_cb, rec_data);
}