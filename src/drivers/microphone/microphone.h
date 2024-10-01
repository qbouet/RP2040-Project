#ifndef MICROPHONE_H
#define MICROPHONE_H
#include <cstdint>  // For C++ code (use <stdint.h> for C code), required for "uint8_t"

class MICROPHONE {
public:
    // Constructor
    MICROPHONE();

    // Method to initialise microphone
    void init();

    // Method to read from microphone
    void read(uint16_t*, unsigned int);

};

#endif // MICROPHONE_H
