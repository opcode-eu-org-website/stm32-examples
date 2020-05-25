/*
 * STM32 - ćwiczenie 3, UART
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>

#include <stdio.h>

#include "uart.h"
int main() {
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  
  // ustawiamy adres wektora przerwań na adres początkowy pamięci FLASH
  SCB_VTOR = FLASH_BASE;
  // aktywujemy przerwania z USART1
  nvic_enable_irq(NVIC_USART1_IRQ);
  // aktywujemy przerwania związane z odbiorem danych z USART1
  usart_enable_rx_interrupt(USART1);
  
  // aktywujemy USART1 jak wcześniej - parametry transmisji, etc
  usart_setup();
  // aktywujemy pin A10 jako wejściowy
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);
  // zmieniamy tryb na RX/RX
  usart_set_mode(USART1, USART_MODE_TX_RX);
  
  for(int j = 0; j < 10; j++) {
    printf("...%d\n", j);
    for (int i = 0; i < 150000; i++) __asm__("nop");
    gpio_toggle(GPIOC, GPIO13);
  }
  
  while(1) {
    __asm__("nop");
  }
}

void usart1_isr(void) {
  if ( USART_SR(USART1) & USART_SR_RXNE ) {
    // przerwanie było z powodu odebranego bajtu
    uint8_t data = usart_recv(USART1);
    if (data%2)
      gpio_set(GPIOC, GPIO13);
    else
      gpio_clear(GPIOC, GPIO13);
  }
}
