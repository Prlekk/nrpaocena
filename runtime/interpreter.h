#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "values.h"
#include "../frontend/parser.h"
#include "environment.h"

RuntimeValue* evaluateNumericBinaryExpression(RuntimeValue* left, RuntimeValue* right, std::string op);
RuntimeValue* evaluateIdentifier(Iden* iden, Environment* env);
RuntimeValue* evaluateBinaryExpression(BinaryExpression* binop, Environment* env);
RuntimeValue* evaluate(Statement* astNode, Environment* env);
RuntimeValue* evaluateVariableDeclaration(VariableDeclaration* declaration, Environment* env);
RuntimeValue* evaluateFunctionDeclaration(FunctionDeclaration* declaration, Environment* env);
RuntimeValue* evaluateAssignment(AssignmentExpression* node, Environment* env);
RuntimeValue* evaluateObject(ObjectLiteral* obj, Environment* env);
RuntimeValue* evaluateCallExpression(CallExpression* obj, Environment* env);
RuntimeValue* evaluateIfStatement(IfStatement* ifStmt, Environment* env);
RuntimeValue* evaluateBody(std::vector<Statement*> body, Environment* env, bool newEnv = true);
RuntimeValue* compare(RuntimeValue* lhs, RuntimeValue* rhs, bool strict);
RuntimeValue* equals(RuntimeValue* lhs, RuntimeValue* rhs, bool strict);

#endif