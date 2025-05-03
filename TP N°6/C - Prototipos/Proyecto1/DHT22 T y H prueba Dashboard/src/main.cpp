#include <DHT.h>
#include <BluetoothSerial.h>  // Biblioteca para Bluetooth clásico en ESP32

#define DHTPIN 15               // Pin conectado al DHT22 (según JSON)
#define DHTTYPE DHT22           // Tipo de sensor DHT22

#define POT_HUMEDAD 35          // Pin del potenciómetro de humedad (según JSON)
#define POT_TEMPERATURA 34      // Pin del potenciómetro de temperatura (según JSON)

#define LED_HUMEDAD 27          // Pin del LED de humedad (según JSON)
#define LED_TEMPERATURA 25      // Pin del LED de temperatura (según JSON)

DHT dht(DHTPIN, DHTTYPE);
BluetoothSerial SerialBT;  // Objeto para la comunicación Bluetooth

void setup() {
  Serial.begin(115200);      // Serial por USB
  SerialBT.begin("ESP32_Sensor"); // Nombre del dispositivo Bluetooth

  dht.begin();

  pinMode(LED_HUMEDAD, OUTPUT);  // Configuración del pin del LED de humedad
  pinMode(LED_TEMPERATURA, OUTPUT);  // Configuración del pin del LED de temperatura
}

void loop() {
  // Leer potenciómetros
  int potHum = analogRead(POT_HUMEDAD);    // Leer el potenciómetro de humedad
  int potTemp = analogRead(POT_TEMPERATURA);  // Leer el potenciómetro de temperatura

  // Escalar valores
  float humedad = map(potHum, 0, 4095, 0, 100);  // Mapear el valor de humedad a un rango de 0 a 100
  float temperatura = map(potTemp, 0, 4095, 0, 50);  // Mapear el valor de temperatura a un rango de 0 a 50

  // Mostrar por consola
  Serial.print("Humedad: ");
  Serial.print(humedad);
  Serial.print(" % | Temperatura: ");
  Serial.print(temperatura);
  Serial.println(" °C");

  // Enviar a la app Virtuino IoT vía Bluetooth
  SerialBT.print("H:");
  SerialBT.print(humedad);
  SerialBT.print(",T:");
  SerialBT.println(temperatura);

  // Control del LED de humedad
  if (humedad <= 30) {
    digitalWrite(LED_HUMEDAD, LOW);  // LED apagado si humedad es menor o igual a 30%
  } else if (humedad <= 60) {
    digitalWrite(LED_HUMEDAD, HIGH);  // LED encendido si humedad está entre 30% y 60%
  } else {
    digitalWrite(LED_HUMEDAD, HIGH);  // LED parpadeante si humedad es mayor a 60%
    delay(200);
    digitalWrite(LED_HUMEDAD, LOW);
    delay(200);
  }

  // Control del LED de temperatura
  if (temperatura <= 24) {
    digitalWrite(LED_TEMPERATURA, LOW);  // LED apagado si temperatura es menor o igual a 24°C
  } else if (temperatura <= 30) {
    digitalWrite(LED_TEMPERATURA, HIGH);  // LED encendido si temperatura está entre 24°C y 30°C
  } else {
    digitalWrite(LED_TEMPERATURA, HIGH);  // LED parpadeante si temperatura es mayor a 30°C
    delay(200);
    digitalWrite(LED_TEMPERATURA, LOW);
    delay(200);
  }

  delay(100);  // Retraso de 100 ms
}
