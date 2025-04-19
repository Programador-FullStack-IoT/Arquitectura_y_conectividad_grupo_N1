#include <Arduino.h>
#include "SensorDHT.h"
#include "WifiManager.h"
#include "MQTTManager.h"

#define DHTPIN 15
#define DHTTYPE DHT22
#define LEDPIN 16
#define BUTTONPIN 4
#define POTPIN 34

const char* ssid = "tu_SSID";  // Coloca tu SSID
const char* password = "tu_contraseña";  // Coloca tu contraseña WiFi
const char* mqttServer = "mqtt.server.com";  // Dirección del servidor MQTT
int mqttPort = 1883;

SensorDHT sensor(DHTPIN, DHTTYPE);
WifiManager wifiManager(ssid, password);
WiFiClient espClient;
PubSubClient mqttClient(espClient);
MQTTManager mqttManager(mqttClient, mqttServer, mqttPort);

bool isOn = false;
int lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  ledcAttachPin(LEDPIN, 0);
  ledcSetup(0, 5000, 8);
  sensor.begin();
  
  wifiManager.connect();  // Conectar a WiFi
  mqttManager.connect();  // Conectar a MQTT
}

void loop() {
  mqttClient.loop();  // Asegura la recepción de mensajes

  int buttonState = digitalRead(BUTTONPIN);

  if (buttonState == LOW && lastButtonState == HIGH) {
    isOn = !isOn;
    delay(300);  // Antirebote
  }
  lastButtonState = buttonState;

  if (isOn) {
    float temp = sensor.leerTemperatura();
    int potValue = analogRead(POTPIN);
    int potMapped = map(potValue, 0, 4095, 0, 255);

    if (temp != -999.0) {
      int brightness = map(temp, 20, 40, 0, 255);
      brightness = constrain(brightness, 0, 255);
      brightness = (brightness + potMapped) / 2;

      ledcWrite(0, brightness);
      mqttManager.publish("sensor/temperatura", String(temp).c_str());
      mqttManager.publish("sensor/potenciometro", String(potMapped).c_str());
      mqttManager.publish("sensor/led_pwm", String(brightness).c_str());

      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print("°C | Potenciómetro: ");
      Serial.print(potMapped);
      Serial.print(" | LED PWM: ");
      Serial.println(brightness);
    }
  } else {
    ledcWrite(0, 0);  // Apagar LED
  }
}
