Konfiguracja GPIO
=================

Rodzina STM32F1 używa trochę odmiennego od wielu innych rodzin modelu konfiguracji wejść-wyjść GPIO.

Funkcja `gpio_set_mode` realizuje zarówno zadania `gpio_mode_setup` jak i `gpio_set_output_options`,
używanych  w libopencm3 dla (przynajmniej niektórych) innych rodzin mikrokontrolerów STM32.

Największa róznicą jest sposób wyboru alternatywnych funkcji dla danego pinu - w wielu wypadkach jest to konfigurowane per pin z użyciem `gpio_set_af`.
Jednak STM32F1 pozwala jedynie na przemapowywanie całymi grupami  – poprzez wybór mapy dla danego peryferium, a nie wybór alternatywnej funkcji dla danego pinu
 (np. nie można mieć I2C1_SCL na PB6 a I2C1_SDA na PB9).
Szczegóły można znaleźć w rozdziale "9.3 Alternate function I/O and debug configuration (AFIO)" w [STM32F103xx Reference manual RM0008](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf).
Do konfiguracji mapowania w libopencm3 służą funkcje gpio_primary_remap i pio_secondary_remap.

STM32F1:
--------

	gpio_primary_remap( ... );  pio_secondary_remap( ... );
	
	gpio_set_mode  ( gpioport,          mode,                    cfg,                  gpios )
	                           GPIO_MODE_INPUT          GPIO_CNF_INPUT_ANALOG
	                           GPIO_MODE_OUTPUT_2_MHZ   GPIO_CNF_INPUT_FLOAT
	                           GPIO_MODE_OUTPUT_10_MHZ  GPIO_CNF_INPUT_PULL_UPDOWN
	                           GPIO_MODE_OUTPUT_50_MHZ  GPIO_CNF_OUTPUT_PUSHPULL
	                                                    GPIO_CNF_OUTPUT_OPENDRAIN
	                                                    GPIO_CNF_OUTPUT_ALTFN_PUSHPULL
	                                                    GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN

STM32F0, STM32F4, STM32G0, ...:
-------------------------------

	gpio_set_af ( gpioport, alt_func_num, gpios )
	                          GPIO_AF0
	                          GPIO_AF1
	                          GPIO_AF2
	                            ...
	
	gpio_set_output_options( gpioport,     otype,           speed,        gpios )
	                                   GPIO_OTYPE_PP   GPIO_OSPEED_2MHZ
	                                   GPIO_OTYPE_OD   GPIO_OSPEED_25MHZ
	                                                   GPIO_OSPEED_50MHZ
	                                                   GPIO_OSPEED_100MHZ
	
	gpio_mode_setup( gpioport,     mode,           pull_up_down,       gpios )
	                           GPIO_MODE_INPUT,   GPIO_PUPD_NONE
	                           GPIO_MODE_OUTPUT   GPIO_PUPD_PULLUP
	                           GPIO_MODE_AF       GPIO_PUPD_PULLDOWN
	                           GPIO_MODE_ANALOG

Dokumentacja libopencm3:
------------------------

* http://libopencm3.org/docs/latest/stm32f1/html/group__gpio__defines.html
* http://libopencm3.org/docs/latest/stm32g0/html/group__gpio__defines.html
