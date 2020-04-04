/*
 * STM32 - ćwiczenie 1, migająca dioda LED
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */
#ifndef STM32F1
#define STM32F1
#endif


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

int main(){
  // Uruchomienie peryferium portu C
  // Włączenie sygnału zegara dla portu C
  rcc_periph_clock_enable(RCC_GPIOC);
  // Ustawienie pinu C13 w trybie wyjścia
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

  while(1){
    // Poczekaj chwilkę
    for (int i = 0; i < 150000; i++) __asm__("nop");
    // Przełącz stan pinu 13 w porcie C
    gpio_toggle(GPIOC, GPIO13);
      
  }
}
