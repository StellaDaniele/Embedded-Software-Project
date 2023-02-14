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

#define BUFSIZE 19200
// 19200 = 76800/4 (easier buffers management)
char buf1[BUFSIZE];
char buf2[BUFSIZE];

char* pWriteBuf = buf1;
char* pSendBuf = buf2;

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
    client.print("<!DOCTYPE html><html><head><title>IOT_webserver</title> \
      <meta name='viewport' content='width=device-width, initial-scale=1.0'>\
      </head><body><canvas id='myCanvas' width='320' height='240'></canvas><script>var brightnessValues=[");
    int i = 0;
    while (true) {  // Wait for a new image
      ESP.wdtFeed();
      if (Serial.available()) {
        char cc = (char)Serial.read();

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
    i = 0;  // now counts pixels
    while (1) {
      ESP.wdtFeed();
      while (Serial.available()) {
        ESP.wdtFeed();
        if (pWriteBuf - buf1 + BUFSIZE > pSendBuf - buf2) {
          // switch buffers
          char* tmp = pWriteBuf;
          pWriteBuf = pSendBuf;
          pSendBuf = tmp;
        }

        *pWriteBuf++ = ((char)Serial.read()) & 0xFF;
        *pWriteBuf++ = ',';

        if (pWriteBuf - buf1 == BUFSIZE) {
          client.write((uint8_t*)pSendBuf, BUFSIZE);
          pWriteBuf = buf1;
        }
        ++i;

        if (i == 76800) {
          client.print("];"
                       "var canvas = document.getElementById('myCanvas');"
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
                       "ctx.putImageData(imageData, 0, 0);"
                       "</script></body></html>");
          client.flush();
          client.stop();
        }
      }
    }
  }
}