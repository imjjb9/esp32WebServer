#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
// Defining the WiFi channel speeds up the connection:
#define WIFI_CHANNEL 6

WebServer server(80);

#define NUM_LEDS 8
const int ledPins[NUM_LEDS] = {18, 19, 21, 22, 23, 5, 4, 2};

bool ledStates[NUM_LEDS] = {false, false, false, false, false, false, false, false};

void sendHtml() {
  String response = R"(
    <!DOCTYPE html><html>
      <head>
        <title>ESP32 Web Server Sequencer</title>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
          html { font-family: sans-serif; text-align: center; }
          body { display: inline-flex; flex-direction: column; }
          h1 { margin-bottom: 1.2em; } 
          h2 { margin: 0; }
          div { display: grid; grid-template-columns: 1fr 1fr; grid-template-rows: auto auto; grid-auto-flow: column; grid-gap: 1em; }
          .btn { background-color: #5B5; border: none; color: #fff; padding: 0.5em 1em;
                 font-size: 2em; text-decoration: none }
          .btn.OFF { background-color: #333; }
        </style>
      </head>
            
      <body>
        <h1>ESP32 Web Server Sequencer</h1>

        <div>
          <h2>SEQUENCE 1</h2>
          <a href="/toggle/1" class="btn LED1_TEXT">LED1_TEXT</a>
          <h2>SEQUENCE 2</h2>
          <a href="/toggle/2" class="btn LED2_TEXT">LED2_TEXT</a>
          <h2>SEQUENCE 3</h2>
          <a href="/toggle/3" class="btn LED3_TEXT">LED3_TEXT</a>
          <h2>SEQUENCE 4</h2>
          <a href="/toggle/4" class="btn LED4_TEXT">LED4_TEXT</a>
          <h2>SEQUENCE 5</h2>
          <a href="/toggle/5" class="btn LED5_TEXT">LED5_TEXT</a>
        </div>
      </body>
    </html>
  )";
  
  for(int i = 0; i < NUM_LEDS; i++){
    String replaceText = "LED" + String(i + 1) + "_TEXT";
    response.replace(replaceText, ledStates[i] ? "ON" : "ACTIVATE");
  }
  server.send(200, "text/html", response);
}

void setup(void) {
  Serial.begin(115200);
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(ledPins[i], OUTPUT);
  }
  

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", sendHtml);

  server.on(UriBraces("/toggle/{}"), []() {
    String seq = server.pathArg(0);
    Serial.print("Toggle Sequence #");
    Serial.println(seq);

    switch (seq.toInt()) {
      case 1:
        // Sequence 1: left to right
        for(int i = 0; i < NUM_LEDS; i++){
          digitalWrite(ledPins[i], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
        }
        break;
      case 2:
        for(int i = NUM_LEDS - 1; i >= 0; i--){
          digitalWrite(ledPins[i], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
        }
        break;
      case 3:
        for(int i = 0; i < NUM_LEDS; i += 2){
          digitalWrite(ledPins[i], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
        }
        for(int i = 1; i < NUM_LEDS; i += 2){
          digitalWrite(ledPins[i], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
        }
        break;
      case 4:
        for(int i = 0; i < NUM_LEDS - 1; i += 2){
          digitalWrite(ledPins[i], HIGH);
          digitalWrite(ledPins[i + 1], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
          digitalWrite(ledPins[i + 1], LOW);
        }
        break;
      case 5:
        for(int i = NUM_LEDS - 1; i > 0; i -= 2){
          digitalWrite(ledPins[i], HIGH);
          digitalWrite(ledPins[i - 1], HIGH);
          delay(100);
          digitalWrite(ledPins[i], LOW);
          digitalWrite(ledPins[i - 1], LOW);
        }
        break;
    }
    sendHtml();
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
}
