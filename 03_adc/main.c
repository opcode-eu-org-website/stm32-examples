/*
 * STM32 - ćwiczenie 4, ADC
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <stdio.h>

#include "uart.h"

void adc_setup(){
  //Ustawienie A0 w tryb wejścia analogowego
  gpio_set_mode(GPIOA,
		GPIO_MODE_INPUT, GPIO_CNF_INPUT_ANALOG, GPIO0);
  //Wyłączenie ADC
  adc_power_off(ADC1);
  //Konfiguracja dla trybu pojedynczych pomiarów
  adc_disable_scan_mode(ADC1);
  adc_set_single_conversion_mode(ADC1);
  adc_disable_external_trigger_regular(ADC1);
  adc_set_right_aligned(ADC1);
  adc_set_sample_time_on_all_channels(ADC1,
				      ADC_SMPR_SMP_28DOT5CYC);
  //Uruchomienie ADC i chwila przerwy na start
  adc_power_on(ADC1);
  for (int i = 0; i < 150000; i++) __asm__("nop");
  
  //Rekalibracja ADC
  adc_reset_calibration(ADC1);
  adc_calibrate(ADC1);
}

uint16_t adc_read(){
  uint8_t channels[16];
  channels[0] = 0;
  adc_set_regular_sequence(ADC1, 1, channels);
  adc_start_conversion_direct(ADC1);
  while(!adc_eoc(ADC1));
  return adc_read_regular(ADC1);
}

int main(){
  rcc_periph_clock_enable(RCC_GPIOC);
  rcc_periph_clock_enable(RCC_ADC1);
  
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
  usart_setup();
  adc_setup();
  while(1){
    for (int i = 0; i < 150000; i++) __asm__("nop");
    gpio_toggle(GPIOC, GPIO13);
    printf("ADC: %d\n", adc_read());
    
  }
}
