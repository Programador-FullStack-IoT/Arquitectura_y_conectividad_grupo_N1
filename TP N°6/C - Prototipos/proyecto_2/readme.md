
# Informe del Proyecto: Control por Voz y Bluetooth con ESP32
## 1. Introducción  

El presente proyecto consiste en el desarrollo de un sistema de control por voz y Bluetooth entre un microcontrolador ESP32 y un teléfono celular. A través de una aplicación móvil desarrollada en MIT App Inventor, se permite el control de dos salidas digitales (LEDs) y la visualización de la lectura de un sensor de luminosidad (LDR), tanto mediante botones como por comandos de voz.

## 2. Objetivos  

- Establecer comunicación Bluetooth entre un celular Android y un ESP32.

- Permitir el control de dos LEDs desde el celular mediante botones y comandos de voz.

- Leer periódicamente la intensidad de luz medida por un sensor LDR.

- Mostrar el valor del sensor en un dashboard en el celular.

- Integrar un sistema de reconocimiento de voz que permita realizar las mismas acciones sin necesidad de interactuar con los botones.  


## 3. Componentes Utilizados  

- ESP32 DevKit v1 (con conectividad Bluetooth).

- Sensor LDR + resistencia de 10kΩ.

- 2 LEDs + resistencias limitadoras de corriente.

- Teléfono con Android.

- App móvil creada en MIT App Inventor.

- Software PlatformIO para la programación del ESP32.

## 4. Descripción del Sistema  

El ESP32 actúa como servidor Bluetooth y espera comandos enviados desde el celular.

Cada comando recibido puede encender o apagar uno de los LEDs conectados al ESP32.

El ESP32 realiza lecturas periódicas del sensor LDR y envía estos datos por Bluetooth.

La aplicación muestra estos datos en pantalla y permite enviar los comandos por botón o voz.  
  

## 5. Aplicación Móvil  

La app fue diseñada en MIT App Inventor y cuenta con:

Botón para conectarse por Bluetooth al ESP32.

Botones para encender/apagar LED 1 y LED 2.

Visualización del valor leído del sensor LDR (se actualiza automaticamente cada 5 segundos).

Botón de reconocimiento de voz que escucha órdenes como “encender lampara patio” y las transforma en comandos.    
  
**Programa en bloques para dashboard realizado en MIT App Inventor**
  
  ![imagen_programa_bloques](<../proyecto_2/assets/blocks.png>)    

**Captura de pantalla dashboard**      
    
  ![captura_de_dashboard](<../proyecto_2/assets/captura_dash.jpeg>)


## 6. Interfaz por Voz
Se utilizó el componente SpeechRecognizer de App Inventor. Los comandos reconocidos se traducen a cadenas de texto como:

"encender lampara patio"

"apagar lampara patio"

"encender lampara frente"

"apagar  lampara frente"  
  
"informar intensidad de luz"  

Luego se envia por bluetooth cada string para ser procesado por el controlador ESP32.

## 7. Código del ESP32  

Se usó la librería BluetoothSerial para establecer la comunicación.

El ESP32 lee datos de la entrada analógica donde está conectado el LDR.

Se procesan los comandos recibidos por Bluetooth y se controlan los LEDs según el caso.

La lectura del sensor se envía automáticamente cada 5 segundos o a pedido a traves de comando de voz.  
  
### Codigo C++ para el controlador  
  
```cpp  
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

  ```  

## 8. Resultados y Validación
La app logró establecer comunicación estable con el ESP32 vía Bluetooth.

Todos los comandos fueron correctamente interpretados y ejecutados.

Las lecturas del sensor LDR se mostraron correctamente.

El sistema de voz funcionó correctamente bajo condiciones normales de ruido ambiental.

## 9. Conclusiones
El proyecto demuestra una integración efectiva entre hardware (ESP32 y sensores) y software (App Android) para crear una interfaz de control por voz.

Es una base ideal para futuros desarrollos de domótica o automatización accesible.

Se sugiere continuar explorando versiones con conectividad WiFi y almacenamiento en la nube.

## 10. Posibles Mejoras
Incorporar control por WiFi o MQTT.

Almacenamiento de las lecturas en la nube.

Notificaciones por evento.

Mejora en la interfaz visual.

Reconocimiento de voz más flexible con Machine Learning o NLP.


[bloques dash]: <>
[captura dash]: <../../E - Assets/captura_dash.jpeg>