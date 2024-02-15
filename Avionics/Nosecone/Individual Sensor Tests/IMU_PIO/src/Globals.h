#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#ifdef DEBUG_MODE
#define DEBUG(val) Serial.print(val)
#define DEBUG_FLUSH() Serial.flush()
#define DEBUGLN(val) Serial.println(val)
#else
#define DEBUG(val)
#define DEBUG_FLUSH()
#define DEBUGLN(val)
#endif

// Main loop has tasks which point to functions to call, including milliseconds until when to read
// struct Task {
//     uint32_t (*taskCall)(void);
//     uint32_t nexttime;
//     bool enabled;
// };


#endif