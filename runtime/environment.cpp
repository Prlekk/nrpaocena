#include "environment.h"

Environment::Environment() {
    parent = nullptr;
    variables = {};
    constants = {};
}

Environment::Environment(Environment* parentENV) {
    const bool global = (parentENV == nullptr);
    parent = parentENV;
    variables = {};
    constants = {};
}

bool Environment::hasVariable(const std::string& varname) const {
    if (this == nullptr) {
        return false;
    }

    if (variables.empty()) {
        return false;
    }

    auto it = variables.find(varname);
    if (it != variables.end()) {
        return true;
    } else {
        return false;
    }
}



RuntimeValue* Environment::declareVariable(const std::string& varname, RuntimeValue* value, bool constant) {
    if (!value) {
        throw std::runtime_error("Cannot declare a variable with a null value");
    }

    if (hasVariable(varname)) {
        throw std::runtime_error("Variable already declared: " + varname);
    }

    this->variables[varname] = value;
    if (constant){
        constants.insert(varname);
    }
    
    return value;
}


RuntimeValue* Environment::assignVariable(const std::string& varname, RuntimeValue* value) {
    Environment* env = this->resolve(varname);

    // Cannot assign a value to a constant
    if(env->constants.find(varname) != env->constants.end()) {
        throw std::runtime_error("Cannot modify constant variable: " + varname);
    }
    env->variables[varname] = value; 

    return value;
}

RuntimeValue* Environment::lookupVariable(const std::string& varname) {
    Environment* env = this->resolve(varname);
    return env->variables.find(varname)->second;
}

Environment* Environment::resolve(std::string varname) {
    if(hasVariable(varname)) {
        return this;
    }
    if(this->parent == nullptr) {
        throw std::runtime_error("Variable not found: " + varname);
    }
    return this->parent->resolve(varname);
}

std::map<std::string, RuntimeValue*> Environment::getVariables() {
    return this->variables;
}

NativeFunctionValue::NativeFunctionValue() {
    type = VALUETYPE_NATIVE_FUNCTION;
}

NativeFunctionValue::NativeFunctionValue(FunctionCall call) {
    type = VALUETYPE_NATIVE_FUNCTION;
    this->call = call;
}

NativeFunctionValue::~NativeFunctionValue() {}

FunctionValue::FunctionValue() {
    type = VALUETYPE_FUNCTION;
}

FunctionValue::FunctionValue(std::string n) {
    type = VALUETYPE_FUNCTION;
    name = n;
}

FunctionValue::FunctionValue(std::string n, std::vector<std::string> p) {
    type = VALUETYPE_FUNCTION;
    name = n;
    parameters = p;
}

FunctionValue::FunctionValue(std::string n, std::vector<std::string> p, Environment* env) {
    type = VALUETYPE_FUNCTION;
    name = n;
    parameters = p;
    declarationENV = env;
}

FunctionValue::FunctionValue(std::string n, std::vector<std::string> p, Environment* env, std::vector<Statement*> b) {
    type = VALUETYPE_FUNCTION;
    name = n;
    parameters = p;
    declarationENV = env;
    body = b;
}

void FunctionValue::toString() {
    std::cout << "\n" << name << "(";
    for(int i = 0; i < parameters.size(); i++) {
        std::cout << parameters[i];
        if(i != parameters.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")" << std::endl;
}

FunctionValue::~FunctionValue() {}

RuntimeValue* MK_NATIVE_FUNCTION(FunctionCall call) {
    return dynamic_cast<RuntimeValue*>(new NativeFunctionValue(call));
}

RuntimeValue* timeFunction(std::vector<RuntimeValue*> args, Environment* env){
    return MK_NUMBER(std::floor(time(nullptr)));
}

Environment* createGlobalEnv() {
    Environment* env = new Environment();

    // Define global variables

    env->declareVariable("true", MK_BOOL(true), true);
    env->declareVariable("false", MK_BOOL(false), true);
    env->declareVariable("null", MK_NULL(), true);

    // Define a native method
    env->declareVariable("izpisi", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            for(auto arg : args) {
                if(arg->getTypeName() == "number") {
                    dynamic_cast<NumberValue*>(arg)->toString();
                }else if(arg->getTypeName() == "boolean") {
                    dynamic_cast<BoolValue*>(arg)->toString();
                }else if(arg->getTypeName() == "function") {
                    dynamic_cast<FunctionValue*>(arg)->toString();
                }else if(arg->getTypeName() == "string") {
                    dynamic_cast<StringValue*>(arg)->toString();
                }
            }
            std::cout << std::endl;
        return MK_NULL(); 
        }
        ), true
    );

    env->declareVariable("cas", MK_NATIVE_FUNCTION(timeFunction), true);

    return env;
}