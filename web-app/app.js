const express = require('express');
const cors = require('cors');
const path = require('path');

const app = express();
const PORT = process.env.PORT || 5000;

// 1. MIDDLEWARE SETUP
app.use(cors()); // Allows your dashboard to talk to this API
app.use(express.json()); // Parses incoming JSON data from the ESP32
app.use(express.static(path.join(__dirname))); // Serves your index.html and style.css

// Temporary data storage (In-memory)
let latestSensorData = {
    temperature: 0,
    humidity: 0,
    air_quality: 0,
    gas_level: 0,
    timestamp: null
};

// 2. RECEIVE DATA ROUTE (ESP32 -> Server)
app.post('/api/sensor-data', (req, res) => {
    const { temperature, humidity, air_quality, gas_level } = req.body;

    // Validate and store the incoming data
    latestSensorData = {
        temperature,
        humidity,
        air_quality,
        gas_level,
        timestamp: new Date().toLocaleString()
    };

    console.log("--- New Data Received ---");
    console.log(`Temp: ${temperature}°C | Gas: ${gas_level}`);

    // Send a success response back to the ESP32
    res.status(200).json({ status: "success", message: "Data logged" });
});

// 3. SEND DATA ROUTE (Server -> Dashboard)
app.get('/api/latest-data', (req, res) => {
    res.json(latestSensorData);
});

// 4. FRONTEND ROUTE
app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});