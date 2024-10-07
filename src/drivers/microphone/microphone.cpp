// microphone.cpp (Implement the microphone class methods in here)

#include "microphone.h"
#include "board.h"
#include <hardware/adc.h>
#include <stdio.h>
#include "pico/stdlib.h"

// Channel 0 is GPIO26
#define MICROPHONE_ADC_CHANNEL 0
#define CAPTURE_DEPTH 1024 // microphone buffer depth


// Constructor
MICROPHONE::MICROPHONE() {
}

// Initialise microphone
void MICROPHONE::init() {
    adc_init();
    adc_select_input(MICROPHONE_ADC_CHANNEL);
    adc_fifo_setup(
        true,    // Write each completed conversion to the sample FIFO
        false,   // Disable DMA data request (DREQ)
        0,       // DREQ (and IRQ) not asserted when at least 1 sample present
        false,   // We won't see the ERR bit because of 8 bit reads; disable.
        false    // Don't shift each sample to 8 bits when pushing to FIFO (we want all 12bits)
    );

    // Set the ADC Clock divisor.
    // We want something close to T = 1/44.1kHz
    adc_set_clkdiv(1087);
}


void MICROPHONE::read(uint16_t* capture_buf, unsigned int number_of_samples) {
    // Turn on free-running sampling mode
    adc_run(true);

    // Loop to capture the specified number of samples
    for (uint i = 0; i < number_of_samples; ++i) {
        // Read a sample from the FIFO buffer and store it in the buffer
        capture_buf[i] = adc_fifo_get_blocking();
    }

    // Turn off free-running sampling mode after collecting the samples
    adc_run(false);

    // Drain the FIFO to discard any further samples that were collected by the ADC
    adc_fifo_drain();

    printf("test 1\n");

    // // Print the contents of the buffer
    // printf("Captured samples:\n");
    // for (unsigned int i = 0; i < number_of_samples; ++i) {
    //     printf("Sample %u: %u\n", i, capture_buf[i]);
    // }
}



