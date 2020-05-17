/*
 * STM32 - ćwiczenie 3, UART
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */
#ifndef STM32F1
#define STM32F1
#endif


#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <stdio.h>

#include "uart.h"
int main(){
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  usart_setup();
  while(1){
    for (int i = 0; i < 150000; i++) __asm__("nop");
    gpio_toggle(GPIOC, GPIO13);
    printf("Hello, World!\n");
  }
}
