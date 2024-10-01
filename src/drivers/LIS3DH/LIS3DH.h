// LIS3DH.h (this is declaring the class in a cpp header file)

#ifndef LIS3DH_H
#define LIS3DH_H
#include <cstdint>  // For C++ code (use <stdint.h> for C code), required for "uint8_t"
#include <cstddef> // For size_t



class LIS3DH {
public:
    // Constructor
    LIS3DH();

    // Method to initialise LIS3DH Accelerometer
    void init();

    // Method to read LIS3DH register
    bool read_register(uint8_t, uint8_t*, int);

    bool accel_write_register(uint8_t, uint8_t);

    int16_t combine_bytes(uint8_t, uint8_t);

    // Function to convert raw accelerometer data to units of g
    float convert_to_g(int16_t, float);

    // Function to convert raw accelerometer data to m/s²
    float convert_to_mps2(int16_t, float);

    // Function to get accelerometer measurements (from X, Y, Z axis) (in integers, no units)
    void get_xyz_measurements_raw(int16_t &, int16_t &, int16_t &);

    // Function to get accelerometer measurements (from X, Y, Z axis) (units of g)
    void get_xyz_measurements_g(float &, float &, float &);

    // Function to get accelerometer measurements (from X, Y, Z axis) (units of m/s^2)
    void get_xyz_measurements_mps2(float &, float &, float &);
};

#endif // LIS3DH_H