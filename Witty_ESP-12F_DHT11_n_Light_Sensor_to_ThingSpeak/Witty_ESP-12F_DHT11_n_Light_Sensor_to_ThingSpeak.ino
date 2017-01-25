/*   Witty ESP-12F based IoT node with DHT11 and built-in LDR sensor
 *   Based on DHT11 + ESP8266 example found here:
 *    https://learn.adafruit.com/esp8266-temperature-slash-humidity-webserver/code
 *    
 *   by Mike Mike Barela
 *   
 *   Version 1.0  25/01/2016  Version 1.0    Dragutin Cvetkovic
 */

#include <DHT.h>
#include <ESP8266WiFi.h>

#define LDR     A0
#define BUTTON  4
#define RED     15
#define BLUE    12
#define GREEN   13
#define DHTPIN  14    // Make sure that the PIN used for DHT input is not already used by the Witty Board 

#define DHTTYPE DHT11 // Change to proper DHT type here

// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "YUNNBF1UYS1B5IGV";
const char* ssid = "IoTtest";
const char* password = "dr4gut1n";
const char* server = "api.thingspeak.com";

// Initialize DHT sensor 
// NOTE: For working with a faster than ATmega328p 16 MHz Arduino chip, like an ESP8266,
// you need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// This is for the ESP8266 processor on ESP-01 
DHT dht(DHTPIN, DHTTYPE, 11); // 11 works fine for ESP8266

WiFiClient client;


void setup(void) {
{
    Serial.begin(115200);
    delay(10);
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
    pinMode(BLINK, OUTPUT);
}

void loop(void) {

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t))
    {
        Serial.println("Failed to read from DHT sensor!");
        //return;
    }

    float ldr = analogRead(LDR);





    if (client.connect(server,80)) {
        String postStr = apiKey;
        postStr +="&field1=";
        postStr += String(t);
        postStr +="&field2=";
        postStr += String(h);
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
        Serial.println(t);
        Serial.print("Humidity: ");
        Serial.println(h);
        Serial.print("LDR: ");
        Serial.println(ldr);
        Serial.println("Sending data to Thingspeak");
    }
    client.stop();

    Serial.println("Waiting 20 secs");
// thingspeak needs at least a 15 sec delay between updates
// 20 seconds to be safe
//    delay(20000);

    digitalWrite(BLINK, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(19000);              // wait for a second
    digitalWrite(BLINK, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second

    
}
