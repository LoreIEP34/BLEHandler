/** 
 *  @file BLEHandler.h
 *  @author Ignacio Peñaloza
 *  @brief BLEHandler is a class that simplifies the creation and management of BLE services and characteristics.
 *  It provides methods to initialize the BLE server, add services and characteristics, start advertising,
 *  and send notifications to connected clients.
 * 
 *  @version 0.1
 *  @date 2025-06-23
 *  @note This class is designed to be used with the NimBLE library for ESP32.
 * 
*/

#pragma once

#include <stdint.h>
#include <map>
#include <string>
#include <NimBLEDevice.h>
#include <NimBLECharacteristic.h>



class BLEHandler {
    public:

    /**
     * @brief Construct a new BLEHandler object
     * 
     * @param name is the name of the BLE device
     */
    BLEHandler(const char* name);

    /**
     * @brief Initializes the BLE server and sets up the necessary callbacks.
     * This should be called before adding services or characteristics.
     */
    void begin();

    /**
     * @brief Adds a BLE service with the specified UUID.
     *
     * @param serviceUUID is the UUID of the service to be added.
     */
    void addService(const char* serviceUUID);

    /**
     * @brief Adds a BLE characteristic to the specified service.
     *
     * @param characteristicUUID is the UUID of the characteristic to be added.
     * @param serviceUUID is the UUID of the service to which the characteristic belongs.
     */
    void addCharacteristic(const char* characteristicUUID, const char* serviceUUID);
    
    /**
     * @brief Starts advertising the BLE service.
     * This should be called after adding services and characteristics.
     */
    void startAdvertising();

    /**
     * @brief Notifies connected clients with the specified characteristic UUID and data.
     *
     * @param characteristicUUID is the UUID of the characteristic to notify.
     * @param data is a pointer to the data to be sent.
     * @param length is the length of the data in bytes.
     */
    void notify(const char* characteristicUUID, const uint8_t* data, size_t length);
    
    /**
     * @brief overloaded notify function to send a string value.
     */
    void notify(const char* characteristicUUID, const std::string& value);

    /**
     * @brief overloaded notify function to send a const char value.
     */
    void notify(const char* characteristicUUID, const char* value); 

    
    /**
     * @brief overloaded notify function to send an integer value.
     *
     * @param characteristicUUID is the UUID of the characteristic to notify.
     * @param value is the integer value to be sent.
     */
    template<typename T>
    void notify(const char* characteristicUUID, T value); 
    
    /**
     * @brief Checks if a client is connected to the BLE server.
     *
     * @return true if a client is connected, false otherwise.
     */
    bool isClientConnected();
    
    
    private:

    // Helper function to check if a service exists
    std::string deviceName;
    
    // Flag to indicate if a client is connected
    bool clientConnected;

    // Pointer to the BLE server
    NimBLEServer* pServer;

    // Maps to store services and characteristics by their UUIDs
    std::map<std::string, NimBLEService*> services; 
    std::map<std::string, NimBLECharacteristic*> characteristics;

    /** * @brief Callback class for handling BLE server events.
     * This class is used to handle connection and disconnection events.
     */
    class ServerCallbacks : public NimBLEServerCallbacks {
        public:
            /** * @brief Constructor for ServerCallbacks.
             * Initializes the connectedFlag reference.
             *  @param connectedFlag is a reference to a boolean that indicates if a client is connected.
             */
             ServerCallbacks(bool& connectedFlag) : connectedFlag(connectedFlag) {
                Serial.println("[BLEHandler] ServerCallbacks initialized");
            };

            /**
             * @brief Called when a client connects to the BLE server.
             * Sets the connectedFlag to true.
             *
             * @param pServer is a pointer to the NimBLEServer instance.
             */
            void onConnect(NimBLEServer* pServer);
            
            /**
             * @brief Called when a client disconnects from the BLE server.
             * Sets the connectedFlag to false.
             *
             * @param pServer is a pointer to the NimBLEServer instance.
             */
            void onDisconnect(NimBLEServer* pServer);

        private:
            // Reference to a boolean that indicates if a client is connected
            bool& connectedFlag;
            
    };
};

/**
 * @brief   Overloaded notify function to send a value of type T.
 * 
 * @tparam T numeber type (e.g., int, float, etc.)
 * @param characteristicUUID   is the UUID of the characteristic to notify.
 * @param value   is the value to be sent.
 * This function converts the value to a string and calls the notify function with the string value.
 */
template<typename T>
void BLEHandler::notify(const char* characteristicUUID, T value) {
    notify(characteristicUUID, std::to_string(value));
}
