#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "values.h"
#include "../frontend/ast.h"

class Environment {
  private:
    // Oèetovski "environment" (slov. okolje)
    Environment* parent;
    
    // Polje spremenljivk in njihovih vrednosti
    std::map<std::string, RuntimeValue*> variables;

    // Polje konstant
    std::set<std::string> constants;
  public:
    Environment();
    Environment(Environment* parentENV);

    // Funkcija za preverjanje ali obstaja spremenljivka v okolju
    bool hasVariable(const std::string& varname) const;

    // Funkcije za deklaracijo spremenljivke v okolju
    RuntimeValue* declareVariable(const std::string& varname, RuntimeValue* value, bool constant);
    
    // Funkcije za prireditev spremenljivke v okolju
    RuntimeValue* assignVariable(const std::string& varname, RuntimeValue* value);
    
    // Funkcija za iskanje vrednosti spremenljivke v okolju
    RuntimeValue* lookupVariable(const std::string& varname);

    RuntimeValue* lookupOrMutateObject(MemberExpression* expr, RuntimeValue* value, Iden* property);

    // Funkcija za iskanje spremenljivke v okolju
    Environment* resolve(std::string varname);

    // Getter funkcija za polje spremenljivk
    std::map<std::string, RuntimeValue*> getVariables();
};

// Define a type alias for a function that takes a vector of RuntimeValue pointers and an Environment pointer as arguments and returns a pointer to a RuntimeValue
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