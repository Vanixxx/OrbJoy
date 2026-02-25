#include "EncoderAdvanced.h"

EncoderAdvanced::EncoderAdvanced(uint8_t pinA, uint8_t pinB)
: enc(pinA, pinB)
{
}

void EncoderAdvanced::attachAxis(int *axisVariable)
{
    axisMode = true;
    axisPtr = axisVariable;
}

bool EncoderAdvanced::update(Joystick_ &joy)
{
    long newPos = enc.read() / 4;
    bool sendUpdate = false;
    if(newPos != oldPos)
    {
        unsigned long now = millis();
        unsigned long dt = now - lastMoveTime;
        lastMoveTime = now;
        sendUpdate = true;

        int multiplier = 1;

        // 🔥 Beschleunigung
        if(dt < 15) multiplier = 4;
        else if(dt < 30) multiplier = 3;
        else if(dt < 60) multiplier = 2;

        int direction = (newPos > oldPos) ? 1 : -1;

        // AXIS MODE
        if(axisMode && axisPtr != nullptr)
        {
            *axisPtr -= direction * multiplier * 5;

            if(*axisPtr < 0) *axisPtr = 0;
            if(*axisPtr > 1023) *axisPtr = 1023;
        }

        oldPos = newPos;
    }
    return sendUpdate;
}