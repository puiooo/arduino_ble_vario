# arduino_ble_vario
DIY Variometer realized with Arduino Nano 33 BLE Sense

Since FlySkyHy doesn't support LK8EX1 sentence when 'vario' field not set, use PRS-sentence protocol for FlySkyHy.

## Used Arduino libs

[Arduino_LPS22HB](https://www.arduino.cc/en/Reference/Arduino_LPS22HB) for pressure sensor

[Arduino_HTS221](https://www.arduino.cc/en/Reference/ArduinoHTS221) for temperature sensor. (Note that [XCTrack does not support to render temperature values](https://gitlab.com/xcontest-public/xctrack-public/-/issues/317)  for now.)


# Useful readings
<https://gitlab.com/xcontest-public/xctrack-public/-/issues/600>

For PRS protocol sentence: <https://gitlab.com/xcontest-public/xctrack-public/-/issues/763>

checksum generation: <https://en.wikipedia.org/wiki/NMEA_0183#C_implementation_of_checksum_generation>

LK8EX1 standard: <https://github.com/LK8000/LK8000/blob/master/Docs/LK8EX1.txt>

# Credits
[HardwareBLESerial](https://github.com/Uberi/Arduino-HardwareBLESerial) by Uberi with small modifications (thx to @Synder14) for receiveCharacteristic and transmitCharacteristic (see [here](https://gitlab.com/xcontest-public/xctrack-public/-/issues/600#note_541486035).
