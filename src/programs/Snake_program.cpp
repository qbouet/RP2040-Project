#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "../drivers/logging/logging.h"
#include "../drivers/leds.h"


// Snake
void Snake_program() {
    stdio_init_all();
    LEDs myLEDs;
    myLEDs.init();
    myLEDs.custom_snake(4, 20, 20, 0);
}