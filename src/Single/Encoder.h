#pragma once

#include "../EncoderBase.h"
#include "attachInterruptEx.h"

namespace EncoderTool
{
    // Simple encoder implementation which reads phase A and B from two digital pins
    class Encoder : public EncoderBase
    {
     public:
        Encoder() = default;
        ~Encoder();

        inline void begin(int pinA, int pinB, CountMode = CountMode::quarter, int inputMode = INPUT_PULLUP);
        inline void begin(int pinA, int pinB, encCallback_t cb, CountMode = CountMode::quarter, int inputMode = INPUT_PULLUP);

     protected:
        int pinA, pinB;
    };

    // Inline implementation ===============================================

    void Encoder::begin(int pinA, int pinB, encCallback_t cb, CountMode countMode, int inputMode)
    {
        begin(pinA, pinB, countMode, inputMode);
        attachCallback(cb);
    }

    void Encoder::begin(int _pinA, int _pinB, CountMode countMode, int inputMode)
    {
        pinA = _pinA;
        pinB = _pinB;
        pinMode(pinA, inputMode);
        pinMode(pinB, inputMode);

        attachInterruptEx(pinA, [this] { update(digitalReadFast(pinA), digitalReadFast(pinB)); }, CHANGE);
        attachInterruptEx(pinB, [this] { update(digitalReadFast(pinA), digitalReadFast(pinB)); }, CHANGE);

        setCountMode(countMode);
        EncoderBase::begin(digitalReadFast(pinA), digitalReadFast(pinB)); // set start state
    }

    Encoder::~Encoder()
    {
        detachInterrupt(pinA);
        detachInterrupt(pinB);
    }

} // namespace EncoderTool