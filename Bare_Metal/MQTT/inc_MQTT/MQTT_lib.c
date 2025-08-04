#include <stdio.h> 
#include "pico/stdio.h"
#include "pico/stdlib.h" 
#include "lwipopts.h"            
#include "lwip/apps/mqtt.h"   
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

// Callback de status de conexão MQTT
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    bool *connected = (bool*)arg;
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("MQTT conectado com sucesso!\n");
        *connected = true;
    } else {
        printf("MQTT falha na conexão: %d\n", status);
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
    // Converte o IP de string para formato numérico
    if (!ip4addr_aton(broker_ip, &broker_addr)) {
        sniprintf(text_buffer, 100, "Erro no IP\n");
        return;
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
    if (result == ERR_OK) {
        printf("Desinscrição realizada com sucesso!\n");
    } else {
        printf("Falha ao se desinscrever no tópico.\n");
    }
}

// Callback de inscrição no tópico
void mqtt_sub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Inscrição realizada com sucesso!\n");
    } else {
        printf("Falha ao se inscrever no tópico.\n");
    }
}

/* Callback de confirmação de publicação
 * Chamado quando o broker confirma recebimento da mensagem (para QoS > 0)
 * Parâmetros:
 *   - arg: argumento opcional
 *   - result: código de resultado da operação */
static void mqtt_pub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Publicação MQTT enviada com sucesso!\n");
    } else {
        printf("Erro ao publicar via MQTT: %d\n", result);
    }
}

/* Função para publicar dados em um tópico MQTT
 * Parâmetros:
 *   - topic: nome do tópico (ex: "sensor/temperatura")
 *   - data: payload da mensagem (bytes)
 *   - len: tamanho do payload */
void mqtt_comm_publish(const char *topic, const uint8_t *data, size_t len, int retain) {

        while(!wifi_is_connected()){
        printf("Wi-Fi não conectado, tentando reiniciar...\n");
        wifi_restart();
    }

    while(!mqtt_is_connected()) {
        printf("MQTT não conectado, tentando reiniciar...\n");

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

    if (status != ERR_OK) {
        printf("mqtt_publish falhou ao ser enviada: %d\n", status);
    }
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

    if (status != ERR_OK) {
        printf("Erro ao se desinscrever no tópico: %d\n", status);
    }
}

/* Função para inscrever em um tópico MQTT
 * Parâmetros:
 *   - client: instância do cliente MQTT
 *   - topico: nome do tópico a ser inscrito */
void mqtt_comm_subscribe(const char *topico) {

    while(!wifi_is_connected()){
        printf("Wi-Fi não conectado, tentando reiniciar...\n");
        wifi_restart();
    }

    while(!mqtt_is_connected()) {
        printf("MQTT não conectado, tentando reiniciar...\n");
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

    if (status != ERR_OK) {
        printf("Erro ao se inscrever no tópico: %d\n", status);
    }
}

void received_data_cb(void *rec_data) {
    
    while(!wifi_is_connected()){
        printf("Wi-Fi não conectado, tentando reiniciar...\n");
        wifi_restart();
    }

    while(!mqtt_is_connected()) {
        printf("MQTT não conectado, tentando reiniciar...\n");
        mqtt_setup(used_client_id, used_broker_ip, used_user_mqtt, used_pass_mqtt, used_mqtt_buffer);

        uint64_t last_time = get_absolute_time();
        while(get_absolute_time() - last_time < 1000000) {
            ;
        }    
    }

    mqtt_set_inpub_callback(client, NULL, mqtt_data_cb, rec_data);
}