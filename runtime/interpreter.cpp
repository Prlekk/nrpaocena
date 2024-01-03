#include "interpreter.h"
#include <cmath>

RuntimeValue* evaluateVariableDeclaration(VariableDeclaration* declaration, Environment* env) {
    return env->declareVariable(declaration->identifier, evaluate(declaration->expressionValue, env), declaration->constant);
}

RuntimeValue* evaluateFunctionDeclaration(FunctionDeclaration* declaration, Environment* env) {
    FunctionValue* func = new FunctionValue(
        declaration->name,
        declaration->parameters,
        env,
        declaration->body
    );

    return env->declareVariable(declaration->name, dynamic_cast<RuntimeValue*>(func), true);
}

RuntimeValue* evaluateProgram(Program* program, Environment* env) {
    RuntimeValue* lastEvaluated = MK_NULL();
    for(auto stmt : program->body) {
        lastEvaluated = evaluate(stmt, env);
    }
    return lastEvaluated;
}

RuntimeValue* evaluateNumericBinaryExpression(RuntimeValue* left, RuntimeValue* right, std::string op) {
    if(op == "!=") {
        return equals(left, right, false);
    } else if(op == "==") {
        return equals(left, right, true);
    } else if(op == "<=") {
        return compare(left, right, true);
    } else if(op == ">=") {
        return compare(left, right, false);
    } else if(left->type == VALUETYPE_NUMBER && right->type == VALUETYPE_NUMBER) {
        NumberValue* l = dynamic_cast<NumberValue*>(left);
        NumberValue* r = dynamic_cast<NumberValue*>(right);
        float result = 0.0;
        if(op == "+") {
            result = l->value + r->value;
        } else if(op == "-") {
            result = l->value - r->value;
        } else if(op == "*") {
            result = l->value * r->value;
        } else if(op == "/") {
            if(r->value == 0.0) {
                throw std::runtime_error("Deljenje z 0.");
            }else {
                result = l->value / r->value;
            }
        } else if(op == "%") {
            if(r->value == 0.0) {
                throw std::runtime_error("Modulo z 0.");
            }else {
                result = fmod(l->value, r->value);
            }
        }
        return MK_NUMBER(result);
    }
    return MK_NULL();
}

template <typename T>
bool isLessEqual(T lhs, T rhs) {
    return lhs <= rhs;
}

template <typename T>
bool notGreaterEqual(T lhs, T rhs) {
    return lhs >= rhs;
}

template <typename T>
bool isComparisonTrue(T lhs, T rhs, bool strict) {
    return strict ? isLessEqual(lhs, rhs) : notGreaterEqual(lhs, rhs);
}

RuntimeValue* compare(RuntimeValue* lhs, RuntimeValue* rhs, bool strict) {
    switch (lhs->type) {
        case VALUETYPE_BOOLEAN:
            return MK_BOOL(isComparisonTrue(dynamic_cast<BoolValue*>(lhs)->value, dynamic_cast<BoolValue*>(rhs)->value, strict));
        case VALUETYPE_NUMBER:
            return MK_BOOL(isComparisonTrue(dynamic_cast<NumberValue*>(lhs)->value, dynamic_cast<NumberValue*>(rhs)->value, strict));
        case VALUETYPE_FUNCTION:
            return MK_BOOL(isComparisonTrue(dynamic_cast<FunctionValue*>(lhs)->body, dynamic_cast<FunctionValue*>(rhs)->body, strict));
        case VALUETYPE_NULL:
            return MK_BOOL(isComparisonTrue(dynamic_cast<NullValue*>(lhs)->value, dynamic_cast<NullValue*>(rhs)->value, strict));
        default:
            throw std::runtime_error("Unhandeled type in equals function.");
    }
    return MK_NULL();
}

template <typename T>
bool isEqual(T lhs, T rhs) {
    return lhs == rhs;
}

template <typename T>
bool notEqual(T lhs, T rhs) {
    return lhs != rhs;
}

template <typename T>
bool compareEquality(T lhs, T rhs, bool strict) {
    return strict ? isEqual(lhs, rhs) : notEqual(lhs, rhs);
}

RuntimeValue* equals(RuntimeValue* lhs, RuntimeValue* rhs, bool strict) {
    switch (lhs->type) {
        case VALUETYPE_BOOLEAN:
            return MK_BOOL(compareEquality(dynamic_cast<BoolValue*>(lhs)->value, dynamic_cast<BoolValue*>(rhs)->value, strict));
        case VALUETYPE_NUMBER:
            return MK_BOOL(compareEquality(dynamic_cast<NumberValue*>(lhs)->value, dynamic_cast<NumberValue*>(rhs)->value, strict));
        case VALUETYPE_FUNCTION:
            return MK_BOOL(compareEquality(dynamic_cast<FunctionValue*>(lhs)->body, dynamic_cast<FunctionValue*>(rhs)->body, strict));
        case VALUETYPE_NULL:
            return MK_BOOL(compareEquality(dynamic_cast<NullValue*>(lhs)->value, dynamic_cast<NullValue*>(rhs)->value, strict));
        default:
            throw std::runtime_error("Unhandeled type in equals function.");
    }
    return MK_NULL();
}

RuntimeValue* evaluateIdentifier(Iden* iden, Environment* env) {
    RuntimeValue* val = env->lookupVariable(iden->value);
    return val;
}

RuntimeValue* evaluateObject(ObjectLiteral* obj, Environment* env) {
    ObjectValue* object = new ObjectValue();

    for(auto prop : obj->properties) {
        RuntimeValue* runtimeValue = (prop->value == nullptr) ? env->lookupVariable(prop->key) : evaluate(prop->value, env);

        object->properties[prop->key] = runtimeValue;
    }

    return dynamic_cast<RuntimeValue*>(object);
}

RuntimeValue* evaluateCallExpression(CallExpression* expr, Environment* env) {
    std::vector<RuntimeValue*> args;
    Expression* temp;
    for(int i = 0; i < expr->args.size(); i++) {
        temp = expr->args[i];
        args.push_back(evaluate(temp, env));
    }

    if(dynamic_cast<FunctionValue*>(evaluate(expr->caller, env))) {
        FunctionValue* func = dynamic_cast<FunctionValue*>(evaluate(expr->caller, env));
        if(func->type == VALUETYPE_FUNCTION) {
            Environment* scope = new Environment(func->declarationENV);

            for(int i = 0; i < func->parameters.size(); i++) {
            // TODO Check the bounds
            // Verify the airity of the function
            std::string varname = func->parameters[i];
            scope->declareVariable(varname, args[i], false);
            }

            RuntimeValue* result = MK_NULL();
            // Evaluate the function body line by line
            for(auto stmt : func->body) {
                result = evaluate(stmt, scope);
            }
            return result;
        }
    }else if(dynamic_cast<NativeFunctionValue*>(evaluate(expr->caller, env))) {
        NativeFunctionValue* fn = dynamic_cast<NativeFunctionValue*>(evaluate(expr->caller, env));
        RuntimeValue* result = fn->call(args, env);
        return result;
    }
    throw std::runtime_error("Cannot call a value that is not a function.");
}

RuntimeValue* evaluateBinaryExpression(BinaryExpression* binop, Environment* env) {
    RuntimeValue* left = evaluate(binop->left, env);
    RuntimeValue* right = evaluate(binop->right, env);

    return evaluateNumericBinaryExpression(left, right, binop->oper);
}

RuntimeValue* evaluateAssignment(AssignmentExpression* node, Environment* env) {
    if(node->assigne->getKind() != NODE_IDENTIFIER) {
        throw std::runtime_error("Invalid left-hand-side inside assignment expression.");
    }
    std::string varname = dynamic_cast<Iden*>(node->assigne)->value;
    return env->assignVariable(varname, evaluate(node->value, env));
}

RuntimeValue* evaluateBody(std::vector<Statement*> body, Environment* env, bool newEnv) {
    Environment* scope = nullptr;
    if(newEnv) {
        scope = new Environment(env);
    }else {
        scope = env;
    }

    RuntimeValue* result = MK_NULL();

    for(auto stmt : body) {
        result = evaluate(stmt, scope);
    }

    return result;
}

RuntimeValue* evaluateIfStatement(IfStatement* ifStmt, Environment* env) {
    RuntimeValue* test = evaluate(ifStmt->test, env);
    if(test->type == VALUETYPE_BOOLEAN) {
        if(dynamic_cast<BoolValue*>(test)->value) {
            return evaluateBody(ifStmt->body, env);
        } else {
            return evaluateBody(ifStmt->alternate, env);
        }
        
    }
    return MK_NULL();
}

RuntimeValue* evaluate(Statement* astNode, Environment* env){
    //  std::cout << "Evaluating " << astNode->getKindName() << "..." << std::endl;
    switch (astNode->getKind()) {
    case NODE_NUMERICLITERAL:
        return dynamic_cast<RuntimeValue*>(new NumberValue(dynamic_cast<NumericLiteral*>(astNode)->value));
    case NODE_STRINGLITERAL:
        //  std::cout << "Evaluating string literal..." << std::endl;
        return dynamic_cast<RuntimeValue*>(new StringValue(dynamic_cast<StringLiteral*>(astNode)->value));
    case NODE_IDENTIFIER:
        return evaluateIdentifier(dynamic_cast<Iden*>(astNode), env);
    case NODE_OBJECTLITERAL:
        return evaluateObject(dynamic_cast<ObjectLiteral*>(astNode), env);
    case NODE_CALLEXPRESSION:
        return evaluateCallExpression(dynamic_cast<CallExpression*>(astNode), env);
    case NODE_ASSIGNMENTEXPRESSION:
        return evaluateAssignment(dynamic_cast<AssignmentExpression*>(astNode), env);
    case NODE_BINARYEXPRESSION:
        // std::cout << "Evaluating binary expression..." << std::endl;
        return evaluateBinaryExpression(dynamic_cast<BinaryExpression*>(astNode), env);
    case NODE_PROGRAM:
        return evaluateProgram(dynamic_cast<Program*>(astNode), env);
    case NODE_VARIABLEDECLARATION:
        return evaluateVariableDeclaration(dynamic_cast<VariableDeclaration*>(astNode), env);
    case NODE_FUNCTIONDECLARATION:
        return evaluateFunctionDeclaration(dynamic_cast<FunctionDeclaration*>(astNode), env);
    case NODE_IFEXPRESSION:
        // std::cout << "Evaluating if statement kind name: " << dynamic_cast<IfStatement*>(astNode)->getKindName() << std::endl;
        return evaluateIfStatement(dynamic_cast<IfStatement*>(astNode), env);
    default:
        std::cout << "This AST node has not yet been set up for interpretation. " << std::endl;
        if(astNode->getKind() == NODE_NUMERICLITERAL) {
            // std::cout << *dynamic_cast<NumericLiteral*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_IDENTIFIER) {
            // std::cout << *dynamic_cast<Iden*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_ASSIGNMENTEXPRESSION){
            // std::cout << *dynamic_cast<AssignmentExpression*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_BINARYEXPRESSION){
            // std::cout << *dynamic_cast<BinaryExpression*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_PROGRAM){
            // std::cout << *dynamic_cast<Program*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_VARIABLEDECLARATION){
            dynamic_cast<VariableDeclaration*>(astNode)->toString();
        } else if(astNode->getKind() == NODE_OBJECTLITERAL){
            // std::cout << *dynamic_cast<ObjectLiteral*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_PROPERTY){
            // // std::cout << *dynamic_cast<Property*>(astNode) << std::endl;
        } else if(astNode->getKind() == NODE_FUNCTIONDECLARATION) {
            dynamic_cast<FunctionDeclaration*>(astNode)->toString();
        }

        exit(1);
    }
}