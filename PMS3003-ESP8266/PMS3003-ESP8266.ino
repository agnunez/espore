// PMS3003 code from https://github.com/sensor-web/arduino-station
// Oct-2017 dded Cloud Server http logging and ESP8266 DeepSleep for large battery life 
// as unatended magnet portable dust meter sensor. https://github.com/agnunez/espore
// Only consumed 816 mAh per day from a 5v power bank (ESP8266 NodeMCU + PMP3003 board & fan)

#include <ESP8266WiFi.h>

#define FAN D6  // Tested with NodeMCU ESP12E 2.0. Can be changed at your risk

unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

//*-- IoT Information
const char* ssid     = "yourwifiSSID";
const char* password = "yourwifipass";
 
const char* host = "192.168.1.XXX";  // cloud server in local linux pc

char buf[50];

void sensor(boolean status){
  if(status){
    digitalWrite(FAN,HIGH);          // awake sensor
    delay(10000);                    // wait 10 sec for stabilization
   } else {
    digitalWrite(FAN,LOW);           // put sensor to sleep
    delay(5000);                     // wait 5 sec for stabilization

  }
}

// Serial only for debug. Delete all serial prints for less consumptiong
void setup() {
  Serial.begin(9600);
  Serial.flush();
  Serial.println("Starting Setup"); 
  Serial.print("Connecting to Wifi: ");
  Serial.print(ssid);

  Serial.print("Connecting to " + *ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected. MyIP: ");
  Serial.println(WiFi.localIP());
  pinMode (FAN, OUTPUT );
  sensor(true);
}


void loop() {
  int index = 0;
  char value;
  char previousValue;
  sensor(true);
  while (Serial.available()) {
    value = Serial.read();
    Serial.print("i: ");
    Serial.print(index, HEX);
    Serial.print("d: ");
    Serial.println(value, HEX);
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      Serial.println("Cannot find the data header.");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm1\": ");
      Serial.print(pm1);
      Serial.print(", ");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(", ");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
    } else if (index > 15) {
      break;
    }
    index++;
  }

    Serial.print("Connect to: ");
    Serial.println(host);
 
    WiFiClient client;
    if (client.connect(host, 80)) 
    {
      String url = "/log.php?sid=1&pm1=";
      url += pm1;
      url += "&pm25=";
      url += pm2_5;
      url += "&pm10=";
      url += pm10;
      url += "&c=";
      url += 
      
      
      Serial.print("Requesting URL for the call: ");
      Serial.println(url);
      
      client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +  "Connection: close\r\n\r\n");
      delay(10); // 10 milisekunden warten
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      Serial.println("Website was called, closing connection");
      Serial.print("Current pm25 is: ");
      Serial.print(pm2_5);
      Serial.println(" ugr/m3 ");
      
    }else{
      //EEPROM.write(4, 1);
      Serial.println("Connection lost");
    }
  
  
  while(Serial.available()) Serial.read();
  Serial.println(" }");
  sensor(false);
  ESP.deepSleep(275 * 1000000);
  //delay(275000);
  sensor(true);
}
