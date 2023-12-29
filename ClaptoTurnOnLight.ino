#define signalToRelayPin 13
#define sensorPin 5
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

int lastSoundValue;
int soundValue;
long lastNoiseTime = 0;
long currentNoiseTime = 0;
long lastLightChange = 0;
// int relayStatus = HIGH;
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
HTTPClient http;
void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  // pinMode(signalToRelayPin, OUTPUT);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("Viettel_Thay Tam", "31102000");
  if ((WiFiMulti.run() != WL_CONNECTED)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected, IP Address: ");
  Serial.println(WiFi.localIP());
  
  
}

void loop() {
  soundValue = digitalRead(sensorPin);
  currentNoiseTime = millis();
  if (soundValue == 1) { // if there is currently a noise
    if (
      (currentNoiseTime > lastNoiseTime + 200) && // to debounce a sound occurring in more than a loop cycle as a single noise
      (lastSoundValue == 0) &&  // if it was silent before
      (currentNoiseTime < lastNoiseTime + 800) && // if current clap is less than 0.8 seconds after the first clap
      (currentNoiseTime > lastLightChange + 1000) // to avoid taking a third clap as part of a pattern
    ) {
      // relayStatus = !relayStatus;
      // digitalWrite(signalToRelayPin, relayStatus);
      // Serial.println(relayStatus);
      Serial.print("[HTTP] begin...\n");
      http.begin(client, "http://192.168.1.9/cm?cmnd=Power%20TOGGLE"); // this is ip of tasmota device, change it
      http.GET();
      lastLightChange = currentNoiseTime;
     }
     lastNoiseTime = currentNoiseTime;
  }
  lastSoundValue = soundValue;
}
