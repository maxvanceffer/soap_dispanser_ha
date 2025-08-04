#pragma once
#include <Arduino.h>
#include <map>
#include <vector>
#include "IService.h"
#include "storage/StorageManager.h"

/**
 * Container Class
 * 
 * A singleton class that holds IService instances.
 * Provides methods to add services and retrieve them by name.
 * 
 * Example usage:
 * 
 * // Add a service
 * MyService* myService = new MyService();
 * Container::getInstance().addService(myService);
 * 
 * // Get a service by name
 * IService* service = Container::getInstance().getService("MyService");
 * if (service != nullptr) {
 *   // Use the service
 * }
 */
class Container {
private:
    // Private constructor to prevent direct instantiation
    Container() {}
    
    // Private copy constructor and assignment operator to prevent duplication
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    
    // Map to store services by name
    std::map<String, IService*> _services;
    
    // StorageManager instance
    StorageManager _storage;

public:
    /**
     * Get the singleton instance of the Container
     * 
     * @return The singleton instance
     */
    static Container& getInstance() {
        static Container instance;
        return instance;
    }
    
    /**
     * Add a service to the container
     * 
     * @param service The service to add
     * @return true if the service was added, false if a service with the same name already exists
     */
    bool addService(IService* service) {
        if (service == nullptr) {
            Serial.println("Container: Cannot add null service");
            return false;
        }
        
        String serviceName = service->name();
        
        // Check if a service with this name already exists
        if (_services.find(serviceName) != _services.end()) {
            Serial.print("Container: Service with name '");
            Serial.print(serviceName);
            Serial.println("' is already registered");
            return false;
        }
        
        // Add the service to the map
        _services[serviceName] = service;
        
        Serial.print("Container: Service '");
        Serial.print(serviceName);
        Serial.println("' registered successfully");
        return true;
    }
    
    /**
     * Get a service by name
     * 
     * @param name The name of the service to retrieve
     * @return The service, or nullptr if no service with the given name exists
     */
    IService* getService(const String& name) {
        auto it = _services.find(name);
        if (it != _services.end()) {
            return it->second;
        }
        
        Serial.print("Container: Service '");
        Serial.print(name);
        Serial.println("' not found");
        return nullptr;
    }
    
    /**
     * Get all registered services
     * 
     * @return A vector containing all registered services
     */
    std::vector<IService*> getAllServices() {
        std::vector<IService*> services;
        for (const auto& pair : _services) {
            services.push_back(pair.second);
        }
        return services;
    }
    
    /**
     * Get the names of all registered services
     * 
     * @return A vector containing the names of all registered services
     */
    std::vector<String> getAllServiceNames() {
        std::vector<String> names;
        for (const auto& pair : _services) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    /**
     * Check if a service with the given name exists
     * 
     * @param name The name of the service to check
     * @return true if a service with the given name exists, false otherwise
     */
    bool hasService(const String& name) {
        return _services.find(name) != _services.end();
    }
    
    /**
     * Remove a service by name
     * 
     * @param name The name of the service to remove
     * @return true if the service was removed, false if no service with the given name exists
     */
    bool removeService(const String& name) {
        auto it = _services.find(name);
        if (it != _services.end()) {
            _services.erase(it);
            Serial.print("Container: Service '");
            Serial.print(name);
            Serial.println("' removed");
            return true;
        }
        
        Serial.print("Container: Cannot remove service '");
        Serial.print(name);
        Serial.println("' - not found");
        return false;
    }
    
    /**
     * Clear all services
     */
    void clear() {
        _services.clear();
        Serial.println("Container: All services cleared");
    }
    
    /**
     * Get the StorageManager instance
     * 
     * @return Reference to the StorageManager instance
     */
    StorageManager& getStorage() {
        return _storage;
    }
};