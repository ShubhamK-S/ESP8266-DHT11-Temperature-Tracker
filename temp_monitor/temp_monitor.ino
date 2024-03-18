#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 

const char* ssid = "WIFI SSID";       // Your WiFi SSID
const char* password = "WIFI password";   // Your WiFi password
const char* serverUrl = "http://your-server-endpoint"; // Your server endpoint

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    return;
  }

  // Print temperature
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Send data to server
  sendTemperatureData(temperature);
}

void sendTemperatureData(float temperature) {
  if (client.connect(serverUrl, 80)) {
    Serial.println("Connected to server");
    String postData = "temperature=" + String(temperature);
    client.println("POST " + String(serverUrl) + " HTTP/1.1");
    client.println("Host: " + String(serverUrl));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + String(postData.length()));
    client.println();
    client.println(postData);
    client.println();

    Serial.println("Data sent to server");
  } else {
    Serial.println("Connection to server failed");
  }

  client.stop();
}
