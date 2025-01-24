#include <ESP8266WiFi.h>

// Replace with your Wi-Fi credentials
const char* ssid = "YOUR SSID NAME";
const char* password = "yOUR PASSWORD ";

// Set LED Pin
#define LED_PIN D1  

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);  // Start with LED OFF

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.print("NodeMCU Web Server IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();  // Listen for incoming clients
  if (!client) return;

  Serial.println("New Client Connected.");
  String request = client.readStringUntil('\r');  // Read HTTP request
  Serial.println(request);
  client.flush();

  // Check for LED ON/OFF commands
  if (request.indexOf("/LED=ON") != -1) {
    digitalWrite(LED_PIN, HIGH);
  } 
  if (request.indexOf("/LED=OFF") != -1) {
    digitalWrite(LED_PIN, LOW);
  }

  // Send HTML Web Page
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println("<html><body>");
  client.println("<h1>NodeMCU LED Control</h1>");
  client.println("<p><a href=\"/LED=ON\"><button>Turn ON</button></a></p>");
  client.println("<p><a href=\"/LED=OFF\"><button>Turn OFF</button></a></p>");
  client.println("</body></html>");
  client.stop();
  Serial.println("Client Disconnected.");
}
