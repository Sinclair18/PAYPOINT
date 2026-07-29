#include "../include/NetworkBridge.h"

#include <WiFi.h> // the wifi library

void NetworkBridge::init()
{
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.print("\n\nConnected to: ");
    Serial.println(WiFi.localIP());
}

bool NetworkBridge::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

void NetworkBridge::sendTransaction(int amount, String authMethod, int userID)
{
    if (isConnected())
    {
        JsonDocument document;

        document["amount"] = amount;
        document["auth_method"] = authMethod;
        document["user_ID"] = userID;
        
        String jsonPayload;
        serializeJson(document, jsonPayload);

        Serial.print("Sending Json: ");
        Serial.println(jsonPayload);


        HTTPClient http;

        http.begin("http://jsonplaceholder.typicode.com/posts");

        http.setTimeout(10000);

        Serial.println("Preparing to send post request...");

        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonPayload);

        delay(200);

        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);

        http.end();
    }

    else
    {
        Serial.println("Error: Wifi not connected.\n Reconnecting...");

        WiFi.reconnect();
    }
}

void NetworkBridge::testPaystackName()
{
    if (isConnected())
    {
        // 1. Set up a secure client for HTTPS
        WiFiClientSecure secureClient;
        secureClient.setInsecure(); // Bypass SSL certificate validation for testing

        HTTPClient http;

        // 2. The Paystack Resolve URL (Replace with your 10-digit UBA account)
        String url = "https://api.paystack.co/bank/resolve?account_number=2330558457&bank_code=033";

        Serial.println("Asking Paystack for account name...");

        http.begin(secureClient, url);

        // 3. Attach your Secret Key to the header to prove who you are
        http.addHeader("Authorization", paystackKey);

        // 4. Fire the GET request!
        int httpResponseCode = http.GET();

        delay(100); // Power stabilization buffer

        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        // 5. If it worked (200), print the JSON response!
        if (httpResponseCode == 200)
        {
            String response = http.getString();
            Serial.println("--- Paystack Response ---");
            Serial.println(response);
            Serial.println("-------------------------");
        }
        else
        {
            // If it fails, print the error so we can debug it
            Serial.println(http.getString());
        }

        http.end();
    }
}