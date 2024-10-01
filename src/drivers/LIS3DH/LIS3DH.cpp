// LIS3DH.cpp (Implement the LIS3DH accelerometer class methods in here)

#include "LIS3DH.h"
#include "board.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pio.h"
#include <hardware/i2c.h>

#include "drivers/logging/logging.h"
#define GRAVITY 9.81  // Earth's gravity in m/s²
#define SENSITIVITY 0.001 // Sensitivity in g/LSB for ±2g

#include "drivers/leds.h"
// Constructor
LIS3DH::LIS3DH() {
}

// Initialise LIS3DH Accelerometer
void LIS3DH::init() {
    // Set up the RP2040’s I2C interface with a suitable baud rate 
    i2c_init(ACCEL_I2C_INSTANCE, ACCEL_I2C_BAUD_RATE);

    // set ACCEL_MISO (SDO/SA0) to high making the slave address 001100"1"b
    gpio_init(ACCEL_MISO);
    gpio_set_dir(ACCEL_MISO, true);
    gpio_put(ACCEL_MISO, true);

    // configure the SDA and SCL pins to be set as GPIO_FUNC_I2C 
    // (so that they are internally multiplexed to the I2C hardware)
    gpio_set_function(ACCEL_SCL_PIN, GPIO_FUNC_I2C);
    gpio_set_function(ACCEL_SDA_PIN, GPIO_FUNC_I2C);

    // Send an I2C transaction to read the value of the WHO_AM_I
    // register and confirm that the returned value is correct. 
    // This will validate that I2C communications are working and the
    // accelerometer is alive.
    uint8_t raw_data [2];
    if (read_register(WHO_AM_I_REG, raw_data, 1)) {
        printf("it works");
    };

    // Configuring CTRL_REG1
    //  0  1  0  0  0  1  1  1
    //                 \_____/
    //                  Zen, Yen, Xen (enable Z, Y and X axis)
    //             \_/
    //             LPen (set to high-resolution mode & disable low power mode)
    //  \________/
    //   ODR[3:0] (set data rate to 50Hz)
    //
    // See page 35 of datasheet for more detail
    accel_write_register(CTRL_REG_1, 0b01000111);
}

// Function to get accelerometer measurements (from X, Y, Z axis) (in integers, no units)
void LIS3DH::get_xyz_measurements_raw(int16_t &x_accel, int16_t &y_accel, int16_t &z_accel) {
    uint8_t accel_data[6];
    // Read 6 bytes from the accelerometer starting at OUT_X_L
    read_register(ACCEL_DATA_REG | 0x80, accel_data, 6);  // 0x80 sets auto-increment mode

    // Combine high and low bytes for X, Y, and Z axes
    x_accel = combine_bytes(accel_data[0], accel_data[1]);
    y_accel = combine_bytes(accel_data[2], accel_data[3]);
    z_accel = combine_bytes(accel_data[4], accel_data[5]);
}

// Function to get accelerometer measurements (from X, Y, Z axis) (units of g)
void LIS3DH::get_xyz_measurements_g(float &x_accel_g, float &y_accel_g, float &z_accel_g) {
    uint8_t accel_data[6];
    // Read 6 bytes from the accelerometer starting at OUT_X_L
    read_register(ACCEL_DATA_REG | 0x80, accel_data, 6);  // 0x80 sets auto-increment mode

    // Combine high and low bytes for X, Y, and Z axes
    int16_t x_raw = combine_bytes(accel_data[0], accel_data[1]);
    int16_t y_raw = combine_bytes(accel_data[2], accel_data[3]);
    int16_t z_raw = combine_bytes(accel_data[4], accel_data[5]);

    // Convert raw accelerometer data to g
    x_accel_g = convert_to_g(x_raw, SENSITIVITY);
    y_accel_g = convert_to_g(y_raw, SENSITIVITY);
    z_accel_g = convert_to_g(z_raw, SENSITIVITY);
}


// Function to get accelerometer measurements (from X, Y, Z axis) (units of m/s^2)
void LIS3DH::get_xyz_measurements_mps2(float &x_accel_mps2, float &y_accel_mps2, float &z_accel_mps2) {
    uint8_t accel_data[6];
    // Read 6 bytes from the accelerometer starting at OUT_X_L
    read_register(ACCEL_DATA_REG | 0x80, accel_data, 6);  // 0x80 sets auto-increment mode

    // Combine high and low bytes for X, Y, and Z axes
    int16_t x_raw = combine_bytes(accel_data[0], accel_data[1]);
    int16_t y_raw = combine_bytes(accel_data[2], accel_data[3]);
    int16_t z_raw = combine_bytes(accel_data[4], accel_data[5]);

    // Convert raw accelerometer data to m/s²
    x_accel_mps2 = convert_to_mps2(x_raw, SENSITIVITY);
    y_accel_mps2 = convert_to_mps2(y_raw, SENSITIVITY);
    z_accel_mps2 = convert_to_mps2(z_raw, SENSITIVITY);
}


// Function to convert raw accelerometer data to units of g
float LIS3DH::convert_to_g(int16_t raw_data, float sensitivity) {
    return (raw_data * sensitivity);  // Convert raw data to g
}

// Function to convert raw accelerometer data to m/s²
float LIS3DH::convert_to_mps2(int16_t raw_data, float sensitivity) {
    return convert_to_g(raw_data, sensitivity) * GRAVITY;  // Convert g to m/s²
}

// read register
bool LIS3DH::read_register(uint8_t reg, uint8_t* data, int length) {
    
    // “reg” is the register to read
    // “data” is a pointer to a buffer into which the data will be placed
    // “length” is the number of bytes to read
    // Tell the device which address we want to read
    if (1 != i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, &reg, 1, true)) {
    // You need to pass the pointer to the register address because
    // i2c_write_blocking expects a pointer to a buffer of data.
    log(LogLevel::ERROR, "lis3dh::read_registers: Failed to select register address.");
        return false;
    }
    // Now read the data
    int bytes_read = i2c_read_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, data, length, false);
    if (bytes_read != length) {
    log(LogLevel::ERROR, "lis3dh::read_registers: Failed to read data.");
        return false;
    }
    return true;
}

int16_t LIS3DH::combine_bytes(uint8_t low_byte, uint8_t high_byte) {
    return (int16_t)((high_byte << 8) | low_byte);
}

bool LIS3DH::accel_write_register(uint8_t reg, uint8_t data) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = data;
    int bytes_written = i2c_write_blocking(ACCEL_I2C_INSTANCE, ACCEL_I2C_ADDRESS, buf, 2, false);
    if (bytes_written != 2) {
        log(LogLevel::ERROR, "Failed to write to accelerometer register.");
        return false;
    }
    return true;
}