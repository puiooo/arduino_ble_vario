# arduino_ble_vario
DIY Variometer realized with Arduino Nano 33 BLE Sense

Since FlySkyHy doesn't support LK8EX1 sentence when 'vario' field not set, use PRS-sentence protocol for FlySkyHy.

## used libs

Arduino_LPS22HB for pressure sensor
Arduino_HTS221 for temperature sensor


# Useful readings
<https://gitlab.com/xcontest-public/xctrack-public/-/issues/600>

For PRS protocol sentence: <https://gitlab.com/xcontest-public/xctrack-public/-/issues/763>

checksum generation: <https://en.wikipedia.org/wiki/NMEA_0183#C_implementation_of_checksum_generation>

LK8EX1 standard: <https://github.com/LK8000/LK8000/blob/master/Docs/LK8EX1.txt>

# Credits
HardwareBLESerial by Uberi https://github.com/Uberi/Arduino-HardwareBLESerial with small modifications
