#include <Arduino_LPS22HB.h>
#include "HardwareBLESerial.h"

#define LK8EX1_MODE
// #define PRS_MODE

const char NAME[] = "BLE_VARIO";
const int DELAY_MS = 50;

#ifdef LK8EX1_MODE
const String LK8EX1_SENTENCE_BEGIN = String("LK8EX1,");
const String LK8EX1_SENTENCE_END = String(",99999,9999,29,999,");
#endif


HardwareBLESerial & bleSerial = HardwareBLESerial::getInstance();

void setup() {
  if (!BARO.begin()) {
    Serial.begin(9600);
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  if (!bleSerial.beginAndSetupBLE(NAME)) {
    Serial.begin(9600);
    while (true) {
      Serial.println("failed to initialize HardwareBLESerial!");
      delay(1000);
    }
  }
}

void loop() {
  // this must be called regularly to perform BLE updates
  bleSerial.poll();

  // read the sensor value (Defaults to KILOPASCAL, if unit parameter is not provided )
  float pressure = BARO.readPressure();

  Serial.print("gathered pressure (kPA): ");
  Serial.println(pressure);


#ifdef LK8EX1_MODE
  pressure = (pressure * 10) * 100; // (kPA  * 10) -> hPA * 100 (example for 1013.25 becomes  101325)

  String str_out = LK8EX1_SENTENCE_BEGIN + String(pressure) + LK8EX1_SENTENCE_END;
  
  uint16_t checksum = calculateChecksum(str_out);

  str_out = "$" + str_out + "*" + String(checksum, HEX);

  Serial.println(str_out);

  char output[str_out.length() + 1];
  str_out.toCharArray(output, str_out.length() + 1);

  bleSerial.println(output);

#endif
#ifdef PRS_MODE
  int finalPressure = (int)(pressure * 1000); // 'PRS <value in Pascal>'
  char messageBuffer[16];
  sprintf(messageBuffer, "PRS %X\r\n", finalPressure); // convert to HEX

  Serial.println(messageBuffer);
  bleSerial.println(messageBuffer);
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
