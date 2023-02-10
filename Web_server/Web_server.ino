/*
I am using a LOLIN(WEMSO) D1 mini (clone).
So it should be equivalent with a D1 mini lite (based on the ESP8266)
This code works also with the ESP8266.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#include "ssid_password.hpp"

#define WIDTH 320
#define HEIGHT 240

const char* ssid = SSID;
const char* password = PASSWORD;

const char command[] = { '*', 'R', 'D', 'Y', '*' };
WiFiServer server(80);

void setup(void) {
  ESP.wdtEnable(16000);
  // ESP.wdtDisable();

  Serial.begin(1000000);
  WiFi.begin(ssid, password);
#ifdef DEBUG
  Serial.println("");
#endif
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    ESP.wdtFeed();
    delay(500);
#ifdef DEBUG
    Serial.print(".");
#endif
  }
#ifdef DEBUG
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif

  server.begin();

#ifdef DEBUG
  Serial.println("HTTP server started");
#endif
}

void loop(void) {
  ESP.wdtFeed();
  WiFiClient client = server.available();
  if (client) {
#ifdef DEBUG
    Serial.println("New client detected.");
#endif
    String buffer = "";
    client.print("<!DOCTYPE html><html><head><title>IOT_webserver</title> \
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
      </head><body><canvas id='myCanvas' width='320' height='240'></canvas>");
    int i = 0;
    int x = 0;
    int y = 0;
    buffer.concat("<script>var brightnessValues=[");
    while (1) {
      ESP.wdtFeed();
      while (Serial.available()) {
        ESP.wdtFeed();
        int tmp = Serial.read();
        i++;
        if (i <= 5)
          continue;
        if (y < HEIGHT) {
          if (x < WIDTH) {
            int rgb = tmp & 0xFF;
            buffer.concat(rgb);

            buffer.concat(',');
            x++;
          } else {
            if (x == WIDTH && y == HEIGHT - 1)
              buffer.remove(buffer.length() - 1);
            client.print(buffer);
            buffer = "";
            x = 0;
            y++;
          }
        }
        if (y == HEIGHT) {
          buffer = "";
          buffer += "];";
          buffer += "var canvas = document.getElementById('myCanvas');"
                    "var ctx = canvas.getContext('2d');"
                    "var imageData = ctx.createImageData(320, 240);"
                    "for (var i = 0; i < brightnessValues.length; i++) {"
                    "var brightness = brightnessValues[i];"
                    "var index = i * 4;"
                    "imageData.data[index] = brightness;"
                    "imageData.data[index + 1] = brightness;"
                    "imageData.data[index + 2] = brightness;"
                    "imageData.data[index + 3] = 255;"
                    "}"
                    "ctx.putImageData(imageData, 0, 0);";
          buffer += "</script></body></html>";
          client.print(buffer);
          client.flush();
          client.stop();
        }
      }
    }
  }
}