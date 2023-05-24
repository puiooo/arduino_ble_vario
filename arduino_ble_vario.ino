#include <Arduino_LPS22HB.h>
#include <Arduino_HTS221.h>
#include <Adafruit_GPS.h>
#include "HardwareBLESerial.h"

#define DEBUG

#define LK8EX1_MODE
// #define PRS_MODE

#define GPS_MODE


const char NAME[] = "BLE_VARIO";
const int DELAY_MS = 50;

#ifdef LK8EX1_MODE
const String LK8EX1_SENTENCE_BEGIN = String("LK8EX1,");
const String LK8EX1_SENTENCE_VARIO = String(",99999,9999,");
const String LK8EX1_SENTENCE_END = String(",999,");
#endif


HardwareBLESerial & bleSerial = HardwareBLESerial::getInstance();

#ifdef GPS_MODE
#define GPSSerial Serial1
Adafruit_GPS GPS(&GPSSerial);
#endif

void setup() {
  if (!BARO.begin()) {
    Serial.begin(9600);
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }
  if (!HTS.begin()) {
    Serial.begin(9600);
    Serial.println("Failed to initialize temperature sensor!");
    while (1);
  }
  if (!bleSerial.beginAndSetupBLE(NAME)) {
    Serial.begin(9600);
    Serial.println("failed to initialize HardwareBLESerial!");
    while (1);
  }
#ifdef GPS_MODE
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); // turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA); // Request updates on antenna status

#ifdef DEBUG
  GPSSerial.println(PMTK_Q_RELEASE);
#endif
#endif
}

void loop() {
  // this must be called regularly to perform BLE updates
  bleSerial.poll();

  // read the sensor value (Defaults to KILOPASCAL, if unit parameter is not provided )
  float pressure = BARO.readPressure();
  int temperature = HTS.readTemperature();

#ifdef DEBUG
  Serial.print("gathered pressure (kPA): ");
  Serial.println(pressure);
  Serial.print("gathered temperature (°C): ");
  Serial.println(temperature);
#endif

#ifdef LK8EX1_MODE
  pressure = (pressure * 10) * 100; // (kPA  * 10) -> hPA * 100 (example for 1013.25 becomes  101325)
  String str_out = LK8EX1_SENTENCE_BEGIN + String(pressure) + LK8EX1_SENTENCE_VARIO + String(temperature) + LK8EX1_SENTENCE_END;

  uint16_t checksum = calculateChecksum(str_out);

  str_out = "$" + str_out + "*" + String(checksum, HEX);

#ifdef DEBUG
  Serial.println(str_out);
#endif

  char output[str_out.length() + 1];
  str_out.toCharArray(output, str_out.length() + 1);

  bleSerial.println(output);

#endif
#ifdef PRS_MODE
  int finalPressure = (int)(pressure * 1000); // 'PRS <value in Pascal>'
  char messageBuffer[16];
  sprintf(messageBuffer, "PRS %X\r\n", finalPressure); // convert to HEX

#ifdef DEBUG
  Serial.println(messageBuffer);
#endif

  bleSerial.println(messageBuffer);
#endif

#ifdef GPS_MODE
  char c = GPS.read();

  if (!GPS.parse(GPS.lastNMEA())) { // this also sets the newNMEAreceived() flag to false
    Serial.print("rekt");
    return; // we can fail to parse a sentence in which case we should just wait for another
  }

  Serial.print("\nTime: ");
  Serial.print(GPS.hour, DEC); Serial.print(':');
  Serial.print(GPS.minute, DEC); Serial.print(':');
  Serial.print(GPS.seconds, DEC); Serial.print('.');
  Serial.print("Date: ");
  Serial.print(GPS.day, DEC); Serial.print('/');
  Serial.print(GPS.month, DEC); Serial.print("/20");
  Serial.println(GPS.year, DEC);

  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    bleSerial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false

#ifdef DEBUG
    Serial.print('received: ');
    Serial.print(c);
    Serial.print("\n");
#endif
  }
#endif

  // wait to print again
  delay(DELAY_MS);
}

#ifdef LK8EX1_MODE
uint16_t calculateChecksum(String str_out) {
  // Calculating checksum for data string
  uint16_t checksum = 0, bi;
  for (uint8_t ai = 0; ai < str_out.length(); ai++)
  {
    bi = (uint8_t)str_out[ai];
    checksum ^= bi;
  }

  return checksum;
}
#endif
