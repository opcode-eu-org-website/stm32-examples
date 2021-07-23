STM-examples
============

Repozytorium zawiera przykłady kodu w C na potrzeby kursu „Linux i Python w Elektronicznej Sieci”.

Skrypt do zajęć z SMT32 (zawierający informacje o podłączeniu mikrokontrolera oraz opis poszczególnych przykładów znajduje się pod adresami:
	http://www.opcode.eu.org/Podstawy_programowania_mikrokontrolerów_STM32.pdf oraz http://ciekawi.icm.edu.pl/lpes/stm32.pdf

Przykłady zostały stworzone dla mikrokontrolera STM32F103, dostępnego np. na płytce "blue-pill".
Możliwe jest ich uruchamianie / przenoszenie na inne mikrokontrolery wspierane przez libopencm3 (zobacz m.in. [info/gpio.md](info/gpio.md))

Repozytorium podzielone jest na podkatalogi. Każdy z nich to oddzielne ćwiczenie, zaznajamiające z daną funkcjonalnością mikrokontrolera.

Przykłady z repozytorium oparte są o bibliotekę libopencm3:
	https://github.com/libopencm3/libopencm3

Aby ich używać należy mieć pobraną i skompilowaną tę bibliotekę.
Jej położenie można wskazać za linku symbolicznego o nazwie libopencm3, wskazującego na katalog z skompilowaną biblioteką libopencm3 lub zmiennej środowiskowej OPENCM3_DIR wskazującej na ten katalog.


## Przykłady

### obsługa GPIO

* [sterowanie wyjściem](10_blink)
* [odczyt stanu wejścia](11_di)

### obsługa portu szeregowego

* [wysyłanie danych](20_uart), w tym  funkcje pozwalająca na używanie printf via UART
* [z obsługą przerwań](21_uart_receiver)
* [wysyłanie danych via UART w stylu printf](2-UART/serialPrintf-lib4avr.c) – biblioteczka używana w dalszych przykładach

### obsługa przetwornika analogo-cyfrowego

* [odczyt wartości z ADC](30_adc)

### obsługa I2C

* [master i slave](4-I2C/40_i2c)

### timery i PWM

* [sprzętowy pwm z użyciem timera](51_pwm)


## Zobacz także:

* [minimalna implementacja Modbus RTU dla STM32F103](https://bitbucket.org/OpCode-eu-org/stm32-modbusrtu)
