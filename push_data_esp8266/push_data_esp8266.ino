/*
 * Download the blynk library by Volodymyr Shymanskyy
 * from your library manager
 */
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL2_lozNc5p"
#define BLYNK_TEMPLATE_NAME "Aeroponics"
#define BLYNK_AUTH_TOKEN "ZvkTntJ8D2siPvH1MQcwqDUOB_fiIPeg"

BlynkTimer timer;

// Set password to "" for open networks.
char ssid[] = "akili_factory";
char pass[] = "akili_factory";

// This function sends Arduino's up time every second to Virtual Pin (0).
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, millis() / 1000);
}

void setup()
{
  Serial.begin(9600);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
