#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ledMatrix_lib.h"
#include "ws2818b.pio.h"


#define LED_COUNT 25


// Definição de pixel GRB
struct pixel_t {
  uint8_t G, R, B; // Três valores de 8-bits compõem um pixel.
};
typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t" por clareza.

// Declaração do buffer de pixels que formam a matriz.
npLED_t leds[LED_COUNT];

// Variáveis para uso da máquina PIO.
PIO np_pio;
uint sm;

/**
 * Inicializa a máquina PIO para controle da matriz de LEDs.
 */
void npInit(uint pin) {

  // Cria programa PIO.
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;

  // Toma posse de uma máquina PIO.
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true); // Se nenhuma máquina estiver livre, panic!
  }

  // Inicia programa na máquina PIO obtida.
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

  // Limpa buffer de pixels.
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds[i].R = 0;
    leds[i].G = 0;
    leds[i].B = 0;
  }


}

/**
 * Escreve os dados do buffer nos LEDs.
 */
void npWrite() {
  // Escreve cada dado de 8-bits dos pixels em sequência no buffer da máquina PIO.
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds[i].G);
    pio_sm_put_blocking(np_pio, sm, leds[i].R);
    pio_sm_put_blocking(np_pio, sm, leds[i].B);
  }
  sleep_us(100); // Espera 100us, sinal de RESET do datasheet.
}

/**
 * Reverte a ordem de um byte LSB -> MSB e sucessivamente.
 * @param b o byte a ser revertido
 */
uint8_t reverse_byte(uint8_t b) {
  uint8_t reversed = 0; // Inicializa o byte a ser revertido

  // Faz a reversão bit a bit seguindo a lógica: 
  // 1. Captura o i-ésimo bit do byte original ((b >> i) & 0x1)
  // 2. Envia-o para a variável reversed com o operador |
  // 3. Arrasta o bit transferido para a esquerda (reversed << 1)
  for (uint8_t i = 0; i < 8; i++) {
    reversed = reversed << 1; // 
    reversed = (reversed | ((b >> i) & 0x1)); 
  }

  return reversed;
}


/**
 * Atribui uma cor RGB a um LED.
 */
void npSetLED(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  uint8_t reversed_r = reverse_byte(r);
  uint8_t reversed_g = reverse_byte(g);
  uint8_t reversed_b = reverse_byte(b);

  leds[index].R = reversed_r;
  leds[index].G = reversed_g;
  leds[index].B = reversed_b;
}

void setLedInPosition(int X, int Y, const uint8_t r, const uint8_t g, const uint8_t b) {
    if(Y % 2 == 0){
        npSetLED((4 - X)+(Y * 5), r, g, b);
    }else{
        npSetLED((X)+(Y * 5), r, g, b);
    }
}

void setLedInPositionWithHexa(int X, int Y, const uint32_t color) {
    // Extrai os componentes de cor do valor hexadecimal
    uint8_t r = (color >> 16) & 0xFF; // Extrai o componente vermelho
    uint8_t g = (color >> 8) & 0xFF;  // Extrai o componente verde
    uint8_t b = color & 0xFF;         // Extrai o componente azul

    setLedInPosition(X, Y, r, g, b);
}

/**
 * Limpa o buffer de pixels.
 */
void npClear() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED(i, 0, 0, 0);
}



