// leds.h (this is declaring the class in a cpp header file)

#ifndef LEDS_H
#define LEDS_H
#include <cstdint>  // For C++ code (use <stdint.h> for C code), required for "uint8_t"

class LEDs {
public:
    // Constructor
    LEDs();

    // Method to initialise LEDs
    void init();

    // Method to turn an LED on
    void set_LED_colour(int, uint8_t, uint8_t, uint8_t);

    // Method to turn an LED off
    void turn_off(int);

    // Method to turn all LEDs off
    void turn_all_off();

    // Method for original snake
    void snake();

    // Method for custom snake
    void custom_snake(int, uint8_t, uint8_t, uint8_t);

    // Method for applying changes
    void apply_changes();
};

#endif // LEDS_H
