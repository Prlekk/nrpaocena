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

/**
 * Check if the environment has a specific variable
 * @param varname - the name of the variable to check
 * @return true if the variable exists, false otherwise
 */
bool Environment::hasVariable(const std::string& varname) const {
    if (this == nullptr) {
        return false; // Environment is null, so it doesn't have the variable
    }

    if (variables.empty()) {
        return false; // Environment doesn't have any variables
    }

    auto it = variables.find(varname);
    if (it != variables.end()) {
        return true; // Variable found in the environment
    } else {
        return false; // Variable not found in the environment
    }
}



/**
 * Declare a new variable in the environment
 * 
 * @param varname - The name of the variable
 * @param value - The value to be assigned to the variable
 * @param constant - Indicates whether the variable is constant
 * @return The runtime value of the declared variable
 * @throws std::runtime_error if the value is null or if the variable is already declared
 */
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


/**
 * Assigns a value to the specified variable in the environment.
 * 
 * @param varname The name of the variable to assign the value to.
 * @param value The value to assign to the variable.
 * @return The assigned value.
 */
RuntimeValue* Environment::assignVariable(const std::string& varname, RuntimeValue* value) {
    Environment* env = this->resolve(varname);

    // Cannot assign a value to a constant
    if(env->constants.find(varname) != env->constants.end()) {
        throw std::runtime_error("Cannot modify constant variable: " + varname);
    }
    env->variables[varname] = value; 

    return value;
}

/**
 * Looks up a variable in the environment.
 * 
 * @param varname - The name of the variable to look up.
 * @return A pointer to the RuntimeValue associated with the variable.
 */
RuntimeValue* Environment::lookupVariable(const std::string& varname) {
    Environment* env = this->resolve(varname); // Resolve the variable in the environment
    return env->variables.find(varname)->second; // Return the value associated with the variable
}

RuntimeValue* Environment::lookupOrMutateObject(MemberExpression* expr, RuntimeValue* value, Iden* property) {
    if(expr->object->getKind() == NODE_MEMBEREXPRESSION) {
        return lookupOrMutateObject(dynamic_cast<MemberExpression*>(expr->object), value, dynamic_cast<Iden*>(expr->property));
    }

    std::string varname = dynamic_cast<Iden*>(expr->object)->value;
    Environment* env = this->resolve(varname);

    ObjectValue* pastVal = dynamic_cast<ObjectValue*>(env->variables[varname]);

    std::string prop = (property != nullptr) ? property->value : dynamic_cast<Iden*>(expr->property)->value;
    std::string currentProp = dynamic_cast<Iden*>(expr->property)->value;

    if(value != nullptr) {
        pastVal->properties[currentProp] = value;
    }

    if(currentProp != "") {
        pastVal = dynamic_cast<ObjectValue*>(pastVal->properties[currentProp]);
    }

    return pastVal;
}

// Resolves the variable by checking if it exists in the current environment.
// If the variable exists, returns the current environment.
// If the variable does not exist, checks the parent environment recursively.
// Throws a runtime_error if the variable is not found in the entire environment hierarchy.
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

/**
 * Convert the function value to a string representation
 */
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

// Create a new RuntimeValue object by casting a NativeFunctionValue object
// Parameters:
// - call: FunctionCall object representing the function call
// Returns:
// - Pointer to the newly created RuntimeValue object
RuntimeValue* MK_NATIVE_FUNCTION(FunctionCall call) {
    return dynamic_cast<RuntimeValue*>(new NativeFunctionValue(call));
}

// This function takes a vector of RuntimeValue pointers and an Environment pointer as arguments
// It returns a RuntimeValue pointer
RuntimeValue* timeFunction(std::vector<RuntimeValue*> args, Environment* env){
    // Return a new RuntimeValue pointer with the floor value of the current time

    std::time_t currentTimeInSeconds = std::time(nullptr);

    std::tm* timeInfo = std::localtime(&currentTimeInSeconds);

    int year = timeInfo->tm_year + 1900;
    int month = timeInfo->tm_mon + 1;
    int day = timeInfo->tm_mday;
    int hour = timeInfo->tm_hour;
    int minute = timeInfo->tm_min;
    int second = timeInfo->tm_sec;

    std::string date = std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year);

    std::string currentHour = (hour < 10) ? "0" + std::to_string(hour) : std::to_string(hour);
    std::string currentMinute = (minute < 10) ? "0" + std::to_string(minute) : std::to_string(minute);
    std::string currentSecond = (second < 10) ? "0" + std::to_string(second) : std::to_string(second);

    std::string currentTime = currentHour + ":" + currentMinute + ":" + currentSecond;

    return MK_STRING(currentTime + " :: " + date);
}

// Function to create a global environment
Environment* createGlobalEnv() {
    Environment* env = new Environment();

    // Define global variables
    env->declareVariable("true", MK_BOOL(true), true);
    env->declareVariable("false", MK_BOOL(false), true);
    env->declareVariable("null", MK_NULL(), true);

    // Define a native method for printing different types of values
    env->declareVariable("izpisi", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            for (const auto& arg : args) {
                if (!arg) {
                    std::cout << "nullptr\n";
                    continue;
                }

                if (arg->getTypeName() == "number") {
                    dynamic_cast<NumberValue*>(arg)->toString();
                } else if (arg->getTypeName() == "boolean") {
                    dynamic_cast<BoolValue*>(arg)->toString();
                } else if (arg->getTypeName() == "function") {
                    dynamic_cast<FunctionValue*>(arg)->toString();
                } else if (arg->getTypeName() == "string") {
                    dynamic_cast<StringValue*>(arg)->toString();
                } else if (arg->getTypeName() == "object") {
                    std::cout << "not implemented yet\n";
                }
            }

            std::cout << std::endl;

            return MK_NULL();
        }
    ), true);



    // Define a native method for getting the current time
    env->declareVariable("cas", MK_NATIVE_FUNCTION(timeFunction), true);

    // env->declareVariable("MAT", MK_OBJECT(
    //     std::map<std::string, RuntimeValue*>(
    //         {
    //             {"Pi", MK_NUMBER(3.1415926535897932384626433832795)},
    //             {"Kvadrat", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     float num = dynamic_cast<NumberValue*>(args[0])->value;
    //                     return MK_NUMBER(num * num);
    //                 }
    //             )},
    //             {"Kub", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     float num = dynamic_cast<NumberValue*>(args[0])->value;
    //                     return MK_NUMBER(num * num * num);
    //                 }
    //             )},
    //             {"Faktorial", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     int num = dynamic_cast<NumberValue*>(args[0])->value;
    //                     int fact = 1;
    //                     for(int i = 1; i <= num; i++) {
    //                         fact *= i;
    //                     }
    //                     return MK_NUMBER(fact);
    //                 }
    //             )},
    //             {"Koren", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     float num = dynamic_cast<NumberValue*>(args[0])->value;
    //                     return MK_NUMBER(std::sqrt(num));
    //                 }
    //             )},
    //             {"NakljucnoStevilo", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     int min = dynamic_cast<NumberValue*>(args[0])->value;
    //                     int max = dynamic_cast<NumberValue*>(args[1])->value;
    //                     return MK_NUMBER(rand() % (max - min + 1) + min);
    //                 }
    //             )},
    //             {"Zaokrozi", MK_NATIVE_FUNCTION(
    //                 [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
    //                     float num = dynamic_cast<NumberValue*>(args[0])->value;
    //                     return MK_NUMBER(std::round(num));
    //                 }
    //             )}
    //         }
    //     )
    // ), true);

    env->declareVariable("Pi", MK_NUMBER(3.1415926535897932384626433832795), true);

    env->declareVariable("Kvadrat", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(num * num);
        }
    ), true);

    env->declareVariable("Kub", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(num * num * num);
        }
    ), true);

    env->declareVariable("Faktorial", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            int num = dynamic_cast<NumberValue*>(args[0])->value;
            int fact = 1;
            for(int i = 1; i <= num; i++) {
                fact *= i;
            }
            return MK_NUMBER(fact);
        }
    ), true);

    env->declareVariable("Koren", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(std::sqrt(num));
        }
    ), true);

    env->declareVariable("NakljucnoStevilo", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            int min = dynamic_cast<NumberValue*>(args[0])->value;
            int max = dynamic_cast<NumberValue*>(args[1])->value;
            return MK_NUMBER(std::rand() % (max - min + 1) + min);
        }
    ), true);

    env->declareVariable("Zaokrozi", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(std::round(num));
        }
    ), true);

    env->declareVariable("Sin", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(std::sin(num));
        }
    ), true);

    env->declareVariable("Cos", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;

            return MK_NUMBER(std::cos(num));
        }
    ), true);

    env->declareVariable("Tan", MK_NATIVE_FUNCTION(
        [](std::vector<RuntimeValue*> args, Environment* env) -> RuntimeValue* {
            float num = dynamic_cast<NumberValue*>(args[0])->value;
            return MK_NUMBER(std::tan(num));
        }
    ), true);

    return env;
}