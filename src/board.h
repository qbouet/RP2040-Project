// board.h

#ifndef BOARD_H
#define BOARD_H

// LEDS
#define LED_PIN 14
#define NUMBER_OF_LEDS 12

// Acceleromoter
#define ACCEL_I2C_INSTANCE i2c0
#define ACCEL_SDA_PIN 16
#define ACCEL_SCL_PIN 17
#define ACCEL_MISO 4 // also known as SDO/SA0
#define ACCEL_I2C_BAUD_RATE 400000 // 400kHz
#define ACCEL_I2C_ADDRESS 0b11001 // least significant bit is 1, if SDO/SA0 (ACCEL_MISO) is set to high (0 if set to low)
#define ACCEL_DATA_REG 0x28 // register OUT_X_L
#define WHO_AM_I_REG 0x0F
#define CTRL_REG_1 0x20

#endif // BOARD_H