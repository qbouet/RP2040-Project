#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"
#include "drivers/leds.h"
#include "drivers/LIS3DH/LIS3DH.h"
#include "drivers/microphone/microphone.h"

#include "programs.h"

#define LED_PIN 14
#define BUTTON_PIN 15  // GPIO pin for button
#define CAPTURE_DEPTH 1024 // microphone buffer depth

volatile int current_task = 3;  // Variable to track the current task
volatile bool end_task = false;  // Variable to end the current task

// Programs
void Colliding_LEDs_program();
void Accelerometer_program();
void Snake_program();
void Microphone_program();

// Interrupt handler for the button press
void button_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN) {
        end_task = true;
        current_task = (current_task + 1) % 4;  // Toggle between 4 tasks (0, 1, 2, 3)
        printf("Button pressed, switching to task %d\n", current_task);
    }
}

int main() {
    // Initialize standard IO for print statements
    stdio_init_all();  

    // Initialize button GPIO
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
  
    // Attach interrupt to button pin with a callback on falling edge
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    // Main loop
    while (true) {
        end_task = false;
        switch (current_task) {
            case 0:
                printf("Running Colliding LEDs Program\n");
                Colliding_LEDs_program();
                break;
            case 1:
                printf("Running Accelerometer Program\n");
                Accelerometer_program();
                break;
            case 2:
                printf("Running Snake Program\n");
                Snake_program();
                break;
            case 3:
                printf("Running Microphone Program\n");
                Microphone_program();
                break;
            default:
                printf("Unknown Program!\n");
                break;
        }
    }
    return 0;
}


// Define each program as a function

// // Colliding LEDs
// void Colliding_LEDs_program() {

//     LEDs myLEDs;

//     // Initialise PIO0 to control the LED chain
//     myLEDs.init();

//     // Colliding LEDs
//     while (!end_task){
//         int left = 0;
//         int right = 11;
//         for (int i = 0; i < 6; i++) {
//             // left LEDs
//             myLEDs.set_LED_colour(left, 0 + 5*i, 0, 20 - 3*i);
//             myLEDs.apply_changes();
//             sleep_ms(100 - i*10);
//             myLEDs.turn_all_off();

//             // right LEDs
//             myLEDs.set_LED_colour(right, 0 + 5*i, 0, 20 - 3*i);
//             myLEDs.apply_changes();
//             sleep_ms(100 - i*10);
//             myLEDs.turn_all_off();

//             left++;
//             right--;
//         }

//         for (int i = 0; i < 3; i++) {
//             // left LEDs
//             myLEDs.set_LED_colour(right, 30, 0, 3);
//             myLEDs.apply_changes();
//             sleep_ms(50);
//             myLEDs.turn_all_off();

//             // right LEDs
//             myLEDs.set_LED_colour(left, 30, 0, 3);
//             myLEDs.apply_changes();
//             sleep_ms(50);
//             myLEDs.turn_all_off();
//         }
//     }

//}

// // Basic accelerometer program
// void Accelerometer_program() {

//     stdio_init_all();

//     LEDs myLEDs;
//     myLEDs.init();

//     LIS3DH accel;
//     accel.init();

//     float x_accel;
//     float y_accel;
//     float z_accel;

//     while (!end_task) {

//         accel.get_xyz_measurements_g(x_accel, y_accel, z_accel);

//         // Print measurements to terminal
//         printf("\nX: %f\n", x_accel);
//         printf("Y: %f\n", y_accel);
//         printf("Z: %f\n", z_accel);

//         myLEDs.turn_all_off();
//         if (x_accel < 0){
//             myLEDs.set_LED_colour(4,0,30,0);
//         }
//         if (x_accel > 0){
//             myLEDs.set_LED_colour(7,0,30,0);
//         }

//         if (y_accel < 0){
//             myLEDs.set_LED_colour(0,30,0,0);
//         }
//         if (y_accel > 0){
//             myLEDs.set_LED_colour(3,30,0,0);
//         }

//         if (z_accel < 0){
//             myLEDs.set_LED_colour(8,0,0,30);
//         }
//         if (z_accel > 0){
//             myLEDs.set_LED_colour(11,0,0,30);
//         }

//         myLEDs.apply_changes();
//         sleep_ms(100);  // Read data every 100ms
//     }
    
// }

// // Snake
// void Snake_program() {
//     stdio_init_all();
//     LEDs myLEDs;
//     myLEDs.init();
//     myLEDs.custom_snake(4, 20, 20, 0);
// }

// // Microphone
// void Microphone_program() {
//     stdio_init_all();
//     MICROPHONE microphone;    

//     uint16_t buffer[CAPTURE_DEPTH];

//     microphone.init();
//     microphone.read(buffer, 1024);


// }