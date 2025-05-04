#include <DHT.h>
#include <BluetoothSerial.h>  

#define DHTPIN 15               // Pin conectado al DHT22 
#define DHTTYPE DHT22           // Tipo de sensor DHT22

#define LED_HUMEDAD 27          // Pin del LED de humedad 
#define LED_TEMPERATURA 25      // Pin del LED de temperatura 

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
  // Leer datos del sensor DHT22
  float humedad = dht.readHumidity();
  float temperatura = dht.readTemperature();

  // Verificar si la lectura fue exitosa
  if (isnan(humedad) || isnan(temperatura)) {
    Serial.println("Error al leer del sensor DHT22");
    return;
  }

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
    digitalWrite(LED_HUMEDAD, LOW);
  } else if (humedad <= 60) {
    digitalWrite(LED_HUMEDAD, HIGH);
  } else {
    digitalWrite(LED_HUMEDAD, HIGH);
    delay(200);
    digitalWrite(LED_HUMEDAD, LOW);
    delay(200);
  }

  // Control del LED de temperatura
  if (temperatura <= 24) {
    digitalWrite(LED_TEMPERATURA, LOW);
  } else if (temperatura <= 30) {
    digitalWrite(LED_TEMPERATURA, HIGH);
  } else {
    digitalWrite(LED_TEMPERATURA, HIGH);
    delay(200);
    digitalWrite(LED_TEMPERATURA, LOW);
    delay(200);
  }

  delay(100);  // Retraso de 100 ms
}
