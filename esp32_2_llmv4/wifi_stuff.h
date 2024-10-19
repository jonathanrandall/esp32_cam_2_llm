#ifndef HTML_STUFF_H_
#define HTML_STUFF_H_

#include <WiFi.h>
#include "ssid_stuff.h"
#include <HTTPClient.h>


// IP and port of the Python server
String serverName = "http://jonny-ubuntu.local:8000/request_inference";  // Replace with your server IP


IPAddress local_IP(192, 168, 1, 171);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

// AsyncWebServer server(80);


void init_wifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("");
    return;

    // if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
    // {
    //     Serial.println("STA Failed to configure");
    // }
    // WiFi.mode(WIFI_AP_STA); // need this mode to use esp now

    // WiFi.begin(ssid, password);
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     delay(500);
    //     Serial.print(".");
    // }
    // Serial.println("");
    // Serial.println("WiFi connected");

    // Serial.print("Go to: http://");
    // Serial.println(WiFi.localIP());

    
}

void request_inference() {
  if (WiFi.status() == WL_CONNECTED) {  // Check WiFi connection
    HTTPClient http;

    // Specify the target URL
    http.begin(serverName);

    // Send the GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      String response = http.getString();
      Serial.println("Response from server: " + response);  // Should be an empty response
    }
    else {
      Serial.print("Error on sending GET request: ");
      Serial.println(httpResponseCode);
    }

    // Free resources
    http.end();
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

#endif