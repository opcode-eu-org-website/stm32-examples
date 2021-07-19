/*
 * STM32 - PWM
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 * Robert Paciorek <rrp@opcode.eu.org>
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/timer.h>

#include <stdio.h>

#include "uart.h"
int main() {
  // LED na GPIO 13
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  
  // USART
  SCB_VTOR = FLASH_BASE;
  nvic_enable_irq(NVIC_USART1_IRQ);
  usart_enable_rx_interrupt(USART1);
  usart_setup();
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO10);
  usart_set_mode(USART1, USART_MODE_TX_RX);
  
  printf("init\n");
  
  
  // test PA6
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO6);
  for(int j = 0; j < 4; j++) {
    for (int i = 0; i < 150000; i++) __asm__("nop");
    gpio_toggle(GPIOA, GPIO6);
  }
  
  
  // konfiguracja PWM na PA6 (TIM3 - OC1)
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
  
  // włączenie zegara i reset dla TIM3
  rcc_periph_clock_enable(RCC_TIM3);
  rcc_periph_reset_pulse(RST_TIM3);
  // konfiguracja TIM3:
  //  standardowe (TIM_CR1_CMS_EDGE) odliczanie w górę (TIM_CR1_DIR_UP)
  //  działanie cykliczne (timer_continuous_mode)
  //  prescaler = 32, bez dodatkowego mnożnika (TIM_CR1_CKD_CK_INT) => jedno tyknięcie co 4 us
  //  okres = 100 (tyknięć) = 400 us
  timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
  timer_continuous_mode(TIM3);
  timer_set_prescaler(TIM3, 32 - 1);
  timer_set_period(TIM3, 100 - 1);
  
  // konfigurujemy wyjście OC1 (dostępne domyślnie na PA6)
  //  nie jest czyszczone (OC1CE=0) przez zewnętrzny sygnał triggera (TRGI)
  timer_disable_oc_clear(TIM3, TIM_OC1);
  //  wartość porównywana (TIM3_CCR1) programowana z użyciem preloadera (OC1PE=1)
  timer_enable_oc_preload(TIM3, TIM_OC1);
  //  nie używamy fast mode (OC1FE=0)
  timer_set_oc_slow_mode(TIM3, TIM_OC1);
  //  używamy PWM1: licznik < próg => stan wysoki, licznik > próg => stan niski
  timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
  //  ustawiamy wartość progu na 200 (próg/okres = 20/100 = 20% wypełnienia)
  timer_set_oc_value(TIM3, TIM_OC1, 20);
  //  włączamy wyjście OC1
  timer_enable_oc_output(TIM3, TIM_OC1);
  
  // rozpoczynamy odliczanie timera TIM3
  timer_enable_counter(TIM3);
  
  
  // komunikat powitalny
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
    uint8_t data = usart_recv(USART1);
    uint16_t val = 10 * (data - '0');
    timer_set_oc_value(TIM3, TIM_OC1, val );
    printf("%d %ld\n", val, TIM3_CCR1);
  }
}
