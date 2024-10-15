#define BLYNK_TEMPLATE_ID "TMPL6xy30aNMi"
#define BLYNK_TEMPLATE_NAME "sensorcahaya"
#define BLYNK_AUTH_TOKEN "6qqsIUAC-NJBnvCp6gXrlTlC988781Ti"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// WiFi credentials
char ssid[] = "Fti";
char pass[] = "!@FTI2022&*";

// Constants
const int ledBuiltIn = LED_BUILTIN;
const int Relay = D1;
const int ldrPin = A0;
const int DARKEST_LDR_VALUE = 1024;  // Adjust this based on your LDR
const int LIGHTEST_LDR_VALUE = 600;  // Adjust this based on your LDR

int valueOfV1 = 0;
int isAutoActive = 0;
BlynkTimer timer;
// Function to handle slider changes on Virtual Pin V1
BLYNK_WRITE(V1) {
  valueOfV1 = param.asInt();
  //digitalWrite(ledBuiltIn, valueOfV1 == 1 ? LOW : HIGH);
  digitalWrite(Relay, valueOfV1 == 1 ? LOW : HIGH);
}

// Function to handle changes on Virtual Pin V3
BLYNK_WRITE(V3) {
  isAutoActive = param.asInt();
  //Blynk.virtualWrite(V2, 50);
}



void gauge() {
  int ldrValue = analogRead(ldrPin);
  //int ldrStatus = DARKEST_LDR_VALUE - ldrValue; // Adjust for proper LDR response
  int ldrStatus = ldrValue;  // Adjust for proper LDR response
  float lightPercentage = constrain(float(ldrStatus) / (DARKEST_LDR_VALUE - LIGHTEST_LDR_VALUE) * 100, 0, 100);

  Blynk.virtualWrite(V2, lightPercentage);

  if (isAutoActive == 1) {
    autoMode(lightPercentage);
  }
}

// Auto mode function
void autoMode(float lightPercentage) {
  digitalWrite(Relay, lightPercentage > 50 ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  pinMode(ledBuiltIn, OUTPUT);
  pinMode(ldrPin, INPUT);
  pinMode(Relay, OUTPUT);
  timer.setInterval(300L, gauge);
}

void loop() {
  Blynk.run();
  timer.run();
}
