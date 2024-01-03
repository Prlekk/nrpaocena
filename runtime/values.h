#ifndef VALUES_H
#define VALUES_H

#include "../frontend/Functions.h"

#include <map>

enum ValueType {
    VALUETYPE_NULL,
    VALUETYPE_NUMBER,
    VALUETYPE_STRING,
    VALUETYPE_BOOLEAN,
    VALUETYPE_OBJECT,
    VALUETYPE_NATIVE_FUNCTION,
    VALUETYPE_FUNCTION,
};

class NullValue;
class NumberValue;

class RuntimeValue {
  public:
    RuntimeValue();
    RuntimeValue(ValueType t, std::string v);
    std::string getTypeName();
    virtual ~RuntimeValue();
    ValueType type;
    std::string value = "";
};

class NullValue : public RuntimeValue {
  public:
    NullValue();
    virtual ~NullValue();
};

class BoolValue : public RuntimeValue {
  public:
    BoolValue(bool b = true);
    virtual ~BoolValue();
    bool value;
    void toString();
};

class NumberValue : public RuntimeValue {
  public:
    NumberValue(float val = 0.0);
    virtual ~NumberValue();
    float value;
    void toString();
};

class StringValue : public RuntimeValue {
  public:
    StringValue(std::string val = "");
    virtual ~StringValue();
    std::string value;
    void toString();
};

class ObjectValue : public RuntimeValue {
  public:
    ObjectValue();
    ObjectValue(std::map<std::string, RuntimeValue*> p);
    virtual ~ObjectValue();
    std::map<std::string, RuntimeValue*> properties;
};

RuntimeValue* MK_NUMBER(float n = 0.0);
RuntimeValue* MK_BOOL(bool b = true);
RuntimeValue* MK_NULL();

#endif
