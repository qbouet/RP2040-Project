#include "../programs.h"
#include "../drivers/leds.h"
#include "../drivers/LIS3DH/LIS3DH.h"
#include <stdio.h>
#include <time.h>
#include <pico/stdio.h>
#include <pico/time.h>

// Basic accelerometer program
void Accelerometer_program() {

    stdio_init_all();

    LEDs myLEDs;
    myLEDs.init();

    LIS3DH accel;
    accel.init();

    float x_accel;
    float y_accel;
    float z_accel;

    while (!end_task) {

        accel.get_xyz_measurements_g(x_accel, y_accel, z_accel);

        // Print measurements to terminal
        printf("\nX: %f\n", x_accel);
        printf("Y: %f\n", y_accel);
        printf("Z: %f\n", z_accel);

        myLEDs.turn_all_off();
        if (x_accel < 0){
            myLEDs.set_LED_colour(4,0,30,0);
        }
        if (x_accel > 0){
            myLEDs.set_LED_colour(7,0,30,0);
        }

        if (y_accel < 0){
            myLEDs.set_LED_colour(0,30,0,0);
        }
        if (y_accel > 0){
            myLEDs.set_LED_colour(3,30,0,0);
        }

        if (z_accel < 0){
            myLEDs.set_LED_colour(8,0,0,30);
        }
        if (z_accel > 0){
            myLEDs.set_LED_colour(11,0,0,30);
        }

        myLEDs.apply_changes();
        sleep_ms(100);  // Read data every 100ms
    }
    
}