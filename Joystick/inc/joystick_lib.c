#include "pico/stdlib.h" // Biblioteca para operações de entrada e saída
#include "hardware/adc.h" 

#define SW 22 // Pino de leitura do botão do joystick

// Inicializa o joystick configurando o ADC (Conversor Analógico-Digital)
void joystick_init(){
    adc_init(); // Inicializa o ADC para permitir leituras analógicas
    adc_gpio_init(26); // Configura o pino GPIO 26 como entrada ADC Y
    adc_gpio_init(27); // Configura o pino GPIO 27 como entrada ADC X

    gpio_init(SW);             // Inicializa o pino do botão
    gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
    gpio_pull_up(SW);
}

// Função que retorna a posição do joystick no eixo Y
int value_of_the_pos_of_joystick_y(){
    uint16_t vry_value; // Variável para armazenar o valor lido do ADC

    adc_select_input(0); // Seleciona o canal 0 do ADC (eixo Y do joystick)
    sleep_us(2); // Pequeno delay para garantir estabilidade na leitura
    vry_value = adc_read(); // Lê o valor analógico do eixo Y (0 a 4095)
       
    // Verifica se o joystick foi movido para cima
    if(vry_value < 500){
        return 1;  // Indica movimento para cima
    }
    // Verifica se o joystick foi movido para baixo
    else if(vry_value > 3500){
        return -1; // Indica movimento para baixo
    }
    // Se estiver no meio, considera como neutro
    else{
        return 0;  // Sem movimento significativo
    }
}

// Função que detecta se houve movimento no joystick no eixo Y
int read_moviment_of_joystick_y(){
    uint16_t vry_value; // Variável para armazenar a leitura do ADC

    adc_select_input(0); // Seleciona o canal 0 do ADC (eixo Y do joystick)
    sleep_us(2); // Pequeno delay para garantir estabilidade na leitura
    vry_value = adc_read(); // Lê o valor do eixo Y do joystick
        
    // Se o valor estiver fora da faixa central, considera que houve movimento
    if(vry_value < 500 || vry_value > 3500){
        return 1; // Movimento detectado
    }else{
        return 0; // Nenhum movimento detectado
    }
}

// Função que retorna a posição do joystick no eixo X
int value_of_the_pos_of_joystick_x(){
    uint16_t vrx_value; // Variável para armazenar o valor lido do ADC

    adc_select_input(1); // Seleciona o canal 1 do ADC (eixo X do joystick)
    sleep_us(2); // Pequeno delay para garantir estabilidade na leitura
    vrx_value = adc_read(); // Lê o valor analógico do eixo X (0 a 4095)
       
    // Verifica se o joystick foi movido para a esquerda
    if(vrx_value < 500){
        return 1;  // Indica movimento para a esquerda
    }
    // Verifica se o joystick foi movido para a direita
    else if(vrx_value > 3500){
        return -1; // Indica movimento para a direita
    }
    // Se estiver no meio, considera como neutro
    else{
        return 0;  // Sem movimento significativo
    }
}

// Função que detecta se houve movimento no joystick no eixo X
int read_moviment_of_joystick_x(){
    uint16_t vrx_value; // Variável para armazenar a leitura do ADC

    adc_select_input(1); // Seleciona o canal 1 do ADC (eixo X do joystick)
    sleep_us(2); // Pequeno delay para garantir estabilidade na leitura
    vrx_value = adc_read(); // Lê o valor do eixo X do joystick
        
    // Se o valor estiver fora da faixa central, considera que houve movimento
    if(vrx_value < 500 || vrx_value > 3500){
        return 1; // Movimento detectado
    }else{
        return 0; // Nenhum movimento detectado
    }
}
