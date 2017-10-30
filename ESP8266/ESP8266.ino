// ESP8266 Script for WIFI LED Strip Controller

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "";
const char* password = "";

ESP8266WebServer server(80);

int r = 0;
int g = 0;
int b = 0;

void setup(void) {
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, 0);

  Serial.begin(9600);

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

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

    if (Serial.available()) {
    String data = Serial.readString();

    String command = data.substring(0, data.indexOf(":"));
//  Serial.print("ESP HANDLE COMMAND: ");
//  Serial.println(command);

    if (command == "COLORIS") {
      r = data.substring(data.indexOf("r=") + 2, data.indexOf(",g=")).toInt();
      g = data.substring(data.indexOf("g=") + 2, data.indexOf(",b=")).toInt();
      b = data.substring(data.indexOf("b=") + 2, data.indexOf("\r\n")).toInt();

      Serial.println("ESP GOT COLOR");
    }
  }
}

void handleRoot() {
  digitalWrite(BUILTIN_LED, 1);

  String body ="<!DOCTYPE html><html><head><meta charset=\"utf-8\">";
  body += "<script src=\"https://cdn.rawgit.com/jaames/iro.js/master/dist/iro.min.js\" charset=\"utf-8\"></script>";
  body += "<meta name=\"viewport\" content=\"width=360, initial-scale=1.0\">";
  body += "</head><body>";
  body += "<div id=\"demoWheel\"></div>";
  body += "<script>var demo=new iro.ColorPicker('#demoWheel',{width:360,height:360,markerRadius:8,color:'rgb(" + String(r) + "," + String(g) + "," + String(b) + ")',borderWidth:2,padding:4,anticlockwise:!0,css:{body:{'background-color':'$color'}}});demo.on('color:change',function(o){var o=o.rgb,e=new XMLHttpRequest;e.open('GET','http://192.168.178.104/set?r='+o.r+'&g='+o.g+'&b='+o.b),e.onload=function(){200===e.status?console.log('request ok'):console.log('Request failed. Returned status of '+e.status)},e.send()});</script>";
  body += "</body></html>";
  
  server.send(200, "text/html", body);
  digitalWrite(BUILTIN_LED, 0);
}

void handleSet() {
  digitalWrite(BUILTIN_LED, 1);

  r = server.arg("r").toInt();
  g = server.arg("g").toInt();
  b = server.arg("b").toInt();

  Serial.print("SETCOLOR:");
  Serial.print("r=");
  Serial.print(r);
  Serial.print(",g=");
  Serial.print(g);
  Serial.print(",b=");
  Serial.println(b);

  String body = "<form action = \"/set\" method=\"get\">\n";
  body += "red: <input type=\"number\" name=\"r\" value=\"" + String(r) + "\"><br>\n";
  body += "green: <input type=\"number\" name=\"g\" value=\"" + String(g) + "\"><br>\n";
  body += "blue: <input type=\"number\" name=\"b\" value=\"" + String(b) + "\"><br>\n";
  body += "<input type=\"submit\" value=\"anwenden\">\n";
  body += "</form>";

  server.send(200, "text/html", body);
 
  digitalWrite(BUILTIN_LED, 0);
}

void handleNotFound() {
  digitalWrite(BUILTIN_LED, 1);
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  
  server.send(404, "text/plain", message);
  
  digitalWrite(BUILTIN_LED, 0);
}
