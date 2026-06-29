Just a few findings:

https://www.berrybase.at/waveshare-esp32-s3-touch-lcd-169-1.69-display-240-280-wi-fi-bluetooth-5-512kb-sram
-- Abmessungen: 41,13mm x 33,13 mm
-- Appears to not have many extra pins

https://www.berrybase.at/waveshare-esp32-s3r8-amoled-touch-display-1-64-240mhz-280x456-16mb-flash-6-achsen-imu-3-7v
https://www.waveshare.com/wiki/ESP32-S3-Touch-AMOLED-1.64#Interfaces
29,12 x 44
-- appears to have some ~10 pins, TODO :: find out if any of the exposed pins are reserved for i.e. display or touch

https://www.adafruit.com/product/815
Adafruit 16-Channel 12-bit PWM/Servo Driver - I2C interface - PCA9685
62.5mm x 25.4mm x 3mm

https://www.adafruit.com/product/3632?srsltid=AfmBOoq7t37u-mDymeDldEOJqLtEoX8ZKdBMDewydao9bexi4C4LOjtf
Adafruit Joy FeatherWing for all Feathers

https://batteryzone.de/collections/ladegerat-fur-18650-batterien/products/18650-battery-charger-cell-phone-diy-portable-battery-case-18650-li-ion-battery-power-bank
3 x 18650 ladegerät

- Single controller:
  - (control display and touch)
  - on/off switch
    - should interrupt the primary power
  - read controls like mini joystick
  - read 3 encoders for motor angles
  - read 1 or 2 orientation sensors (I²C)
  - drive the PWM breakout (I²C)
    - 3 x simplefoc mini

- maybe a 3x 18650 pack (11.1V)
- maybe a battery holder for 3 separate 18650 cells
  - larger capacity
  - faster loading
  - more complexity regarding battery compartment
