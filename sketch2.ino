// Include the necessary libraries for WiFi and web server.
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <uri/UriBraces.h>

// Define WiFi network credentials.
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_CHANNEL 6

// Start the web server on port 80.
WebServer server(80);

// Define the pin numbers for LEDs.
#define NUM_LEDS 8
const int ledPins[NUM_LEDS] = {18, 19, 21, 22, 23, 5, 4, 2};

// Piezo Buzzer Pin
const int buzzerPin = 26;

// Define the HTML content to be sent to the client. It includes buttons to control each LED.
void sendHtml() {
  // The 'R' before the parenthesis makes it a raw string literal, which allows multi-line strings.
  String response = R"(10<!DOCTYPE html>
  <html>
  <head>
  <title>ESP32 Web Server Sequencer</title>
  <!-- Meta tag for responsiveness -->
  <meta name="viewport" content="width=device-width, initial-scale=1">
  
  <!-- Include Bootstrap CSS -->
  <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css">
  
  <!-- Custom Styles -->
  <style>
  .btn-custom {
    background-color: #5B5;
    border: none;
    color: #fff;
    padding: 0.5em 1em;
    font-size: 2em;
    text-decoration: none;
    margin-bottom: 1em;
    }
    .btn-custom.OFF { background-color: #333; }
    </style>
    </head>

    <body class="container">
    <h1 class="text-center mt-4">ESP32 Web Server Sequencer</h1>

    <div class="row">
    <div class="col-12 col-md-6 my-3 text-center">
    <h2>SEQUENCE 1</h2>
    <a href="/toggle/1" class="btn btn-custom LED1_TEXT">ACTIVATE</a>
    </div>
    <div class="col-12 col-md-6 my-3 text-center">
    <h2>SEQUENCE 2</h2>
    <a href="/toggle/2" class="btn btn-custom LED2_TEXT">ACTIVATE</a>
    </div>
    <div class="col-12 col-md-6 my-3 text-center">
    <h2>SEQUENCE 3</h2>
    <a href="/toggle/3" class="btn btn-custom LED3_TEXT">ACTIVATE</a>
    </div>
    <div class="col-12 col-md-6 my-3 text-center">
    <h2>SEQUENCE 4</h2>
    <a href="/toggle/4" class="btn btn-custom LED4_TEXT">ACTIVATE</a>
    </div>
    <div class="col-12 col-md-6 my-3 text-center">
    <h2>SEQUENCE 5</h2>
    <a href="/toggle/5" class="btn btn-custom LED5_TEXT">ACTIVATE</a>
    </div>
    </div>
    </body>
    </html>
    )";
    server.send(200, "text/html", response);
}

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}

void setup(void) {
  // Initialize serial communication and set the mode of each LED pin to OUTPUT.
  Serial.begin(115200);
  for(int i = 0; i < NUM_LEDS; i++){
    pinMode(ledPins[i], OUTPUT);
  }

  // Set piezo buzzer pin as output
  pinMode(buzzerPin, OUTPUT);

  // Connect to the WiFi network.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, WIFI_CHANNEL);
  Serial.print("Connecting to WiFi ");
  Serial.print(WIFI_SSID);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // Define the server routes.
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server.
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
        playTone(200 + i * 50, 100);
        digitalWrite(ledPins[i], LOW);
      }
      break;
      case 2:
      // Sequence 2: right to left
      for(int i = NUM_LEDS - 1; i >= 0; i--){
        digitalWrite(ledPins[i], HIGH);
        playTone(200 + i * 50, 100);
        digitalWrite(ledPins[i], LOW);
      }
      break;
      case 3:
      // Sequence 3: Even to odd
      for(int i = 0; i < NUM_LEDS; i += 2){
        digitalWrite(ledPins[i], HIGH);
        playTone(200 + i * 50, 100);
        digitalWrite(ledPins[i], LOW);
      }
      for(int i = 1; i < NUM_LEDS; i += 2){
        digitalWrite(ledPins[i], HIGH);
        playTone(200 + i * 50, 100);
        digitalWrite(ledPins[i], LOW);
      }
      break;
      case 4:
      // Sequence 4: Pairs from left to right
      for(int i = 0; i < NUM_LEDS - 1; i += 2){
        digitalWrite(ledPins[i], HIGH);
        digitalWrite(ledPins[i + 1], HIGH);
        playTone(200 + i * 50, 100);
        digitalWrite(ledPins[i], LOW);
        digitalWrite(ledPins[i + 1], LOW);
      }
      break;
      case 5:
      // Sequence 5: Pairs from right to left
      for(int i = NUM_LEDS - 1; i > 0; i -= 2){
        digitalWrite(ledPins[i], HIGH);
        digitalWrite(ledPins[i - 1], HIGH);
        playTone(200 + i * 50, 100);
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
  // Let the server handle incoming client requests.
  server.handleClient();
  // Delay between loops to prevent watchdog timer resets.
  delay(2);
}
