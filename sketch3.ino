#include <WiFi.h>
#include <WebServer.h>

// WiFi network credentials
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Create instance of server
WebServer server(80);

// LED Pin Array
const int LEDPins[8] = {2, 4, 5, 12, 13, 14, 22, 23};
int moodChoice = 0;

// Piezo Buzzer Pin
const int buzzerPin = 26;

void setup() {
  // Initialize serial and wait for it to open:
  Serial.begin(115200);
  
  // set LED pins to output
  for(int i = 0; i < 8; i++) {
    pinMode(LEDPins[i], OUTPUT);
  }

  // Set piezo buzzer pin as output
  pinMode(buzzerPin, OUTPUT);

  // connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println(WiFi.localIP());

  // Route for root
  server.on("/", [](){
    server.send(200, "text/html", R"(
      <!DOCTYPE html>
      <html>
      <head>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css'>
      </head>
      <body>
        <div class='container'>
          <h1>Web-Controlled LED Mood Light with Ambient Sounds</h1>
          <button class='btn btn-primary' onclick="location.href='/mood1'">Mood 1</button>
          <button class='btn btn-secondary' onclick="location.href='/mood2'">Mood 2</button>
          <button class='btn btn-success' onclick="location.href='/mood3'">Mood 3</button>
          <button class='btn btn-warning' onclick="location.href='/mood4'">Mood 4</button>
        </div>
      </body>
      </html>
    )");
});

  // Route for moods
  for(int i = 1; i <= 4; i++) {
  server.on(("/mood"+String(i)).c_str(), [i](){
    moodChoice = i;
    server.send(200, "text/html", "<!DOCTYPE html><html><head><meta name='viewport' content='width=device-width, initial-scale=1'><link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css'></head><body><div class='container'><h1>Mood " + String(i) + " Selected</h1><button class='btn btn-info' onclick=\"location.href='/'\">Back</button></div></body></html>");
  });
}


  // Start server
  server.begin();
}

void playTone(int frequency, int duration) {
  tone(buzzerPin, frequency, duration);
  delay(duration);
  noTone(buzzerPin);
}

void loop() {
  server.handleClient();
  switch(moodChoice) {
    case 1: // Mood 1
      for(int i = 0; i < 8; i++) {
        digitalWrite(LEDPins[i], HIGH);
        playTone(200, 100);
        digitalWrite(LEDPins[i], LOW);
      }
      break;
    case 2: // Mood 2
      for(int i = 7; i >= 0; i--) {
        digitalWrite(LEDPins[i], HIGH);
        playTone(300, 100);
        digitalWrite(LEDPins[i], LOW);
      }
      break;
    case 3: // Mood 3
      for(int i = 0; i < 8; i+=2) {
        digitalWrite(LEDPins[i], HIGH);
        playTone(400, 100);
        digitalWrite(LEDPins[i], LOW);
      }
      for(int i = 1; i < 8; i+=2) {
        digitalWrite(LEDPins[i], HIGH);
        playTone(400, 100);
        digitalWrite(LEDPins[i], LOW);
      }
      break;
    case 4: // Mood 4
      for(int i = 0; i < 8; i+=3) {
        digitalWrite(LEDPins[i], HIGH);
        playTone(500, 100);
        digitalWrite(LEDPins[i], LOW);
      }
      break;
  }
  delay(1000); // Reset delay
}
