#pragma once
#include <Arduino.h>
#include "FeatureSupport.h"
#include <map>

/**
 * ServiceValue Class
 * 
 * A helper class that can store values of different types (bool, int, float, String)
 * and provide type-specific getter methods.
 */
class ServiceValue {
private:
    enum class ValueType {
        NONE,
        BOOL,
        INT,
        FLOAT,
        STRING
    };
    
    ValueType _type = ValueType::NONE;
    union {
        bool boolValue;
        int intValue;
        float floatValue;
    };
    String stringValue;

public:
    // Default constructor
    ServiceValue() : _type(ValueType::NONE) {}
    
    // Constructors for different types
    ServiceValue(bool value) : _type(ValueType::BOOL), boolValue(value) {}
    ServiceValue(int value) : _type(ValueType::INT), intValue(value) {}
    ServiceValue(float value) : _type(ValueType::FLOAT), floatValue(value) {}
    ServiceValue(const String& value) : _type(ValueType::STRING), stringValue(value) {}
    ServiceValue(const char* value) : _type(ValueType::STRING), stringValue(value) {}
    
    // Type checking methods
    bool isBool() const { return _type == ValueType::BOOL; }
    bool isInt() const { return _type == ValueType::INT; }
    bool isFloat() const { return _type == ValueType::FLOAT; }
    bool isString() const { return _type == ValueType::STRING; }
    bool isNull() const { return _type == ValueType::NONE; }
    
    // Getter methods with type conversion
    bool asBool() const {
        switch (_type) {
            case ValueType::BOOL: return boolValue;
            case ValueType::INT: return intValue != 0;
            case ValueType::FLOAT: return floatValue != 0.0f;
            case ValueType::STRING: return stringValue.length() > 0 && stringValue != "0" && stringValue != "false";
            default: return false;
        }
    }
    
    int asInt() const {
        switch (_type) {
            case ValueType::BOOL: return boolValue ? 1 : 0;
            case ValueType::INT: return intValue;
            case ValueType::FLOAT: return (int)floatValue;
            case ValueType::STRING: return stringValue.toInt();
            default: return 0;
        }
    }
    
    float asFloat() const {
        switch (_type) {
            case ValueType::BOOL: return boolValue ? 1.0f : 0.0f;
            case ValueType::INT: return (float)intValue;
            case ValueType::FLOAT: return floatValue;
            case ValueType::STRING: return stringValue.toFloat();
            default: return 0.0f;
        }
    }
    
    String asString() const {
        switch (_type) {
            case ValueType::BOOL: return boolValue ? "true" : "false";
            case ValueType::INT: return String(intValue);
            case ValueType::FLOAT: return String(floatValue);
            case ValueType::STRING: return stringValue;
            default: return "";
        }
    }
};

/**
 * IService Class
 * 
 * A base class for services that inherits from FeatureSupport and provides
 * a getValue method that can return values of different types.
 * 
 * Example usage:
 * 
 * class MyService : public IService {
 *   public:
 *     MyService() {
 *       // Add features
 *       addSupportFeature("feature1", true);
 *       
 *       // Add values
 *       setValue("boolValue", true);
 *       setValue("intValue", 42);
 *       setValue("floatValue", 3.14f);
 *       setValue("stringValue", "Hello, world!");
 *     }
 * };
 * 
 * // Usage:
 * MyService service;
 * bool b = service.getValue("boolValue").asBool();
 * int i = service.getValue("intValue").asInt();
 * float f = service.getValue("floatValue").asFloat();
 * String s = service.getValue("stringValue").asString();
 */
class IService : public FeatureSupport {
public:
    /**
     * Get current value of class property. For SoapMotor, can be isDispens
     * 
     * @param key The key to look up
     * @return A ServiceValue object that can be converted to different types
     */
    virtual ServiceValue getValue(String key) const = 0;

    /**
     * Execute a function with the given name and arguments
     *
     * @param fnName The name of the function to execute
     * @param args JSON object containing the function arguments (can be any JSON type)
     * @return A ServiceValue object that can be converted to different types
     */
    virtual bool execute(String fnName, JsonVariant args) = 0;

    /**
     *  Show if service is ready.
     */
    bool isReady () {
        return _ready;
    }

protected:
    void setReady(bool ready) {
        _ready = ready;
    };

private:
    bool _ready = false;
};