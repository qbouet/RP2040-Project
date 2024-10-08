#include "../programs.h"
#include "../drivers/leds.h"
#include "../drivers/LIS3DH/LIS3DH.h"
#include <stdio.h>
#include <time.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <hardware/uart.h>
#include <hardware/gpio.h>

#define BAUD_RATE 115200
#define UART_ID uart1
#define UART_TX_PIN 8
#define UART_RX_PIN 9

// Basic accelerometer program
void Bluetooth_program() {

    stdio_init_all();

    // uart for bluetooth
    // Initialize the UART
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);

    LEDs myLEDs;
    myLEDs.init();

    LIS3DH accel;
    accel.init();

    float x_accel;
    float y_accel;
    float z_accel;

      char buffer[100];  // Buffer to hold the formatted string

    while (!end_task) {

        accel.get_xyz_measurements_g(x_accel, y_accel, z_accel);

        // Print measurements to terminal
        printf("\nX: %f\n", x_accel);
        printf("Y: %f\n", y_accel);
        printf("Z: %f\n", z_accel);

        // Send measurements over UART
        snprintf(buffer, sizeof(buffer), "X: %f, Y: %f, Z: %f\n", x_accel, y_accel, z_accel);
        uart_puts(UART_ID, buffer);  // Transmit the formatted string


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