#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

#define DHTPIN 15 //Pin D8 == Sensor Data
#define DHTTYPE DHT11

const char *SSID = "";
const char *PS = "";
const char *MQTT_BROKER = "";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup_wifi()
{
    delay(10);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    WiFi.begin(SSID, PS);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        digitalWrite(12, HIGH); 
        digitalWrite(14, HIGH);
        digitalWrite(16, HIGH);
        delay(500);
        digitalWrite(12, LOW);
        digitalWrite(14, LOW);
        digitalWrite(16, LOW);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void setupLeds(){
    pinMode(12, OUTPUT); //RED
    pinMode(14, OUTPUT); //BLUE
    pinMode(16, OUTPUT); //GREEN
}

void setup()
{

    Serial.begin(115200);
    setup_wifi();
    setupLeds();
    client.setServer(MQTT_BROKER, 1883);
    dht.begin();
    sensor_t sensor;
    delay(500);
    Serial.println("Setting up.");
    delay(1);
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    delayMS = sensor.min_delay / 1000;
}

void reconnect()
{
    while (!client.connected())
    {
        Serial.print("Reconnecting...");
        if (!client.connect("ESP8266Client"))
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds");
            delay(5000);
        }
    }
}

void loop()
{
    
    if (client.connected())
    {
        sensors_event_t event;

        dht.temperature().getEvent(&event);
        if (!isnan(event.temperature))
            client.publish("home/weather/temperature", String(event.temperature).c_str());
            digitalWrite(12, HIGH); 
            delay(1000); 
        
        dht.humidity().getEvent(&event);

        if (!isnan(event.relative_humidity))
            client.publish("home/weather/humidity", String(event.relative_humidity).c_str());
            digitalWrite(14, HIGH);
            delay(1000);
        
        digitalWrite(16, HIGH); 
        delay(1000); 
        digitalWrite(12, LOW);
        digitalWrite(14, LOW);
        digitalWrite(16, LOW);
        client.loop();
        Serial.println("loop");
    }
    else
    {
        reconnect();
    }
    delay(1000);
}
