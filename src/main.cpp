#include <stdio.h>
#include <iostream>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"

#include "WS2812.pio.h" // This header file gets produced during compilation from the WS2812.pio file
#include "drivers/logging/logging.h"

#include "programs.h"

#define LED_PIN 14
#define BUTTON_PIN 15  // GPIO pin for button
#define DEBOUNCE_TIME_MS 100  // Debounce time in milliseconds

volatile int current_task = 4;  // Variable to track the current task
volatile bool end_task = false;  // Variable to end the current task

// Variable to store the last time the button was pressed
volatile uint32_t last_press_time = 0;

// Interrupt handler for the button press
void button_callback(uint gpio, uint32_t events) {
    // Get the current time in milliseconds
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    // Check if the button press is within the debounce time
    if ((current_time - last_press_time) < DEBOUNCE_TIME_MS) {
        // Ignore this press as it's within the debounce time
        return;
    }

    // Update the last press time
    last_press_time = current_time;

    if (gpio == BUTTON_PIN) {
        end_task = true;
        current_task = (current_task + 1) % 5;  // Toggle between 5 tasks (0, 1, 2, 3, 4)
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
            case 4:
                printf("Running Bluetooth Program\n");
                Bluetooth_program();
                break;
            default:
                printf("Unknown Program!\n");
                break;
        }
    }
    return 0;
}
