#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int LED_RED = 46;   // Pin del LED rojo
const int LED_GREEN = 17; // Pin del LED verde
const int POT_PIN = 34;   // Pin del potenciómetro

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT_Control"); // Nombre del dispositivo Bluetooth

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);

  Serial.println("El dispositivo está listo para conectarse por Bluetooth.");
}

void loop() {
  // Leer comandos por Bluetooth
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    handleCommand(command);
  }

  // Leer el valor del potenciómetro y enviarlo por Bluetooth
  int potValue = analogRead(POT_PIN);
  float voltage = potValue * (3.3 / 4095.0); // Convertir a voltaje
  SerialBT.printf("Potenciometro: %.2f V\n", voltage);

  delay(500); // Reducir la frecuencia de envío
}

void handleCommand(String command) {
  command.trim(); // Eliminar espacios en blanco

  if (command.equalsIgnoreCase("encender rojo")) {
    digitalWrite(LED_RED, HIGH);
    SerialBT.println("LED rojo encendido");
  } else if (command.equalsIgnoreCase("apagar rojo")) {
    digitalWrite(LED_RED, LOW);
    SerialBT.println("LED rojo apagado");
  } else if (command.equalsIgnoreCase("encender verde")) {
    digitalWrite(LED_GREEN, HIGH);
    SerialBT.println("LED verde encendido");
  } else if (command.equalsIgnoreCase("apagar verde")) {
    digitalWrite(LED_GREEN, LOW);
    SerialBT.println("LED verde apagado");
  } else if (command.equalsIgnoreCase("encender todos")) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, HIGH);
    SerialBT.println("Todos los LEDs encendidos");
  } else if (command.equalsIgnoreCase("apagar todos")) {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, LOW);
    SerialBT.println("Todos los LEDs apagados");
  } else {
    SerialBT.println("Comando no reconocido");
  }
}