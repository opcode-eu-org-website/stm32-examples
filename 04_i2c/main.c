/*
 * STM32 - ćwiczenie 5, I2C
 * projekt ,,Python w Elektronicznej Sieci''
 * Krzysztof Lasocki <krz.lasocki@gmail.com>
 */
#ifndef STM32F1
#define STM32F1
#endif

#include <libopencm3/cm3/common.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/scb.h>

#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/usart.h>

#include <libopencm3/cm3/nvic.h>

#include <stdio.h>
#include "uart.h"


#define SLAVE_ADDR 0x0F

void HardFault_Handler(void){
  printf("HFH!!\n");
  while(1){
    gpio_toggle(GPIOC, GPIO13);
    gpio_toggle(GPIOC, GPIO14);
  }
  
}

uint32_t slavebyte;

void i2c2_ev_isr(void){
  
  uint32_t sr1, sr2;

   sr1 = I2C_SR1(I2C2);

  



   
   // Address matched (Slave)
   if (sr1 & I2C_SR1_ADDR){
       
        //Clear the ADDR sequence by reading SR2.
        sr2 = I2C_SR2(I2C2);
        (void) sr2;
	
	
   }
   //Master write request
   else if(sr1 & I2C_SR1_RxNE){
     gpio_toggle(GPIOC, GPIO13);
     gpio_toggle(GPIOC, GPIO13);

     
     slavebyte = I2C_DR(I2C2);
     slavebyte *= 2;

     I2C_CR1(I2C2) &= ~I2C_CR1_STOP;
   }
   //Master read request
   if ((sr1 & I2C_SR1_TxE)){
     gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);
	gpio_toggle(GPIOC, GPIO14);

     
     //i2c_send_data(I2C2, slavebyte);
     I2C_DR(I2C2) = slavebyte;
     I2C_CR1(I2C2) |= I2C_CR1_STOP;
   }
}



void i2c_setup(){
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_I2C1);
  rcc_periph_clock_enable(RCC_I2C2);


  /* I2C1 - master; SDA=B7, SCL=B6 */
  i2c_reset(I2C1);
  i2c_peripheral_disable(I2C1);
  i2c_set_speed(I2C1, i2c_speed_sm_100k, 8);

  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO6 | GPIO7);
  
  i2c_peripheral_enable(I2C1);


  /* I2C2 - slave; SDA=B11, SCL=B10 */
  i2c_reset(I2C2);
  i2c_peripheral_disable(I2C2);
  i2c_set_speed(I2C2, i2c_speed_sm_100k, 8);

  i2c_set_own_7bit_slave_address(I2C2, SLAVE_ADDR);

  nvic_enable_irq(NVIC_I2C2_EV_IRQ);
  
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_2_MHZ,
		  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO10 | GPIO11);

  i2c_enable_interrupt(I2C2, I2C_CR2_ITEVTEN );
  i2c_peripheral_enable(I2C2);

  
  i2c_enable_ack(I2C2);

}

void i2c_send_write(uint32_t peryf, uint8_t dane){
  /* gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);*/
  
  
  
  i2c_send_start(peryf);
  // Czekaj na wysłanie startu
  while (!((I2C_SR1(peryf) & I2C_SR1_SB)
	   & (I2C_SR2(peryf) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
  
  i2c_send_7bit_address(peryf, SLAVE_ADDR, I2C_WRITE);


  //Czekaj na wysłanie adresu
  while (!(I2C_SR1(peryf) & I2C_SR1_ADDR));
  (void) I2C_SR2(peryf); //Wyczyść EV6

  
  i2c_send_data(peryf, dane);


  while (!(I2C_SR1(peryf) & (I2C_SR1_BTF))) gpio_toggle(GPIOC, GPIO13); //czekaj na wyslanie danych
  i2c_send_stop(peryf);
  
  /*gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);*/
}


uint8_t i2c_send_read(uint32_t peryf){
  gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  gpio_set(GPIOC, GPIO13);
  gpio_clear(GPIOC, GPIO13);
  
  uint8_t dane;
  
  i2c_send_start(peryf);
  // Czekaj na wysłanie startu
  while (!((I2C_SR1(peryf) & I2C_SR1_SB)
	   & (I2C_SR2(peryf) & (I2C_SR2_MSL | I2C_SR2_BUSY))));
  
  i2c_send_7bit_address(peryf, SLAVE_ADDR, I2C_READ);
  //  I2C_DR(peryf) = 0x55;

  //Czekaj na wysłanie adresu
  while (!(I2C_SR1(peryf) & I2C_SR1_ADDR));
  (void) I2C_SR2(peryf); //Wyczyść EV6


  while (!(I2C_SR1(peryf) & I2C_SR1_RxNE)) {gpio_toggle(GPIOC, GPIO14); gpio_toggle(GPIOC, GPIO14);}
  
  dane = i2c_get_data(peryf);
  

  
 
  i2c_send_stop(peryf);

  gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);
  gpio_set(GPIOC, GPIO14);
  gpio_clear(GPIOC, GPIO14);
  
  return dane;
}


int main(){
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
		GPIO_CNF_OUTPUT_PUSHPULL, GPIO13 | GPIO14);
  usart_setup();
  
  for(int j = 0; j < 10; j++) {
    printf("...%d\n", j);
    for (int i = 0; i < 150000; i++) __asm__("nop");
    gpio_toggle(GPIOC, GPIO13);

  }

  printf("SCB_VTOR is");
  printf("%lx\n", SCB_VTOR);
  SCB_VTOR = 0x08000000;
  printf("**** zmieniam SCB_VTOR\n");
  printf("SCB_VTOR is");
  printf("%lx\n", SCB_VTOR);

  
  uint8_t k = 0;
  i2c_setup();
  printf("po i2c setup\n");

  while(1){
    for (int i = 0; i < 45000; i++) __asm__("nop");
    
    printf("Wysylam %d\n", k);
    i2c_send_write(I2C1, k);    
    uint8_t wynik = i2c_send_read(I2C1);
    printf("Odebralem %d\n", wynik);
    k++;

  }
}
