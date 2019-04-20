/**
 * If you're wondering what monstrosity this is, all the comments are
 * stuff I borrowed from jermbox. This example is just blink but with
 * my own, faster direct port manupulation. The constructor is beefy,
 * because I didn't want to do an enourmous switch-case. I can now 
 * move on with getting the bongos to work like they should.
 */

#include "direct_manip.hpp"

Pin pin(CORE_PIN13_BIT, CORE_PIN13_BITMASK, &CORE_PIN13_PORTREG, 
    &CORE_PIN13_PORTSET, &CORE_PIN13_PORTCLEAR, &CORE_PIN13_PORTTOGGLE, 
    &CORE_PIN13_PINREG, &CORE_PIN13_DDRREG, &CORE_PIN13_CONFIG);

// // Period length is 5 us
// // 72 MHz / 1000000 * 2.5 us = 180
// #define CYCLES_HALF F_CPU / 400000

// // Period length is 5 us
// // 72 MHz / 1000000 * 10 us = 720
// #define CYCLES_TWO F_CPU / 100000

// bool passedAll = false;

// uint32_t fallTime = 0;
// uint32_t buffer = 0;
// uint8_t bitsRead = 0;

void setup()
{
    // cli();
    // pinMode(PIN, INPUT);

    // ARM_DEMCR |= ARM_DEMCR_TRCENA;
    // ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;

    while (!Serial);
    Serial.begin(115200);
    pin.directMode(OUTPUT);
    pin.directWrite(HIGH);
}

void loop()
{
    Serial.println(pin.directRead());
    delay_us(127500);
    pin.directToggle();
    delay_us(127500);
    
//     while (1)
//     {
//         // Wait for falling edge
//         while (digitalReadFast(PIN) == HIGH)
//             ;
//         uint32_t newFallTime = ARM_DWT_CYCCNT;
//         if (newFallTime - fallTime > CYCLES_TWO)
//         {
//             // start of new command
//             buffer = 0;
//             bitsRead = 0;
//         }
//         fallTime = newFallTime;

//         // Wait for rising edge
//         while (digitalReadFast(PIN) == LOW)
//             ;
//         uint32_t cycles = ARM_DWT_CYCCNT - fallTime;
//         buffer = (buffer << 1) | (cycles > CYCLES_HALF ? 0 : 1);
//         bitsRead++;
//         if (bitsRead == 9)
//         {
//             if (buffer & 0x1)
//             {
//                 handleShort(buffer >> 1);
//             }
//         }
//         else if (bitsRead == 25)
//         {
//             if (buffer & 0x1)
//             {
//                 handleLong(buffer >> 1);
//             }
//         }
//     }
// }

// void handleShort(uint32_t buffer)
// {
//     if (buffer == 0x0)
//     {
//         respondWithId();
//     }
//     else if (buffer == 0x41)
//     {
//         respondWithOrigins();
//     }
// }

// void handleLong(uint32_t buffer)
// {
//     // the bottom two bits of the command control rumble.
//     // mask them out cuz we don't care, but for reference:
//     // XXXXXX00, brake off, motor off
//     // XXXXXX01, brake off, motor on
//     // XXXXXX10, brake on, motor off
//     // XXXXXX11, brake on, motor on? idk
//     buffer &= 0xFFFFFFFC;

//     if (buffer == 0x00400000)
//     {
//         respondWithCalibration();
//     }
//     else
//     {
//         // Technically, we should check for the right commands
//         // before responding, but we don't actually know what
//         // all the commands are
//         if (!passedAll)
//         {
//             passedAll = true;
//         }
//         respondWithCalibration();
//     }
// }

// void respondWithId()
// {
//     pinMode(PIN, OUTPUT);
//     digitalWriteFast(PIN, HIGH);

//     // 0x09 0x00 0x00 STOP
//     send0();send0();send0();send0();send1();send0();send0();send1();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send1();

//     pinMode(PIN, INPUT);
// }

// void respondWithOrigins()
// {
//     pinMode(PIN, OUTPUT);
//     digitalWriteFast(PIN, HIGH);

//     // 0x00 0x80 0x80 0x80 0x80 0x80 0x00 0x00 0x02 0x02 STOP
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send1();send0();
//     send0();send0();send0();send0();send0();send0();send1();send0();
//     send1();

//     pinMode(PIN, INPUT);
// }

// void respondWithCalibration()
// {
//     pinMode(PIN, OUTPUT);
//     digitalWriteFast(PIN, HIGH);

//     // 0x00 0x80 0x80 0x80 0x80 0x80 0x00 0x00 STOP
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send1();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send0();send0();send0();send0();send0();send0();send0();send0();
//     send1();

//     pinMode(PIN, INPUT);
}

// // pinMode must be set to OUTPUT
// void send0()
// {
//     digitalWriteFast(PIN, LOW);
//     delayMicroseconds(3);
//     digitalWriteFast(PIN, HIGH);
//     delayMicroseconds(1);
// }

// // pinMode must be set to OUTPUT
// void send1()
// {
//     digitalWriteFast(PIN, LOW);
//     delayMicroseconds(1);
//     digitalWriteFast(PIN, HIGH);
//     delayMicroseconds(3);
// }
