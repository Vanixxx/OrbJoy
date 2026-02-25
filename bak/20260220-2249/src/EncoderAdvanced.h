#ifndef ENCODER_ADVANCED_H
#define ENCODER_ADVANCED_H

#include <Arduino.h>
#include <Encoder.h>
#include <Joystick.h>

class EncoderAdvanced {
public:
    EncoderAdvanced(uint8_t pinA, uint8_t pinB);

    void attachAxis(int *axisVariable);

    bool update(Joystick_ &joy);

private:
    Encoder enc;
    long oldPos = 0;

    unsigned long lastMoveTime = 0;

    // Button Mode
    bool buttonMode = false;
    uint8_t btnCW, btnCCW;

    // Axis Mode
    bool axisMode = false;
    int *axisPtr = nullptr;
};

#endif