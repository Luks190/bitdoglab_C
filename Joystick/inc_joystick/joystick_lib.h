#ifndef JOYSTICK_H
#define JOYSTICK_H

// Inicializa o joystick configurando o ADC (Conversor Analógico-Digital)
void joystick_init();

// Função que retorna a posição do joystick no eixo Y
int value_of_the_pos_of_joystick_y();

// Função que detecta se houve movimento no joystick no eixo Y
int read_movement_of_joystick_y();

// Função que retorna a posição do joystick no eixo X
int value_of_the_pos_of_joystick_x();

// Função que detecta se houve movimento no joystick no eixo X
int read_movement_of_joystick_x();

#endif