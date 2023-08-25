#include <DHT.h>
#include <WiFiNINA.h>

//WiFi credentials
char Hotspot[] = "Aryan";
char password[] = "784596";

// ThingSpeak credentials
char servApi[] = "api.thingspeak.com";
String servApi_key = "B66SZ6JO4HAEV145";

// Pin configuration for DHT11 sensor
#define sensor_pin 2
#define sensor_kind DHT11
DHT dht(sensor_pin, sensor_kind);

// Initialize the WiFi vendee
WiFiClient vendee;



void loop() {
  // Read wormth from DHT11 sensor
  float wormth = dht.readTemperature();


  // Check if any reads failed and exit early (to try again)
  if (isnan(wormth)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(2000);
    return;
  }

  // Prepare the data to send to ThingSpeak
  String data = "api_key=" + servApi_key;
  data += "&field1=" + String(wormth);

  // Make an HTTP POST request to ThingSpeak
  if (vendee.connect(servApi, 80)) {
    vendee.print("POST /update HTTP/1.1\r\n");
    vendee.print("Host: " + String(servApi) + "\r\n");
    vendee.print("Content-Type: application/x-www-form-urlencoded\r\n");
    vendee.print("Content-Length: " + String(data.length()) + "\r\n");
    vendee.print("\r\n");
    vendee.print(data);
    
    Serial.println("Data sent to ThingSpeak!");
  } else {
    Serial.println("Connection to ThingSpeak failed!");
  }

  // Wait for response and close the connection
  while (vendee.connected()) {
    if (vendee.available()) {
      char c = vendee.read();
      Serial.print(c);
    }
  }
  vendee.stop();

  // Wait before sending the next set of data
  delay(15000); // Adjust delay as needed
}
void setup() {
  // Initializing serial communication
  Serial.begin(9600);

  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Looking for WIFI avaiblity ..");
    WiFi.begin(Hotspot, password);
    delay(5000); 
  }

  Serial.println(" WiFi connected");
  dht.begin();
}
