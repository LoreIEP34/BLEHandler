/**
 * @file BLEHandler.cpp
 * @author  Ignacio Peñaloza
 * @brief  
 * @version 0.1
 * @date 2025-06-23
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include <Arduino.h>
#include "BLEHandler.h"

#include <NimBLEDevice.h>
#include <NimBLEServer.h>
#include <NimBLECharacteristic.h>
#include <NimBLEUtils.h>
#include <NimBLEAdvertising.h>
#include <NimBLEDescriptor.h>


//---------------------- Constructor BLEHandler ----------------------
BLEHandler::BLEHandler(const char* name) 
    : deviceName(name), clientConnected(false), pServer(nullptr) {}

//---------------------- begin ----------------------
void BLEHandler::begin() {
    // Initialize the BLE server only if it hasn't been initialized yet
    if (pServer != nullptr) return;
    
    // Initialize the BLE device with the given name
    NimBLEDevice::init(deviceName);
 
    // Create the BLE server
    pServer = NimBLEDevice::createServer();

    // Set the server callbacks 
    pServer->setCallbacks(new ServerCallbacks(clientConnected));
    Serial.println("[BLEHandler] callbacks set");
}

//---------------------- ServerCallbacks ----------------------

    // if a client connects to the BLE server call this function
void BLEHandler::ServerCallbacks::onConnect(NimBLEServer* pServer) {
    connectedFlag = true; 
    Serial.println("[BLEHandler] OnConnect called, client connected");
}

    // if a client disconnects from the BLE server call this function
void BLEHandler::ServerCallbacks::onDisconnect(NimBLEServer* pServer) {
    connectedFlag = false; 
    Serial.println("[BLEHandler] OnDisconnect called, Client disconnected");
}

//---------------------- isClientConnected ----------------------
bool BLEHandler::isClientConnected() {
    return clientConnected; // Return the current connection status 
}

//---------------------- addService ----------------------
void BLEHandler::addService(const char* serviceUUID) {
    // Check if the server is initialized
    if (!pServer) {
        Serial.println("[BLEHandler] Server not initialized");
        return;
    }

    // Check if the service UUID is valid
    std::string uuidStr(serviceUUID);

    // Check if the service already exists
    if (services.count(uuidStr) > 0) {
        Serial.printf("[BLEHandler] Service %s already exists\n", serviceUUID);
        return;
    }

    // Create the service
    NimBLEService* pService = pServer->createService(serviceUUID);
    services[uuidStr] = pService;
    

    // Add the service UUID to advertising
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(serviceUUID);
}

//---------------------- addCharacteristic ----------------------
void BLEHandler::addCharacteristic(const char* characteristicUUID, const char* serviceUUID) {
    // Check if the server is initialized
    if (!pServer) {
        Serial.println("[BLEHandler] Server not initialized");
        return;
    }

    // Check if the service UUID and characteristic UUID are valid
    std::string serviceStr(serviceUUID);
    std::string characteristicStr(characteristicUUID);

    // Check if the service exists
    if (services.count(serviceStr) == 0) {
        Serial.printf("[BLEHandler] Service %s does not exist\n", serviceUUID);
        return;
    }

    // Check if the characteristic already exists
    if (characteristics.count(characteristicStr) > 0) {
        Serial.printf("[BLEHandler] Characteristic %s already exists\n", characteristicUUID);
        return;
    }

    // Create the characteristic
    NimBLECharacteristic* pCharacteristic = services[serviceStr]->createCharacteristic(
        characteristicUUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::NOTIFY
    );

    pCharacteristic->setValue(0); // Default value, can be changed later

    // Add a descriptor to the characteristic (optional, but recommended)
    characteristics[characteristicStr] = pCharacteristic;
    Serial.printf("[BLEHandler] Characteristic %s added to service %s\n", characteristicUUID, serviceUUID);
    
    // Start the service after adding the characteristic
    services[serviceStr]->start(); 
}

//---------------------- startAdvertising ----------------------
void BLEHandler::startAdvertising() {
    // Check if the server is initialized
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();

    // Check if there are any services to advertise
    for (const auto& pair : services) {
        pAdvertising->addServiceUUID(pair.first);
    }

    // Check if there are any characteristics to advertise
    pAdvertising->setScanResponse(true);
    
    // set name of the device
    pAdvertising->setName(deviceName.c_str());
    
    // start advertising
    pAdvertising->start();
    Serial.println("[BLEHandler] Advertising started (delayed)");
}


//---------------------- notify ----------------------
void BLEHandler::notify(const char* characteristicUUID, const uint8_t* data, size_t length) {
    // Check if the server is initialized
    if (!clientConnected) {
        Serial.println("[BLEHandler] No client connected, cannot notify");
        return;
    }

    // Check if the characteristic UUID is valid
    std::string uuidStr(characteristicUUID);


    // Check if the characteristic exists
    if (characteristics.count(uuidStr) == 0) {
        Serial.printf("[BLEHandler] Characteristic %s does not exist\n", characteristicUUID);
        return;
    }

    // Get the characteristic 
    NimBLECharacteristic* pCharacteristic = characteristics[uuidStr];

    // set the value and notify the client
    pCharacteristic->setValue(data, length);    
    pCharacteristic->notify();
}

void BLEHandler::notify(const char* characteristicUUID, const std::string& value) {
    notify(characteristicUUID, reinterpret_cast<const uint8_t*>(value.data()), value.length());
}

void BLEHandler::notify(const char* characteristicUUID, const char* value) {
    notify(characteristicUUID, std::string(value));
}

void BLEHandler::notify(const char* characteristicUUID, const String& value) {
    notify(characteristicUUID, value.c_str()); 
}

