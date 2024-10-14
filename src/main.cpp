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

volatile int current_task = 0;  // Variable to track the current task
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
            // case 5:
            //     printf("Running I2C Scan Program\n");
            //     I2C_scan_program();
            //     break;
            default:
                printf("Unknown Program!\n");
                break;
        }
    }
    return 0;
}

// #include <stdio.h>
// #include "pico/stdlib.h"
// #include "hardware/gpio.h"
// #include "hardware/i2c.h"
// #include "pico/binary_info.h"

// #include "board.h"

// // Remove this if causing conflicts
// //#include <gpio.h>

// // Define SDA and SCL pins if not in board.h


// // I2C reserves some addresses for special purposes. We exclude these from the scan.
// // These are any addresses of the form 000 0xxx or 111 1xxx
// bool reserved_addr(uint8_t addr) {
//     return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
// }

// int main() {
//     // Enable UART so we can print status output
//     stdio_init_all();
// #if !defined(ACCEL_I2C_INSTANCE) || !defined(ACCEL_SDA_PIN) || !defined(ACCEL_SCL_PIN)
// #warning i2c/bus_scan example requires a board with I2C pins
//     puts("Default I2C pins were not defined");
// #else
//     // This example will use I2C0 on the default SDA and SCL pins (GP4, GP5 on a Pico)
//     i2c_init(ACCEL_I2C_INSTANCE, 100 * 1000);
//     gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);
//     gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);
//     gpio_pull_up(ACCEL_SDA_PIN);
//     gpio_pull_up(ACCEL_SCL_PIN);
//     // Make the I2C pins available to picotool
//     bi_decl(bi_2pins_with_func(ACCEL_SDA_PIN, ACCEL_SCL_PIN, GPIO_FUNC_I2C));

//     printf("\nI2C Bus Scan\n");
//     printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");

//     for (int addr = 0; addr < (1 << 7); ++addr) {
//         if (addr % 16 == 0) {
//             printf("%02x ", addr);
//         }

//         // Perform a 1-byte dummy read from the probe address. If a slave
//         // acknowledges this address, the function returns the number of bytes
//         // transferred. If the address byte is ignored, the function returns
//         // -1.

//         // Skip over any reserved addresses.
//         int ret;
//         uint8_t rxdata;
//         if (reserved_addr(addr))
//             ret = PICO_ERROR_GENERIC;
//         else
//             ret = i2c_read_blocking(ACCEL_I2C_INSTANCE, addr, &rxdata, 1, false);

//         printf(ret < 0 ? "." : "@");
//         printf(addr % 16 == 15 ? "\n" : "  ");
//     }
//     printf("Done.\n");
//     return 0;
// #endif
// }



