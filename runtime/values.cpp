// values.cpp
#include "values.h"

RuntimeValue::RuntimeValue() : type(VALUETYPE_NULL), value("null") {}

RuntimeValue::RuntimeValue(ValueType t, std::string v) : type(t), value(v) {}

std::string RuntimeValue::getTypeName() {
    switch (type) {
        case VALUETYPE_NULL:
            return "null";
        case VALUETYPE_NUMBER:
            return "number";
        case VALUETYPE_BOOLEAN:
            return "boolean";
        case VALUETYPE_OBJECT:
            return "object";
        case VALUETYPE_NATIVE_FUNCTION:
            return "native_function";
        case VALUETYPE_FUNCTION:
            return "function";
        case VALUETYPE_STRING:
            return "string";
        
    }
    return "boolean";
}

RuntimeValue::~RuntimeValue() {}

NullValue::NullValue() {
    type = VALUETYPE_NULL;
    value = "null";
}

NullValue::~NullValue() {}

BoolValue::BoolValue(bool b) : value(b) {
    type = VALUETYPE_BOOLEAN;
    value = b;
}

void BoolValue::toString() {
    std::cout << std::boolalpha << this->value << std::endl;
}

BoolValue::~BoolValue() {}

NumberValue::NumberValue(float val) : value(val) {
    type = VALUETYPE_NUMBER;
    value = val;
}

void NumberValue::toString() {
    std::cout << this->value;
}

NumberValue::~NumberValue() {}

StringValue::StringValue(std::string val) : value(val) {
    type = VALUETYPE_STRING;
    value = val;
}

void StringValue::toString() {
    std::cout << this->value;
}

StringValue::~StringValue() {}

ObjectValue::ObjectValue() {
    type = VALUETYPE_OBJECT;
    properties = std::map<std::string, RuntimeValue*>();
}
ObjectValue::ObjectValue(std::map<std::string, RuntimeValue*> p) {
    properties = p;
}

ObjectValue::~ObjectValue() {};

RuntimeValue* MK_NUMBER(float n) {
    return new NumberValue(n);
}

RuntimeValue* MK_STRING(std::string s) {
    return new StringValue(s);
}

RuntimeValue* MK_BOOL(bool b) {
    return new BoolValue(b);
}

RuntimeValue* MK_NULL() {
    return new NullValue();
}
