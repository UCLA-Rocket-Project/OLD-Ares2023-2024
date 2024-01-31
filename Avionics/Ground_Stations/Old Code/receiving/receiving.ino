
// include the library
#include <RadioLib.h>
#define LED 7

#define PACKET_SIZE 75
uint8_t encoded_packet[PACKET_SIZE]; 

// SX1276 has the following connections:
// NSS pin:   10
// DIO0 pin:  2
// RESET pin: 9
// DIO1 pin:  3
SX1276 radio = new Module(10, 2, 9, 3);
// or using RadioShield
// https://github.com/jgromes/RadioShield
//SX1278 radio = RadioShield.ModuleA;
void setup() {
  
  Serial.begin(9600);
  // initialize SX1276 with default settings
  Serial.print(F("[SX1276] Initializing ... "));
  int state = radio.begin(915.0,500.0,12,5,RADIOLIB_SX127X_SYNC_WORD,20,6,0);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
  // some modules have an external RF switch
  // controlled via two pins (RX enable, TX enable)
  // to enable automatic control of the switch,
  // call the following method
  // RX enable:   4
  // TX enable:   5
  radio.setRfSwitchPins(4, 5);

  pinMode(LED, OUTPUT);
}


String decodeMessage(uint8_t* encoded_message, int message_size) {

  String decoded_message = "";
  for (int i = 0; i < message_size/2; i ++) {
    decoded_message += (char)((encoded_message[i] >> 4) + 44);
    decoded_message += (char)((encoded_message[i] & 0x0F) + 44);
  }

  return decoded_message;
}


void loop() {
  Serial.print(F("[SX1276] Waiting for incoming transmission ... "));
  // you can receive data as an Arduino String
  // NOTE: receive() is a blocking method!
  //       See example ReceiveInterrupt for details
  //       on non-blocking reception method.
  String str;
  digitalWrite(LED, HIGH);
  int state = radio.receive(str);
  digitalWrite(LED, LOW);
  // you can also receive data as byte array
  /*
    byte byteArr[8];
    int state = radio.receive(byteArr, 8);
  */
  if (state == RADIOLIB_ERR_NONE) {
    // packet was successfully received
    Serial.println(F("success!"));
    // print the data of the packet
    Serial.println(F("[SX1276] Data: "));
    
//    Serial.println(decodeMessage(encoded_packet, PACKET_SIZE*2));
    
//    Serial.print(F("raw dataa packet: "));
    Serial.println(str);
//    printFormatted(str);

    // blink led for 1s to indicate receiving
//    digitalWrite(LED, HIGH);

//    digitalWrite(LED, LOW);
//    delay(1000);
    
  } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
    // packet was received, but is malformed
    Serial.println(F("CRC error!"));
  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
    delay(700);
}

// this is horribly inefficient but oh well
void printFormatted(String s) {
  String curr_time;
  String temp;
  String pressure;
  String alt;
  String accel_x;
  String accel_y;
  String accel_z;
  String gyro_x;
  String gyro_y;
  String gyro_z;
  String latitude;
  String longitude;
  String SIV;

  String values[13] = {curr_time, temp, pressure, alt, accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z, latitude, longitude, SIV}; 
  int j = 0;
  for (int i = 0; i < 13; i++) {
    if (j >= s.length()) {break;}
    String data;
    while (s[j] != ',') {
      Serial.println(data);
      Serial.println("i = " + i);
      Serial.println("j = " + j);
      data += s[j];
      j++;
    }
    // skip the comma
    j++;
    values[i] = data;
  }
  Serial.println(curr_time);
  Serial.println(temp);
  Serial.println(pressure);
  Serial.println(alt);
  Serial.println(accel_x);
  Serial.println(accel_y);
  Serial.println(accel_z);
  Serial.println(gyro_x);
  Serial.println(gyro_y);
  Serial.println(gyro_z);
  Serial.println(latitude);
  Serial.println(longitude);
  Serial.println(SIV);
}
