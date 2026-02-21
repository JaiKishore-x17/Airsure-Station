//header guard - It prevents the file from being included multiple times in your program.
#ifndef CONFIG_H
#define CONFIG_H

// WiFi Credentials - put your wifi name and password 
const char* WIFI_SSID     = "YOUR_WIFI_NAME";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Backend API Configuration - fill the server api ---- defines where esp32 collects data from 
const char* SERVER_URL = "http://your-server-ip:5000/api/sensor-data";

// Sensor Pin Configuration - Defines which ESP32 pins sensors are connected to

#define DHT_PIN      4        // GPIO4
#define DHT_TYPE     DHT22

#define MQ135_PIN    34       // Analog pin (GPIO34)
#define GAS_PIN      35       // Additional gas sensor (optional)

// Alert Thresholds - Defines safety limits. Used to trigger alerts.

#define TEMP_THRESHOLD_HIGH      40.0
#define HUMIDITY_THRESHOLD_HIGH  80.0
#define GAS_THRESHOLD_HIGH       300

// Data Send Interval (ms) - Controls how often ESP32 sends data.

#define SEND_INTERVAL 5000

#endif
