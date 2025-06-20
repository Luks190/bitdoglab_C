#include <stdio.h>
#include "pico/stdlib.h"
#include "inc_joystick/joystick_lib.h"

int main() {
    stdio_init_all(); 
    joystick_init(); // Inicializa o joystick
    
    while (1) {
        bool btn_v = false, btn_v2 = false;

        // Exibe os resultados formatados
        printf("\n--- Estado do Joystick ---\n");
        printf("Eixo X: %s | Movimento: %s\n", 
               (value_of_the_pos_of_joystick_x() == 1) ? "Esquerda" : (value_of_the_pos_of_joystick_x() == -1) ? "Direita" : "Neutro",
               read_movement_of_joystick_x() ? "Detectado" : "Ausente");
        
        printf("Eixo Y: %s | Movimento: %s\n", 
               (value_of_the_pos_of_joystick_y() == 1) ? "Baixo" : (value_of_the_pos_of_joystick_y() == -1) ? "Cima" : "Neutro",
               read_movement_of_joystick_y() ? "Detectado" : "Ausente");

        printf("-------------------------\n");
        printf("porcentagem do eixo X: %.2f%%\n", read_percentage_of_move_x());
        printf("porcentagem do eixo Y: %.2f%%\n", read_percentage_of_move_y());

        printf("\nvalor do eixo x: %d\n", read_value_x());
        printf("valor do eixo y: %d\n", read_value_y());
        printf("-------------------------\n");

        sleep_ms(500); // Atualiza a cada 500ms
    }
    return 0;
}
