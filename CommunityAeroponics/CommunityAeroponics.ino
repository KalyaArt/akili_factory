#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <String.h>

#define BLYNK_TEMPLATE_ID "TMPL2gx05hjzT"
#define BLYNK_TEMPLATE_NAME "Community Aeroponics"
#define BLYNK_AUTH_TOKEN "MK5si3ywY4omTNDwmAdihBwTXO6rXOc2"

#define PUMP_PIN 14

BlynkTimer timer;

// These variables are set from blynk.
unsigned long pumpRunTime = 5000; // How long the pump runs for
unsigned long pumpTurnOnInterval = 10000; // How often to turn on the pump

unsigned long lastTimePumpRun = 0;
unsigned long pumpTurnOnTime = 0;

const char* ssid = "akili_factory";
const char* pass = "akili_factory";

bool pumpState = false;

// Gets pump runtime set by user
BLYNK_WRITE(V1)
{
  unsigned long runTime = param.asInt() * 1000UL; // convert from seconds to milliseconds
  pumpRunTime = runTime;
}

// Gets interval at which pump should be turned on
BLYNK_WRITE(V2)
{
  unsigned long interval = param.asInt() * 60UL * 1000UL; // convert from minutes to milliseconds
  pumpTurnOnInterval = interval;
}

// This function sends values
void myTimerEvent()
{
  if (pumpState)
    Blynk.virtualWrite(V0, "PUMP ON");
  else
    Blynk.virtualWrite(V0, "PUMP OFF");
}

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH); // Turn off the pump

  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run();

  unsigned long currentTime = millis();
 
  Serial.print("CurrentTime: ");
  Serial.print(currentTime);
  Serial.print(" pumpTurnOnTime: ");
  Serial.print(pumpTurnOnTime);
  Serial.print("Pump State: ");
  Serial.println(pumpState);

  if (pumpState == false && currentTime - lastTimePumpRun >= pumpTurnOnInterval) {
    
    digitalWrite(PUMP_PIN, LOW);
    Serial.println("Pump On");
    pumpState = true;
    pumpTurnOnTime = currentTime;
    lastTimePumpRun = currentTime;
  }

  if (pumpState == true && (currentTime - pumpTurnOnTime >= pumpRunTime)) {
    // turn off time after an runTime interval
    Serial.println("Pump Off");

    digitalWrite(PUMP_PIN, HIGH);
    pumpState = false;
    lastTimePumpRun = currentTime;
  }

  delay(1000);
}
