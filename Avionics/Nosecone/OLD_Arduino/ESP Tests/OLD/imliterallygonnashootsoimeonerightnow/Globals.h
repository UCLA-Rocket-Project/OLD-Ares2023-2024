#ifndef GLOBALS_H
#define GLOBALS_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define VSCK_PIN 18
#define VMISO_PIN 19
#define MOSI_PIN 23

#define HSCK_PIN 14
#define HMISO_PIN 12
#define HMOSI_PIN 13

#define NUM_DIGITS 7
#define MAXLENGTH 50

// Whether to log specific things
bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;

#endif
