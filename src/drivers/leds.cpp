// leds.cpp (Implement the leds class methods in here)

#include "leds.h"
#include "board.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file

extern volatile bool end_task;


uint32_t led_data [NUMBER_OF_LEDS]; // stores all the LED colours

// Constructor
LEDs::LEDs() {
}

// Initialise LED
void LEDs::init() {
    // Initialise PIO0 to control the LED chain
    uint pio_program_offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, pio_program_offset, LED_PIN, 800000, false);
}

// Turn a single LED on, only specify led_index to turn off
void LEDs::set_LED_colour(int led_index, uint8_t red, uint8_t green, uint8_t blue) {
    led_data[led_index] = (red << 24) | (green << 16) | (blue << 8);
}

// Turn a single LED off
void LEDs::turn_off(int led_index) {
    led_data[led_index] = (0 << 24) | (0 << 16) | (0 << 8);
}

// Turn all the LEDs off
void LEDs::turn_all_off() {
    for (int led_count = 0; led_count < NUMBER_OF_LEDS; led_count++) {
        turn_off(led_count);
    }
}

// Apply changes to LEDs
void LEDs::apply_changes() {
    sleep_us(300); // "reset" (>280us) to ensure led_index starts at the first led
    for (int led_index = 0; led_index < NUMBER_OF_LEDS; led_index++) {
        pio_sm_put_blocking(pio0, 0, led_data[led_index]);
    }  
}

// Run original snake
void LEDs::snake() {
    int led_count = 0;
    int led_start = -2;
    while (!end_task) {
        for (int led_count = 0; led_count < NUMBER_OF_LEDS; led_count++) {
            if (led_count >= led_start && led_count < led_start + 3) {
                set_LED_colour(led_count, 20, 0, 20);
            }
            else{
                turn_off(led_count);
            }
        }
        apply_changes();
        led_count = -2;

        if (led_start < NUMBER_OF_LEDS){
            led_start++;
        }
        else {
            led_start = -2;
        }
        sleep_ms(100);
    }
    
}

// Run custom snake
void LEDs::custom_snake(int length, uint8_t red, uint8_t green, uint8_t blue) {
    int led_count = 0;
    int led_start = - length -1;
    while (!end_task) {
        for (int led_count = 0; led_count < NUMBER_OF_LEDS; led_count++) {
            if (led_count >= led_start && led_count < led_start + length) {
                set_LED_colour(led_count, red, green, blue);
            }
            else{
                turn_off(led_count);
            }
        }
        apply_changes();
        led_count = - length -1;

        if (led_start < NUMBER_OF_LEDS){
            led_start++;
        }
        else {
            led_start = - length -1;
        }
        sleep_ms(100);
    }
    
}












