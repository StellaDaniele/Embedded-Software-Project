/*
I am using a LOLIN(WEMSO) D1 mini (clone).
So it should be equivalent with a D1 mini lite (based on the ESP8266)
This code works also with the ESP8266.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <stdlib.h>

#include "ssid_password.hpp"

#define WIDTH 320
#define HEIGHT 240

const char* ssid = SSID;
const char* password = PASSWORD;
const int led = D1;
char* bf;
char* rgb_buf;

const char command[] = { '*', 'R', 'D', 'Y', '*' };
WiFiServer server(80);

void setup(void) {
  bf = new char[10];
  rgb_buf = new char[10];
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
      <style>table, th, td {border-collapse: collapse;} td {width: 1px; height: 1px;}\
      </style></head><body><table style=\"width: 320px; height: 240px\">");
    int i = 0;
    int x = 0;
    int y = 0;
    client.print("<tr>");
    while (1) {
      ESP.wdtFeed();
      while (Serial.available()) {
        ESP.wdtFeed();
        int tmp = Serial.read();
        // client.print(tmp);
        // client.print(" ");
        i++;
        if (i < 5)
          continue;
        if (y < HEIGHT) {
          if (x < WIDTH) {
            int rgb = tmp & 0xFF;
            rgb_buf = itoa(rgb, bf, 10);
            //client.println(r);
            buffer += "<td style=\"background-color:rgb(";
            buffer += rgb_buf;
            buffer += ",";
            buffer += rgb_buf;
            buffer += ",";
            buffer += rgb_buf;
            buffer += ")\"></td>";
            client.print(buffer);
            buffer = "";
            x++;
          } else {
            x = 0;
            y++;
            client.print("</tr>");
          }
        }
      }
    }
  }
}