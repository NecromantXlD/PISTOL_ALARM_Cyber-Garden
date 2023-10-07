# PISTOL_ALARM_Cyber-Garden

The project was developed at the Cyber ​​Garden hackathon in the city of Taganrog 10/06/2023-10/08/2023.
It is an alarm clock that is started via a browser on the esp32 page via IP on the local network.


List of Alarm Clock components (receiving part):
Esp32
Lcd display 16x2
Potentiometer 10 Kohm
Speaker
Humidity and temperature sensor DHT11
Amplifier PAM8403
Photoresistor

List of Alarm Clock components (sending part):
Arduino nano
630nm laser module
7-segment indicator with shift register TM1637
Ultrasonic distance sensor
Krona battery
Momentary button
DC-DC converter

Libraries used:
<TM1637Display.h>
<DHT.h>
<LiquidCrystal.h>
<WiFi.h>
<HTTPClient.h>
<ArduinoJson.h>
<NTPClient.h>
<WiFiUdp.h>


