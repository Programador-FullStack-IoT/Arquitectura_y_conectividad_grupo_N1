#include <DHT.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define DHTPIN 15
#define DHTTYPE DHT22

#define POT_HUMEDAD 35          // Potenciómetro de humedad
#define POT_TEMPERATURA 34      // Potenciómetro de temperatura

#define LED_HUMEDAD 27
#define LED_TEMPERATURA 25

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LED_HUMEDAD, OUTPUT);
  pinMode(LED_TEMPERATURA, OUTPUT);

  // Iniciar pantalla OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("No se pudo encontrar la pantalla OLED"));
    while (true);
  }
  display.clearDisplay();
}

void loop() {
  // Leer potenciómetros
  int potHum = analogRead(POT_HUMEDAD);        // 0 a 4095
  int potTemp = analogRead(POT_TEMPERATURA);   // 0 a 4095

  // Escalar valores a humedad (0–100%) y temperatura (0–50 °C)
  float humedad = map(potHum, 0, 4095, 0, 100);  // Potenciómetro de humedad mapea entre 0 y 100
  float temperatura = map(potTemp, 0, 4095, 0, 50);  // Potenciómetro de temperatura mapea entre 0 y 50°C

  // Mostrar en consola
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" % | Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  // Actualizar pantalla OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Humedad: ");
  display.print(humedad);
  display.println(" %");
  display.print("Temperatura: ");
  display.print(temperatura);
  display.println(" C");
  display.display();

  // Control del LED de humedad
  if (humedad <= 30) {
    digitalWrite(LED_HUMEDAD, LOW);  // LED apagado si la humedad es baja
  } else if (humedad <= 60) {
    digitalWrite(LED_HUMEDAD, HIGH); // LED encendido si la humedad es media
  } else {
    digitalWrite(LED_HUMEDAD, HIGH); 
    delay(200);
    digitalWrite(LED_HUMEDAD, LOW);
    delay(200);  // Parpadeo si la humedad es alta
  }

  // Control del LED de temperatura
  if (temperatura <= 24) {
    digitalWrite(LED_TEMPERATURA, LOW);  // LED apagado si la temperatura es baja
  } else if (temperatura <= 30) {
    digitalWrite(LED_TEMPERATURA, HIGH); // LED encendido si la temperatura es media
  } else {
    digitalWrite(LED_TEMPERATURA, HIGH); 
    delay(200);
    digitalWrite(LED_TEMPERATURA, LOW);
    delay(200);  // Parpadeo si la temperatura es alta
  }

  delay(100); // Pequeño retardo para estabilidad
}
