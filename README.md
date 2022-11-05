# This project has ended

pleaase checkout aircoookies wled project https://github.com/Aircoookie/WLED
It has all the features of this project, the same architectural approach but is waaay more advanced.

# HomeLED
Project to manage NeoPixel Leds with an ESP8266 and control them by an xamarin App.

# Idea:

* The Arduino Sketch uses SSDP to register it self at the network as iot device. The xamarin apps scans for compatile SSDP Devices in the network.
* Communication is done via URL Parameters
* Arduino Sketch is compatible with Neo Pixel and analog LEDs
* use vars in LEDController.h to modify programs to your needs


Needed Libs:
* AutoConnect https://github.com/Hieromon/AutoConnect
** PageBuilder https://github.com/Hieromon/PageBuilder
** ArduinoJson https://github.com/bblanchon/ArduinoJson
* Adafruit NeoPixel
