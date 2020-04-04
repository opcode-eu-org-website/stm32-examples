/*
 * STM32 - ćwiczenie 2, diody LED sterowane przyciskiem
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */

/*
  Przycisk na A0 zgodnie ze schematem
 */

#ifndef STM32F1
#define STM32F1
#endif


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdio.h>


int main(){
  // Uruchomienie peryferiów portów A, C
  // Włączenie sygnału zegara dla portów A, C
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOC);

  // Ustawienie pinu C13 w trybie wyjścia
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
		GPIO_CNF_INPUT_FLOAT, GPIO0);

  int16_t stan_a;
  int32_t x = 0 ;
  while(1){
    x++;
    // Poczekaj chwilkę
    for (int i = 0; i < 150000; i++) __asm__("nop");
    // Przełącz stan pinu 13 w porcie C bazując na wejściu na porcie A
    stan_a = gpio_port_read(GPIOA);
    if(stan_a & 0x01){
      gpio_set(GPIOC, GPIO13);
    }else{
      gpio_clear(GPIOC, GPIO13);
    }
  }
}
