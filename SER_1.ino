#include <ESP8266WiFi.h>
#include <Servo.h>

// Replace with your WiFi credentials
const char* ssid = " ";
const char* password = "";

// Create a Servo object
Servo myServo;
#define SERVO_PIN D0  // Connect to GPIO 2 (D4)

WiFiServer server(80);  // Start web server on port 80

void setup() {
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);  // Attach servo to GPIO 2
  myServo.write(90);          // Start at 90 degrees

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi!");
  Serial.print("Web Server IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Listen for client connections
  if (!client) return;

  Serial.println("New Client Connected.");
  String request = client.readStringUntil('\r');  // Read HTTP request
  Serial.println(request);
  client.flush();

  // Check for Servo Control Commands
  if (request.indexOf("/servo=0") != -1) {
    myServo.write(0);
  }
  if (request.indexOf("/servo=90") != -1) {
    myServo.write(90);
  }
  if (request.indexOf("/servo=180") != -1) {
    myServo.write(180);
  }

  // Send Web Page to Client
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html><body>");
  client.println("<h1>NodeMCU Servo Control</h1>");
  client.println("<p><a href=\"/servo=0\"><button>Move to 0°</button></a></p>");
  client.println("<p><a href=\"/servo=90\"><button>Move to 90°</button></a></p>");
  client.println("<p><a href=\"/servo=180\"><button>Move to 180°</button></a></p>");
  client.println("</body></html>");

  client.stop();
  Serial.println("Client Disconnected.");
}
