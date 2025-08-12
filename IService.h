#pragma once
#include <Arduino.h>
#include "FeatureSupport.h"
#include <map>
#include <ArduinoJson.h>

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
    
    // Additional constructors for specific numeric types to resolve ambiguity
    ServiceValue(uint32_t value) : _type(ValueType::INT), intValue(static_cast<int>(value)) {}
    ServiceValue(long value) : _type(ValueType::INT), intValue(static_cast<int>(value)) {}
    // Note: time_t constructor removed as it conflicts with long on this platform
    // where time_t is defined as long
    
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
    enum class Type {
        Generic,
        Setup
    };

    /**
     *  Uniq service. This name used, to find service by name inside container.
     *
     *  @return Uniq name
     */
    virtual String name() const = 0;

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
     * Execute a function without arguments.
     *
     * @param fnName The name of the function to execute
     * @return A ServiceValue object that can be converted to different types
     */
    virtual bool execute(String fnName) {
        StaticJsonDocument<1> doc;
        return execute(fnName, doc.to<JsonVariant>());
    }

    /**
     * Whether this service exposes user-configurable settings.
     * SetupManager should check this before calling buildSettingsSchema().
     * Default is false so services without settings don't need to override anything.
     */
    virtual bool hasSettingsSchema() const { return false; }

    /**
     * Build JSON Schema (draft-07) that describes ONLY this service's settings.
     * The caller provides a root `schema` object to be filled. Typical fields:
     *  - "$schema": "http://json-schema.org/draft-07/schema#"
     *  - "$id": a stable identifier for this service (e.g., "spha://" + name())
     *  - "title": human-friendly title (usually `name()`)
     *  - "type": "object"
     *  - "properties": { ... }  // keys for this service only
     *  - "required": [ ... ]     // optional
     *
     * Default implementation creates a skeleton (no properties). Override in
     * derived classes to add your keys under `properties` and `required`.
     */
    virtual void buildSettingsSchema(JsonObject schema) const;

    /**
     *   Service type
     */
    virtual Type type() const { return Type::Generic; }

    /**
     *  Common function which will be called in the loop method, for all registred services.
     */
    virtual void loop() {};

    /**
     * Check if this service blocks the loop
     *
     * @return true if this service blocks the loop, false otherwise
     */
    virtual bool isLoopBlocker() { return false; };

    /**
     * If service has configuration parameters
     * @return true if service something to configure
     */
    virtual bool hasSettingsSchema() const { return false; }

    /**
     *  Show if service is ready.
     */
    bool isReady () { return _ready; }

protected:
    void setReady(bool ready) {
        _ready = ready;
    };

    /**
     * Helper: initialize a minimal JSON Schema skeleton into `schema`.
     * Creates "$schema", "$id", "title", "type":"object" and an empty
     * "properties" object if they are missing.
     */
    void initSchemaSkeleton(JsonObject schema) const;

private:
    bool _ready = false;
};

inline void IService::buildSettingsSchema(JsonObject schema) const {
    // Provide a safe default so existing services don't break if not overridden
    initSchemaSkeleton(schema);
}

inline void IService::initSchemaSkeleton(JsonObject schema) const {
    if (!schema.containsKey("$schema")) {
        schema["$schema"] = "http://json-schema.org/draft-07/schema#";
    }
    if (!schema.containsKey("$id")) {
        schema["$id"] = String("spha://") + name();
    }
    if (!schema.containsKey("title")) {
        schema["title"] = name();
    }
    if (!schema.containsKey("type")) {
        schema["type"] = "object";
    }
    if (!schema.containsKey("properties")) {
        schema.createNestedObject("properties");
    }
}