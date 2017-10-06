# espore
Standalone ESP8266 NodeMCU PMS3003 low consumption, http logger of dust particle counter into local wifi server


// PMS3003 code from https://github.com/sensor-web/arduino-station
// Oct-2017 dded Cloud Server http logging and ESP8266 DeepSleep for large battery life 
// as unatended magnet portable dust meter sensor. https://github.com/agnunez/espore
// Only consumed 816 mAh per day from a 5v power bank (ESP8266 NodeMCU + PMP3003 board & fan)

Install log.php in your local server (apache or other php cgi enabled)
Create a MySQL database with:
CREATE TABLE `dust` (
  `id` int(11) NOT NULL,
  `sid` int(11) NOT NULL,
  `pm25` int(11) NOT NULL,
  `timestamp` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP
) ENGINE=MyISAM DEFAULT CHARSET=latin1 COMMENT='Dust sensor raw data';

Edit ino file with your local wifi SSID and PASS key

Connect 5V power to Vin,Gnd pins in NodeMCU
Connect 5V power to PMS3003 pins 1 and 2
Connect RST pin in NodeMCU to D0 pin to awake from sleep
Connect TX pin (5) in PMS3003 to RX pin (RX) in NodeMCU
Connect Enable pin (3) in PMS3003 to D6 in NodeMCU

let it go..

It will log pm2.5 every 5min into MySQL database, you can adapt at wish.

Have fun!

https://github.com/agnunez/espore  (Oct-2017)
