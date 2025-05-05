#include <OneWire.h>
#include <DallasTemperature.h>
#include <BluetoothSerial.h>  


// Here the input pin is declared to which the sensor module is connected
#define sensor_temperatura_PIN 4
#define LED_TEMPERATURA 22
#define LED_EXTRACTOR 23
BluetoothSerial SerialBT;  // Objeto para la comunicación Bluetooth

// Libraries are configured
OneWire oneWire(sensor_temperatura_PIN);          
DallasTemperature sensor_t(&oneWire);    


void setup() {

    // Initialize serial output
    Serial.begin(115200);
    SerialBT.begin("ESP32_Sensor"); // Nombre del dispositivo Bluetooth
    pinMode(LED_EXTRACTOR, OUTPUT);  // Configuración del pin del LED de extractor
    pinMode(LED_TEMPERATURA, OUTPUT);  // Configuración del pin del LED de temperatura
    Serial.println("Sensor DS18B20");   //nombre del dispositivo sensor

    sensor_t.begin();      //Se inicializa el sensor.
}

//main program loop
void loop()
{
    // Inicio de medicion de temperatura.
    sensor_t.requestTemperatures();
    // ... se asigna la medicion a una variable.
    float temperatura = sensor_t.getTempCByIndex(0);
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");
    // Se manda dato por bluetooth.
    SerialBT.print(",T:");
    SerialBT.println(temperatura);
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
    if (SerialBT.available() > 0){
      String mensaje = SerialBT.readStringUntil('\n'); //lee el strig proveniente del telefono hasta el caracter de nueva linea
      mensaje.trim(); //recorta espacios y saltos 
      Serial.println("Comando Recibido");
      Serial.println(mensaje);
  
      if (mensaje.equalsIgnoreCase("encender extractor")){
        digitalWrite(LED_EXTRACTOR, HIGH);
        Serial.println("Extractor encendido");
        }
      else if (mensaje.equalsIgnoreCase("apagar extractor")){
        digitalWrite(LED_EXTRACTOR, LOW);
        Serial.println("Extractor apagado");
      }
      else {
        Serial.println("Comando incorrecto");
      }
        delay(1000); // 1s pause until next measurement
    }
}

