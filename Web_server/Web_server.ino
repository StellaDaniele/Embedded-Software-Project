#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SoftwareSerial.h>

#include "ssid_password.hpp"

#define WIDTH 320
#define HEIGHT 240

const char* ssid = SSID;
const char* password = PASSWORD;
const int led = D1;

char command[] = { '*', 'R', 'D', 'Y', '*' };

WiFiServer server(80);
volatile static int rgb;

SoftwareSerial ser(2, 3);  // RX, TX

void setup(void) {
  ESP.wdtEnable(16000);
  ESP.wdtDisable();
  // pinMode(led, OUTPUT);
  // digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    ESP.wdtFeed();
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


  server.begin();
  Serial.println("HTTP server started");

  ser.begin(1000000);
}

int t1, t2, t3;

void loop(void) {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client detected.");
    String currentLine = "";
    client.print("<!DOCTYPE html><html><style>table, th, td {border-collapse: collapse;} td {width: 1px; height: 1px;}</style><body>");
    while (1) {
      ESP.wdtFeed();
      int i = 0;
      while (true) {
        ESP.wdtFeed();
        // Wait until we receive *RDY*, which marks the start
        // of an image
        int valid = false;
        i = 0;

        while (true) {
          ESP.wdtFeed();
          if (ser.available()) {
            char cc = (char)ser.read();

            if (command[i] == cc) {
              i++;
            } else {
              i = 0;
            }
            if (i >= sizeof(command)) {
              break;
            }
          }
        }

        // Clear read buffer
        while (ser.available()) {
          ESP.wdtFeed();
          ser.read();
        }

        int tmp = 0;

        // Read the image - just a list of bytes, column by column
        // Each byte represents the brightness 0 - 255

        for (int y = 0; y < HEIGHT; y++) {
          for (int x = 0; x < WIDTH; x++) {
            tmp = (int)ser.read();
            int r = tmp & 0xFF;
            int g = tmp & 0xFF;
            int b = tmp & 0xFF;
            client.println(r);
          }
        }
      }
    }

    /*
    client.print("<table style=\"width: 320px; height: 240px\">");
    //int buffer = 0;

    for (int x = 0; x < HEIGHT; x++) {
      client.print("<tr>");
      for (int y = 0; y < WIDTH; y++) {
        ////////// TODO!!
        /////////file >> buffer;
        // doesn't really matter, just use serial afterwards
        while (Serial.available() <= 3)
          ;
        // read the incoming byte:
        t1 = Serial.read();
        t2 = Serial.read();
        if(t2==' ')
        t3 = Serial.read();
        int _ = Serial.read();
        t1 -= '0';
        t2 -= '0';
        t3 -= '0';
        rgb = t1 * 100 + t2 * 10 + t3;
        Serial.print("I received: ");
        Serial.println(rgb);

        client.print("<td style=\"background-color:rgb(");
        client.print(rgb);
        client.print(",");
        client.print(rgb);
        client.print(",");
        client.print(rgb);
        client.print(")\">");
        client.print("</td>");
      }
      client.print("</tr>");
    }
    client.print("</table>\n</body>\n</html>\n");
    */
    client.println();
    client.stop();
  }
}