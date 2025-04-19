#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTManager {
  private:
    PubSubClient client;
    const char* mqttServer;
    int mqttPort;

  public:
    MQTTManager(PubSubClient& client, const char* mqttServer, int mqttPort);
    void connect();
    void subscribe(const char* topic);
    void publish(const char* topic, const char* message);
};

#endif
