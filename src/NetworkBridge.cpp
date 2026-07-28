#include "../include/NetworkBridge.h"

#include <WiFi.h> // the wifi library

void NetworkBridge::init() {
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print("\n.");
    }

    Serial.print("\n Connected to: ");
    Serial.println(WiFi.localIP());
}

bool NetworkBridge::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}