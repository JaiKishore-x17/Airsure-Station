#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "config.h"

// Initialize DHT sensor - sss
DHT dht(DHT_PIN, DHT_TYPE);

unsigned long lastSendTime = 0;

void connectWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  connectWiFi();
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (millis() - lastSendTime > SEND_INTERVAL) {
    lastSendTime = millis();

    // ===============================
    // Read Sensor Data
    // ===============================
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    int mq135Value = analogRead(MQ135_PIN);
    int gasValue = analogRead(GAS_PIN);

    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // ===============================
    // Print Sensor Data
    // ===============================
    Serial.println("----- Sensor Data -----");
    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    Serial.print("MQ135 (Air Quality): ");
    Serial.println(mq135Value);

    Serial.print("Gas Level: ");
    Serial.println(gasValue);

    // ===============================
    // Send Data to Server
    // ===============================
    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;
      http.begin(SERVER_URL);
      http.addHeader("Content-Type", "application/json");

      String jsonData = "{";
      jsonData += "\"temperature\":" + String(temperature) + ",";
      jsonData += "\"humidity\":" + String(humidity) + ",";
      jsonData += "\"air_quality\":" + String(mq135Value) + ",";
      jsonData += "\"gas_level\":" + String(gasValue);
      jsonData += "}";

      int httpResponseCode = http.POST(jsonData);

      if (httpResponseCode > 0) {
        Serial.print("Data sent successfully. Response code: ");
        Serial.println(httpResponseCode);
      } else {
        Serial.print("Error sending data. Code: ");
        Serial.println(httpResponseCode);
      }

      http.end();
    }

    // ===============================
    // Local Alert Check
    // ===============================
    if (temperature > TEMP_THRESHOLD_HIGH) {
      Serial.println("⚠ HIGH TEMPERATURE ALERT!");
    }

    if (humidity > HUMIDITY_THRESHOLD_HIGH) {
      Serial.println("⚠ HIGH HUMIDITY ALERT!");
    }

    if (gasValue > GAS_THRESHOLD_HIGH) {
      Serial.println("⚠ GAS LEVEL CRITICAL!");
    }
  }
}
