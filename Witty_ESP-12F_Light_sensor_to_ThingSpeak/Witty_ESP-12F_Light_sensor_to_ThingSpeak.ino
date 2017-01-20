#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>

// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "YUNNBF1UYS1B5IGV";
const char* ssid = "IoTtest";
const char* password = "dr4gut1n";
const char* server = "api.thingspeak.com";

//#define DHTPIN D2
#define DHTPIN 4
#define DHTTYPE DHT11

const int LDR = A0;
const int BLINK = 13;


DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup()
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

void loop()
{

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
