#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "values.h"

class Environment {
  private:
    Environment* parent;
    std::map<std::string, RuntimeValue*> variables;
    std::set<std::string> constants;
  public:
    Environment();
    Environment(Environment* parentENV);
    bool hasVariable(const std::string& varname) const;
    RuntimeValue* declareVariable(const std::string& varname, RuntimeValue* value, bool constant);
    RuntimeValue* assignVariable(const std::string& varname, RuntimeValue* value);
    RuntimeValue* lookupVariable(const std::string& varname);
    Environment* resolve(std::string varname);
    std::map<std::string, RuntimeValue*> getVariables();
};

using FunctionCall = std::function<RuntimeValue*(std::vector<RuntimeValue*>, Environment*)>;

class NativeFunctionValue : public RuntimeValue {
  public:
    NativeFunctionValue();
    NativeFunctionValue(FunctionCall call);
    virtual ~NativeFunctionValue();
    FunctionCall call;
};

class FunctionValue : public RuntimeValue {
  public:
    FunctionValue();
    FunctionValue(std::string n);
    FunctionValue(std::string n, std::vector<std::string> p);
    FunctionValue(std::string n, std::vector<std::string> p, Environment* env);
    FunctionValue(std::string n, std::vector<std::string> p, Environment* env, std::vector<Statement*> b);
    virtual ~FunctionValue();
    std::string name = "";
    std::vector<std::string> parameters = {};
    Environment* declarationENV;
    std::vector<Statement*> body = {};

    void toString();
};

Environment* createGlobalEnv();
RuntimeValue* MK_NATIVE_FUNCTION(FunctionCall call);

#endif