/*   Witty ESP-12F based IoT node with DHT11 and built-in LDR sensor
 *   Based on DHT11 + ESP8266 example found here:
 *    https://learn.adafruit.com/esp8266-temperature-slash-humidity-webserver/code
 *    
 *   by Mike Mike Barela
 *   
 *   Version 1.0  25/01/2016  Version 1.0    Dragutin Cvetkovic
 */


#include <ESP8266WiFi.h>
#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN  14
#define LDR     A0
#define BUTTON  4
#define RED     15
#define BLUE    12
#define GREEN   13


// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "YUNNBF1UYS1B5IGV";
const char* ssid = "IoTtest";
const char* password = "dr4gut1n";
const char* server = "api.thingspeak.com";

WiFiClient client;

// Initialize DHT sensor 
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01 
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

float h, t, ldr, humidity, temp; // Values read from the sensor

unsigned long previousMillis = 0;        // will store last temp was read
const long interval = 2000;              // interval at which to read sensor

void setup(void) {

    Serial.begin(115200);
    dht.begin();

    WiFi.begin(ssid, password);

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");

    pinMode(LDR, INPUT);
    pinMode(GREEN, OUTPUT);
}

void loop(void) {

//    h = dht.readHumidity();
//    t = dht.readTemperature();
//    if (isnan(h) || isnan(t))
//    {
//        Serial.println("Failed to read from DHT sensor!");
//        //return;
//    }
//
//    ldr = analogRead(LDR);


    getSensors();           // read sensors


    if (client.connect(server,80)) {
        String postStr = apiKey;
        postStr +="&field1=";
        postStr += String(temp);
        postStr +="&field2=";
        postStr += String(humidity);
        postStr += "&field3=";
        postStr += String(ldr);
        postStr += "\r\n\r\n";


        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(postStr.length());
        client.print("\n\n");
        client.print(postStr);

        Serial.print("Temperature: ");
        Serial.println(temp);
        Serial.print("Humidity: ");
        Serial.println(humidity);
        Serial.print("LDR: ");
        Serial.println(ldr);
        Serial.println("Sending data to Thingspeak");
    }
    client.stop();

//    Serial.println("Waiting 20 secs");
// thingspeak needs at least a 15 sec delay between updates
// 20 seconds to be safe
//    delay(20000);

    digitalWrite(GREEN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(GREEN, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second

    
}

void getSensors() {
  // Wait at least 2 seconds seconds between measurements.
  // if the difference between the current time and last time you read
  // the sensor is bigger than the interval you set, read the sensor
  // Works better than delay for things happening elsewhere also
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis >= interval) {
    // save the last time you read the sensor 
    previousMillis = currentMillis;   

    // Reading temperature for humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    Serial.println("Reading humidity");
    humidity = dht.readHumidity();          // Read humidity (percent)

    Serial.println("Reading temperature");
    //temp_f = dht.readTemperature(true);     // Read temperature as Fahrenheit
    temp = dht.readTemperature();           // Read temperature in Celsius
    
    Serial.println("Reading LDR");
    ldr = analogRead(LDR);
    
    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temp)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }


  } else {

    Serial.println("==================== Skipping ====================");
  }
}
