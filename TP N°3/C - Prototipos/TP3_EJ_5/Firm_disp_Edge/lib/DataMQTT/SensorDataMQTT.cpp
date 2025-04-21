#include "SensorDataMQTT.h"

SensorDataMQTT* SensorDataMQTT::instancia = nullptr;

SensorDataMQTT::SensorDataMQTT(const char* ssid, const char* password, const char* broker, int port,
                               IPAddress ip, IPAddress gateway, IPAddress subnet, Extractor& extractor)
    : ssid(ssid), password(password), broker(broker), port(port),
      ip(ip), gateway(gateway), subnet(subnet),
      extractor(extractor), client(espClient) {
    instancia = this;
}

void SensorDataMQTT::conectarWiFi() {
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Conectando a WiFi...");
    }
    Serial.println("Conectado a WiFi con IP: " + WiFi.localIP().toString());
}

void SensorDataMQTT::conectarMQTT() {
    client.setServer(broker, port);
    client.setCallback(callbackWrapper);

    while (!client.connected()) {
        Serial.print("Conectando al broker MQTT...");
        if (client.connect("ESP32_Control_CO2")) {
            Serial.println("Conectado!");
            client.subscribe("ambiente/extractor/control");
        } else {
            Serial.print(".");
            delay(2000);
        }
    }
}

void SensorDataMQTT::manejarMQTT() {
    if (!client.connected()) {
        conectarMQTT();
    }
    client.loop();
}

void SensorDataMQTT::publicarLecturas(float co2, float temperatura, float humedad/*, float nivelBateria*/) {
    char buffer[16];

    dtostrf(co2, 1, 2, buffer);
    client.publish("ambiente/co2", buffer);

    dtostrf(temperatura, 1, 2, buffer);
    client.publish("ambiente/temperatura", buffer);

    dtostrf(humedad, 1, 2, buffer);
    client.publish("ambiente/humedad", buffer);

    //dtostrf(nivelBateria, 1, 2, buffer);
    //client.publish("ambiente/bateria", buffer);
}

void SensorDataMQTT::callbackWrapper(char* topic, byte* payload, unsigned int length) {
    if (instancia) {
        instancia->procesarMensaje(topic, payload, length);
    }
}

void SensorDataMQTT::procesarMensaje(char* topic, byte* payload, unsigned int length) {
    String mensaje;
    for (unsigned int i = 0; i < length; i++) {
        mensaje += (char)payload[i];
    }

    Serial.print("Mensaje MQTT recibido en ");
    Serial.print(topic);
    Serial.print(": ");
    Serial.println(mensaje);

    if (String(topic) == "ambiente/extractor/control") {
        if (mensaje == "ON") {
            extractor.encender();
            client.publish("ambiente/extractor/estado", "ON");
        } else if (mensaje == "OFF") {
            extractor.apagar();
            client.publish("ambiente/extractor/estado", "OFF");
        }
    }
}
