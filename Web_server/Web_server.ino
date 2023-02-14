/*
I am using a LOLIN(WEMSO) D1 mini (clone).
So it should be equivalent with a D1 mini lite (based on the ESP8266)
This code works also with the ESP8266.
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

#include "ssid_password.hpp"
#include "html_js.hpp"

#define WIDTH 320
#define HEIGHT 240
#define BRIGHTNESS_BUFFER_SIZE (320 * 10)

#define DEBUG

const char* ssid = SSID;
const char* password = PASSWORD;

const char command[] = { '*', 'R', 'D', 'Y', '*' };

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup(void) {
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
  server.on("/", handle_client);
  server.begin();

#ifdef DEBUG
  Serial.println("HTTP server started");
#endif

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
#ifdef DEBUG
  Serial.println("webSocket started");
#endif
}


bool new_image = true;
const int bufferSize = BRIGHTNESS_BUFFER_SIZE;
uint8_t brightness_buffer_1[BRIGHTNESS_BUFFER_SIZE];
uint8_t brightness_buffer_2[BRIGHTNESS_BUFFER_SIZE];
bool use_buffer_1 = true;
int pixels = 0;

void loop(void) {
  server.handleClient();
  webSocket.loop();

  if (Serial.available() > 0) {
    if (new_image) {
      new_image = false;
      find_new_image();
    } else {
      if (use_buffer_1) {
        brightness_buffer_1[pixels % BRIGHTNESS_BUFFER_SIZE] = (uint8_t)Serial.read();
        ++pixels;
        if (pixels % BRIGHTNESS_BUFFER_SIZE == 0) {
          webSocket.broadcastBIN(brightness_buffer_1, BRIGHTNESS_BUFFER_SIZE);
          use_buffer_1 = false;
        }
      } else {
        brightness_buffer_2[pixels % BRIGHTNESS_BUFFER_SIZE] = (uint8_t)Serial.read();
        ++pixels;
        if (pixels % BRIGHTNESS_BUFFER_SIZE == 0) {
          webSocket.broadcastBIN(brightness_buffer_2, BRIGHTNESS_BUFFER_SIZE);
          use_buffer_1 = true;
        }
      }
    }
  }
}

void handle_client() {
#ifdef DEBUG
  Serial.println("New client connected to server");
#endif
  server.send(200, "text/html", html);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
#ifdef DEBUG
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected from socket\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected to socket from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      }
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary lenght: %u\n", num, length);
      // Use the data received here
      break;
  }
#endif
}

void find_new_image() {
  int i = 0;
  while (true) {
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
}