#include "../programs.h"
#include "../drivers/leds.h"
#include <time.h>
#include <stdio.h>
#include <pico/stdio.h>
#include <pico/time.h>

// Colliding LEDs
void Colliding_LEDs_program() {

    stdio_init_all();

    LEDs myLEDs;

    // Initialise PIO0 to control the LED chain
    myLEDs.init();

    // Colliding LEDs
    while (!end_task){
        int left = 0;
        int right = 11;
        for (int i = 0; i < 6; i++) {
            // left LEDs
            myLEDs.set_LED_colour(left, 0 + 5*i, 0, 20 - 3*i);
            myLEDs.apply_changes();
            sleep_ms(100 - i*10);
            myLEDs.turn_all_off();

            // right LEDs
            myLEDs.set_LED_colour(right, 0 + 5*i, 0, 20 - 3*i);
            myLEDs.apply_changes();
            sleep_ms(100 - i*10);
            myLEDs.turn_all_off();

            left++;
            right--;
        }

        for (int i = 0; i < 3; i++) {
            // left LEDs
            myLEDs.set_LED_colour(right, 30, 0, 3);
            myLEDs.apply_changes();
            sleep_ms(50);
            myLEDs.turn_all_off();

            // right LEDs
            myLEDs.set_LED_colour(left, 30, 0, 3);
            myLEDs.apply_changes();
            sleep_ms(50);
            myLEDs.turn_all_off();
        }
    }

}