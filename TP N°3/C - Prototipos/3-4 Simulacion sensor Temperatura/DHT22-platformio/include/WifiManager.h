#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>

class WifiManager {
  private:
    const char* ssid;
    const char* password;

  public:
    WifiManager(const char* ssid, const char* password);
    void connect();
};

#endif
