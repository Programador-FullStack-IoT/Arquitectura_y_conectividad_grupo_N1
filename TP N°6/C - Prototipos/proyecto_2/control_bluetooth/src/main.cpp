#include <Arduino.h>
#include <BluetoothSerial.h>
#define L_PATIO 32
#define L_FRENTE 33

static unsigned long ultimoEnvio = 0;
const unsigned long intervalo_lectura = 5000;

BluetoothSerial controladorBT;


void setup() {
  Serial.begin(115200);
  controladorBT.begin("SKYNET");
  Serial.println("El dispositivo se ha inicializado y esta listo para conectarse");
  pinMode(L_PATIO, OUTPUT);
  digitalWrite(L_PATIO, LOW);
  pinMode(L_FRENTE, OUTPUT);
  digitalWrite(L_FRENTE, LOW);
}

void loop() {
  if (controladorBT.available() > 0){
    String mensaje = controladorBT.readStringUntil('\n'); //lee el strig proveniente del telefono hasta el caracter de nueva linea
    mensaje.trim(); //recorta espacios y saltos 
    Serial.println("Comando Recibido");
    Serial.println(mensaje);

    if (mensaje.equalsIgnoreCase("encender lámpara patio")){
      digitalWrite(L_PATIO, HIGH);
      Serial.println("Lámpara patio encendida");
      }
    else if (mensaje.equalsIgnoreCase("apagar lámpara patio")){
      digitalWrite(L_PATIO, LOW);
      Serial.println("Lámpara patio apagada");
    }
    if (mensaje.equalsIgnoreCase("encender lámpara frente")){
      digitalWrite(L_FRENTE, HIGH);
      Serial.println("Lámpara frente encendida");
      }
    else if (mensaje.equalsIgnoreCase("apagar lámpara frente")){
      digitalWrite(L_FRENTE, LOW);
      Serial.println("Lámpara frente apagada");
    }
    else if (mensaje.equalsIgnoreCase("informar intensidad de luz")){
      int LDR = map(analogRead(34), 0, 4096, 0, 100);
      Serial.print("La intensidad luminica es de: ");
      Serial.print(LDR);
      Serial.println("%");
      controladorBT.println("La intensidad luminica es de: " + String(LDR)+ "%");
    }
    else {
      Serial.println("Comando incorrecto");
    }
    
    if (millis() - ultimoEnvio > intervalo_lectura){
      int LDR = map(analogRead(34), 0, 4095, 0, 100);
      String luz = "La intensidad luminica es de: " + String(LDR) + "%\n";
      controladorBT.println(luz);
      Serial.println("Enviado a app: " + luz);
      ultimoEnvio = millis();
    }
  }
}
