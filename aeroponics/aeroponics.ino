#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <NewPing.h>

#define BLYNK_PRINT Serial
// Blynk credentials
#define BLYNK_TEMPLATE_ID "TMPL2_lozNc5p"
#define BLYNK_TEMPLATE_NAME "Aeroponics"
#define BLYNK_AUTH_TOKEN "ZvkTntJ8D2siPvH1MQcwqDUOB_fiIPeg"

// TODO: Set correct pins
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 200

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// TODO: Set correct pins
#define DHTPIN 2
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);

// TODO: Set correct pins
#define RELAY_PIN 3

BlynkTimer timer;
// Your WiFi credentials.
char ssid[] = "aeroponics";
char pass[] = "aeroponics";

void setup()
{
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();

  pinMode(RELAY_PIN, OUTPUT);
  // function to be called every second
  timer.setInterval(1000L, sendSensorData);
}

void loop()
{
  Blynk.run();
  timer.run();

  float humidity = getHumidity();
  float temp = getTemperature();
  int dist = getDistance();

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("\tTemperature: ");
  Serial.print(temp);
  Serial.print("\tDistance: ");
  Serial.println(dist);
}

// Turn relay on/off via the website/phone app remotely
BLYNK_WRITE(V7) // TODO: Change virtual pin
{
  int val = param.asInt();

  if (val == 1){
    digitalWrite(RELAY_PIN, HIGH);
  }else {
    digitalWrite(RELAY_PIN, LOW);
  }
}

void sendSensorData()
{  
  // TODO: Change virtual pins
  Blynk.virtualWrite(V0, getHumidity());
  Blynk.virtualWrite(V1, getTemperature());
  Blynk.virtualWrite(V2, getDistance());
}

float getHumidity()
{
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return NULL;
  }
  return h;
}

float getTemperature()
{
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return NULL;
  }
  return t;
}

int getDistance()
{
  int dist = sonar.ping_cm();
  return dist;
}


