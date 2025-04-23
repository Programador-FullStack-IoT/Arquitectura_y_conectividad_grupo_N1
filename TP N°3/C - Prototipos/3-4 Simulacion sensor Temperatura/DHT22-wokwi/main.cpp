#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define LEDPIN 16
#define BUTTONPIN 4
#define POTPIN 34

// Clase SensorDHT integrada directamente en el archivo main
class SensorDHT {
  private:
    DHT dht;
  
  public:
    SensorDHT(uint8_t pin, uint8_t type) : dht(pin, type) {}

    void begin() {
      dht.begin();
    }

    float leerTemperatura() {
      float t = dht.readTemperature();
      if (isnan(t)) {
        return -999.0; // Indicar que hubo un error de lectura
      }
      return t;
    }
};

// Instanciación de la clase SensorDHT
SensorDHT sensor(DHTPIN, DHTTYPE);

bool isOn = false;
int lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(LEDPIN, OUTPUT);
  pinMode(BUTTONPIN, INPUT_PULLUP);
  sensor.begin();
}

void loop() {
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

      // Reemplazamos ledcWrite por analogWrite para hacer que funcione en simulación
      analogWrite(LEDPIN, brightness);

      Serial.print("Temp: ");
      Serial.print(temp);
      Serial.print("°C | Potenciómetro: ");
      Serial.print(potMapped);
      Serial.print(" | LED PWM: ");
      Serial.println(brightness);
    }
  } else {
    analogWrite(LEDPIN, 0);  // Apagar LED
  }
}
