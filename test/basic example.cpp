/**
 * @file basic example.cpp
 * @author Ignacio Peñaloza 
 * @brief  is a  basic example of how to use the BLEHandler class to create a BLE service and send notifications.
 * @version 0.1
 * @date 2025-06-23
 * 
 */

#include <Arduino.h> // Include Arduino core library 
#include "BLEHandler.h" // Include the BLEHandler class header

// Define UUIDs for the service and characteristic
#define UUID_SERVICE "12345678-1234-5678-1234-56789abcdef0"
#define UUID_CHARACTERISTIC "12345678-1234-5678-1234-56789abcdef1"

// Create an instance of BLEHandler with the device name
BLEHandler ble("ESP32Actividad");

// Define macros for current time and timeout
#define CURRENT_TIME millis() 
#define TIMEOUT 1000 // 1 second timeout
unsigned long lastCheck = 0; // Variable to store the last notification time
bool flag = false;


//  ---------- Setup function to initialize the BLE service and characteristics  ----------
void setup() {
    // Initialize the Serial communication and give time for it to start
    Serial.begin(115200);
    while (!Serial) {delay(10);}// Wait for Serial to be ready
    delay(1000); 
    
    // Initialize the BLEHandler instance
    ble.begin();
    
    //  Add a service to the BLEHandler instance
    ble.addService(UUID_SERVICE);

    // Add a characteristic to the service
    ble.addCharacteristic(UUID_CHARACTERISTIC, UUID_SERVICE);
    
    // Start advertising the BLE service
    ble.startAdvertising();

    // Wait for a client to connect
    while (!ble.isClientConnected()) {
        Serial.println("Waiting for client to connect...");
        delay(1000); // Wait for a second before checking again
    }
    Serial.println("Client connected, ready to send notifications.");
}

//  ---------- simulate a simple notification mechanism  ----------
void loop() {
    // Check if the current time exceeds the last check time plus the timeout
    if (CURRENT_TIME - lastCheck >= TIMEOUT) {
        lastCheck = CURRENT_TIME; // Update the last notification time

        // send a notification alternating between two messages
        if (!flag) {
            
            // If the flag is false, send the first message and set the flag to true
            flag = true;
            Serial.println("First notification sent.");

            // Send a notification with the first message
            ble.notify(UUID_CHARACTERISTIC, "hola mundo");


        } else {

            // If the flag is true, send the second message and set the flag to false
            flag = false;
            Serial.println("Second notification sent.");

            // Send a notification with the second message
            ble.notify(UUID_CHARACTERISTIC, "adios mundo");
        }

        // Print the current time to the Serial monitor
        Serial.println("Notification sent at: " + String(CURRENT_TIME));
    }
}