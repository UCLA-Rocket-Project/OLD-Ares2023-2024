#ifndef GLOBALS_H
#define GLOBALS_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define SCK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11

// Whether to log specific things
bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;

#endif
