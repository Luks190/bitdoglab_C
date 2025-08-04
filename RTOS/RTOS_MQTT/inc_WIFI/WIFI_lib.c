#include <stdio.h>                      // Biblioteca para operações de entrada e saída
#include "pico/cyw43_arch.h"             // Biblioteca para a arquitetura CYW43XX
#include "string.h"                   // Biblioteca para manipulação de strings
#include "pico/time.h"
#include "lwip/pbuf.h"                  
#include "lwip/tcp.h"                   
#include "lwip/dns.h"                   
#include "lwip/init.h"               
#include "WIFI_lib.h"

const char *used_SSID;       
const char *used_PASS;   

// Função para inicializar a conexão Wi-Fi
void wifi_init(const char *WIFI_SSID, const char *WIFI_PASS) {
    used_SSID = WIFI_SSID;       // Atribui o SSID da rede Wi-Fi
    used_PASS = WIFI_PASS;       // Atribui a senha da rede Wi-Fi

    while (cyw43_arch_init()) {

        uint64_t last_time = get_absolute_time() ; // Define o tempo de espera para 1 segundo a partir de agora
        while(get_absolute_time() - last_time < 1000000) {                 
            ;
        }
    }

    cyw43_arch_enable_sta_mode();          

    // Tenta conectar à rede Wi-Fi especificada
    while (cyw43_arch_wifi_connect_blocking(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_MIXED_PSK)) { 
        uint64_t last_time = get_absolute_time() ; // Define o tempo de espera para 1 segundo a partir de agora
        while(get_absolute_time() - last_time < 1000000) {                 

        }
    }   
}

bool wifi_is_connected() {
    // Verifica se a conexão Wi-Fi está ativa
    if (cyw43_tcpip_link_status(&cyw43_state, CYW43_ITF_STA) == CYW43_LINK_UP) {
        return true;
    } else {
        return false;
    }
}

void wifi_restart() {
    // Desconecta e reinicia a conexão Wi-Fi
    cyw43_arch_deinit();
    wifi_init(used_SSID, used_PASS);
}