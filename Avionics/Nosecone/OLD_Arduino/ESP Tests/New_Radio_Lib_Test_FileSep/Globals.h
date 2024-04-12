#ifndef GLOBALS_H
#define GLOBALS_H

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Includes ----------------------------------------------------------------------//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define VSCK_PIN 18
#define VMISO_PIN 19
#define VMOSI_PIN 23

#define HSCK_PIN 14
#define HMISO_PIN 36
#define HOSI_PIN 13

#define NUM_DIGITS 7
#define MAXLENGTH 50


// Whether to log specific things
bool reading_GPS = 1;
bool reading_IMU = 1;
bool reading_BMP = 1;
bool do_transmit = 1;
bool do_log = 1;
bool print_values = 1;

String downlink_packet;
//uint8_t encoded_packet[MAXLENGTH]; 
uint8_t SD_packet;


void encodeMessage(String message, uint8_t* new_message, int message_size) {

  for (int i = 0; i < message_size; i += 2) {
    if (MAXLENGTH - i == 1) {
      new_message[i/2] = (uint8_t)(message[i] - 44) << 4;
    }
    else {
      new_message[i/2] = ((uint8_t)(message[i] - 44) << 4) + (uint8_t)(message[i+1] - 44);
    }
  }
}

String decodeMessage(uint8_t* encoded_message, int message_size) {

  String decoded_message = "";
  for (int i = 0; i < message_size/2; i ++) {
    decoded_message += (char)((encoded_message[i] >> 4) + 44);
    decoded_message += (char)((encoded_message[i] & 0x0F) + 44);
  }

  return decoded_message;
}


// Encode double
unsigned int truncate(long coordinate) {
  unsigned int t = 0;
  unsigned int digits[NUM_DIGITS];
  for (int i = NUM_DIGITS - 1; i >= 0; i--) {
    if (coordinate < 0) {
      digits[i] = -(coordinate % 10);
    }
    else {
      digits[i] = coordinate % 10;
    }
    coordinate /= 10;
  }

  for (int i = 0; i < NUM_DIGITS; i++) {
    t = (t * 10) + digits[i];
  }

  return t;
}


#endif
