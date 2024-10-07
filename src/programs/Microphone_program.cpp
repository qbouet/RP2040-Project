#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "../drivers/logging/logging.h"
#include "../drivers/microphone/microphone.h"

#define CAPTURE_DEPTH 1024 // microphone buffer depth

// Microphone
void Microphone_program() {
    stdio_init_all();
    MICROPHONE microphone;    

    uint16_t buffer[CAPTURE_DEPTH];

    microphone.init();
    microphone.read(buffer, 1024);


}