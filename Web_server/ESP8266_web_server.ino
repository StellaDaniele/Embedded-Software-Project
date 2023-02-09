#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define WIDTH 320
#define HEIGHT 240

const char* ssid = "Dan's Hotspot";
const char* password = "ciaociao5";
const int led = D1;

WiFiServer server(80);
volatile static int rgb;

void setup(void) {
  // pinMode(led, OUTPUT);
  // digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
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
}

int t1, t2, t3;

void loop(void) {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client detected.");
    String currentLine = "";
    client.print("<!DOCTYPE html><html><style>table, th, td {border-collapse: collapse;} td {width: 1px; height: 1px;}</style><body>it is working");
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