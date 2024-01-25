#include "interpreter.h"
#include <cmath>

/**
 * Evaluates a variable declaration and declares the variable in the environment.
 * 
 * @param declaration The variable declaration to evaluate.
 * @param env The environment in which to declare the variable.
 * @return The runtime value of the variable.
 */
RuntimeValue* evaluateVariableDeclaration(VariableDeclaration* declaration, Environment* env) {
    return env->declareVariable(declaration->identifier, evaluate(declaration->expressionValue, env), declaration->constant);
}

/*
 * Evaluate a function declaration and declare the function in the environment.
 * 
 * @param declaration The function declaration to evaluate.
 * @param env The environment in which to declare the function.
 * @return The runtime value of the function.
 */
RuntimeValue* evaluateFunctionDeclaration(FunctionDeclaration* declaration, Environment* env) {
    // Create a new function value
    FunctionValue* func = new FunctionValue(
        declaration->name,  // name of the function
        declaration->parameters,  // parameters of the function
        env,  // current environment
        declaration->body  // body of the function
    );

    // Declare the function as a variable in the current environment
    return env->declareVariable(declaration->name, dynamic_cast<RuntimeValue*>(func), true);
}

/*
 * Evaluate a program by iterating through its statements
 * 
 * @param program The program to evaluate
 * @param env The environment in which to evaluate the program
 * @return The last evaluated runtime value
*/
RuntimeValue* evaluateProgram(Program* program, Environment* env) {
    RuntimeValue* lastEvaluated = MK_NULL();
    for(auto stmt : program->body) {
        lastEvaluated = evaluate(stmt, env);
    }
    return lastEvaluated;
}

/**
 * Evaluate a numeric binary expression.
 * 
 * @param left The left operand
 * @param right The right operand
 * @param op The operator
 * @return The result of the binary expression
 */
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
            } else {
                result = l->value / r->value;
            }
        } else if(op == "%") {
            if(r->value == 0.0) {
                throw std::runtime_error("Modulo z 0.");
            } else {
                result = fmod(l->value, r->value);
            }
        }
        return MK_NUMBER(result);
    }
    return MK_NULL();
}

template <typename T>
/**
 * @brief Check if the left-hand side value is less than or equal to the right-hand side value.
 * 
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @return true if lhs is less than or equal to rhs, false otherwise.
 */
bool isLessEqual(T lhs, T rhs) {
    return lhs <= rhs;
}

/**
 * Checks if the left-hand side value is not greater than or equal to the right-hand side value.
 * @param lhs - The left-hand side value
 * @param rhs - The right-hand side value
 * @return true if lhs is not greater than or equal to rhs, false otherwise
 */
template <typename T>
bool notGreaterEqual(T lhs, T rhs) {
    return lhs >= rhs;
}

/**
 * @brief Compares two values and returns the result based on the strict flag.
 * 
 * @tparam T The type of the values to be compared.
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @param strict If true, the comparison will be less than or equal to, otherwise not greater than or equal to.
 * @return true if the comparison is true, false otherwise.
 */
template <typename T>
bool isComparisonTrue(T lhs, T rhs, bool strict) {
    return strict ? isLessEqual(lhs, rhs) : notGreaterEqual(lhs, rhs);
}

/*
 * This function compares two RuntimeValues and returns the result
 * 
 * @param lhs The left-hand side RuntimeValue
 * @param rhs The right-hand side RuntimeValue
 * @param strict A boolean indicating whether the comparison should be strict
 * @return A RuntimeValue representing the result of the comparison
 */
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
/**
 * @brief Check if the given values are equal.
 * 
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @return true if the values are equal, false otherwise.
 */
bool isEqual(T lhs, T rhs) {
    return lhs == rhs;
}

template <typename T>
/**
 * @brief Checks if the given values are not equal.
 * 
 * @param lhs The left-hand side value.
 * @param rhs The right-hand side value.
 * @return true if the values are not equal, false otherwise.
 */
bool notEqual(T lhs, T rhs) {
    return lhs != rhs;
}

template <typename T>
/**
 * Compares two values for equality.
 * @param lhs The left-hand side value
 * @param rhs The right-hand side value
 * @param strict If true, strict equality is checked; if false, non-strict equality is checked
 * @return True if the values are equal based on the strictness parameter
 */
bool compareEquality(T lhs, T rhs, bool strict) {
    return strict ? isEqual(lhs, rhs) : notEqual(lhs, rhs);
}

/**
 * Compares the equality of two runtime values.
 * 
 * @param lhs - The left-hand side runtime value
 * @param rhs - The right-hand side runtime value
 * @param strict - Whether to use strict comparison
 * @return A boolean runtime value indicating the result of the comparison
 */
RuntimeValue* equals(RuntimeValue* lhs, RuntimeValue* rhs, bool strict) {
    switch (lhs->type) {
        // Compare equality for boolean values
        case VALUETYPE_BOOLEAN:
            return MK_BOOL(compareEquality(dynamic_cast<BoolValue*>(lhs)->value, dynamic_cast<BoolValue*>(rhs)->value, strict));

        // Compare equality for number values
        case VALUETYPE_NUMBER:
            return MK_BOOL(compareEquality(dynamic_cast<NumberValue*>(lhs)->value, dynamic_cast<NumberValue*>(rhs)->value, strict));

        // Compare equality for function values
        case VALUETYPE_FUNCTION:
            return MK_BOOL(compareEquality(dynamic_cast<FunctionValue*>(lhs)->body, dynamic_cast<FunctionValue*>(rhs)->body, strict));

        // Compare equality for null values
        case VALUETYPE_NULL:
            return MK_BOOL(compareEquality(dynamic_cast<NullValue*>(lhs)->value, dynamic_cast<NullValue*>(rhs)->value, strict));

        // Throw an error for unhandled value types
        default:
            throw std::runtime_error("Unhandeled type in equals function.");
    }

    // Return null value by default
    return MK_NULL();
}

/*
 * Evaluate an identifier in the given environment
 *
 * @param iden The identifier to evaluate
 * @param env The environment in which to evaluate the identifier
 * @return The runtime value of the identifier
 */
RuntimeValue* evaluateIdentifier(Iden* iden, Environment* env) {
    RuntimeValue* val = env->lookupVariable(iden->value);
    return val;
}

/*
 * Evaluate an object literal and return a RuntimeValue
 *
 * @param obj The object literal to evaluate
 * @param env The current environment
 * @return The runtime value of the object
 */
RuntimeValue* evaluateObject(ObjectLiteral* obj, Environment* env) {
    // Create a new ObjectValue
    ObjectValue* object = new ObjectValue();

    // Iterate through each property of the object
    for(auto prop : obj->properties) {
        // Evaluate the property value or look it up in the current environment
        RuntimeValue* runtimeValue = (prop->value == nullptr) ? env->lookupVariable(prop->key) : evaluate(prop->value, env);

        // Assign the evaluated value to the property in the object
        object->properties[prop->key] = runtimeValue;
    }

    // Return the object as a RuntimeValue
    return dynamic_cast<RuntimeValue*>(object);
}

/*
 * Evaluate a call expression
 *
 * @param expr The call expression to evaluate
 * @param env The environment in which to evaluate the call expression
 * @return The runtime value of the call expression
 */
RuntimeValue* evaluateCallExpression(CallExpression* expr, Environment* env) {
    std::vector<RuntimeValue*> args; // Store the evaluated arguments
    Expression* temp; // Temporary variable for storing the current argument expression
    for(int i = 0; i < expr->args.size(); i++) {
        temp = expr->args[i];
        args.push_back(evaluate(temp, env)); // Evaluate each argument and store the result
    }

    if(dynamic_cast<FunctionValue*>(evaluate(expr->caller, env))) {
        FunctionValue* func = dynamic_cast<FunctionValue*>(evaluate(expr->caller, env));
        if(func->type == VALUETYPE_FUNCTION) {
            Environment* scope = new Environment(func->declarationENV); // Create a new environment for the function scope

            for(int i = 0; i < func->parameters.size(); i++) {
                // TODO Check the bounds
                // Verify the arity of the function
                std::string varname = func->parameters[i];
                scope->declareVariable(varname, args[i], false); // Declare function parameters in the function scope
            }

            RuntimeValue* result = MK_NULL(); // Initialize the result with null value
            // Evaluate the function body line by line
            for(auto stmt : func->body) {
                result = evaluate(stmt, scope); // Evaluate each statement in the function body with the function scope
            }
            return result; // Return the result of the function evaluation
        }
    } else if(dynamic_cast<NativeFunctionValue*>(evaluate(expr->caller, env))) {
        NativeFunctionValue* fn = dynamic_cast<NativeFunctionValue*>(evaluate(expr->caller, env));
        RuntimeValue* result = fn->call(args, env); // Call the native function with the arguments and environment
        return result; // Return the result of the native function call
    }
    throw std::runtime_error("Cannot call a value that is not a function."); // Throw an error if the caller is not a function
}

RuntimeValue* evaluateMemberExpression(Environment* env, AssignmentExpression* node, MemberExpression* expr) {
    if(expr != nullptr) {
        RuntimeValue* variable = env->lookupOrMutateObject(expr, nullptr, nullptr);

        return variable;
    }else if (node != nullptr) {
        RuntimeValue* variable = env->lookupOrMutateObject(dynamic_cast<MemberExpression*>(node->assigne), evaluate(node->value, env), nullptr);

        return variable;
    }
    throw std::runtime_error("Evaluating a member expression is not possible without a member or assignment expression.");
}

/**
 * Evaluate a binary expression
 * 
 * @param binop - the binary expression to evaluate
 * @param env - the environment in which to evaluate the expression
 * @return the result of evaluating the binary expression
 */
RuntimeValue* evaluateBinaryExpression(BinaryExpression* binop, Environment* env) {
    // Evaluate the left and right operands
    RuntimeValue* left = evaluate(binop->left, env);
    RuntimeValue* right = evaluate(binop->right, env);

    // Return the result of evaluating the binary expression
    return evaluateNumericBinaryExpression(left, right, binop->oper);
}

/**
 * Evaluate an assignment expression
 * 
 * @param node - the assignment expression node
 * @param env - the environment in which the expression is evaluated
 * @return the runtime value after assignment
 */
RuntimeValue* evaluateAssignment(AssignmentExpression* node, Environment* env) {
    if(node->assigne->getKind() != NODE_IDENTIFIER) {
        throw std::runtime_error("Invalid left-hand-side inside assignment expression.");
    }
    // Get the variable name from the assignment expression
    std::string varname = dynamic_cast<Iden*>(node->assigne)->value;
    // Evaluate the value of the assignment expression and assign it to the variable in the environment
    return env->assignVariable(varname, evaluate(node->value, env));
}

/*
 * Evaluate a body of statements
 *
 * @param body The body of statements to evaluate
 * @param env The environment in which to evaluate the body
 * @param newEnv Whether to create a new environment for evaluation
 * @return The result of the last evaluated statement
 */
RuntimeValue* evaluateBody(std::vector<Statement*> body, Environment* env, bool newEnv) {
    Environment* scope = nullptr;
    if(newEnv) {
        // Create a new environment based on the provided environment
        scope = new Environment(env);
    } else {
        // Use the provided environment as the scope for evaluation
        scope = env;
    }

    // Initialize the result as a null value
    RuntimeValue* result = MK_NULL();

    // Loop through each statement in the body and evaluate it in the current scope
    for(auto stmt : body) {
        result = evaluate(stmt, scope);
    }

    // Return the result of the last evaluated statement
    return result;
}

/**
 * Evaluates an if statement and returns the result.
 * 
 * @param ifStmt The if statement to evaluate
 * @param env The environment in which to evaluate the if statement
 * @return The result of the evaluation
 */
RuntimeValue* evaluateIfStatement(IfStatement* ifStmt, Environment* env) {
    // Evaluate the test expression
    RuntimeValue* test = evaluate(ifStmt->test, env);
    // Check if the result is a boolean value
    if(test->type == VALUETYPE_BOOLEAN) {
        // If the test evaluates to true, evaluate the body
        if(dynamic_cast<BoolValue*>(test)->value) {
            return evaluateBody(ifStmt->body, env);
        } else { // If the test evaluates to false, evaluate the alternate
            return evaluateBody(ifStmt->alternate, env);
        }
    }
    // If the test result is not a boolean, return null
    return MK_NULL();
}

/**
 * @brief Evaluate the AST node and return the runtime value.
 *
 * @param astNode The AST node to evaluate.
 * @param env The environment for evaluation.
 * @return RuntimeValue* The runtime value of the evaluated AST node.
 */
RuntimeValue* evaluate(Statement* astNode, Environment* env){
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
    case NODE_MEMBEREXPRESSION:
        return evaluateMemberExpression(env, dynamic_cast<AssignmentExpression*>(astNode), dynamic_cast<MemberExpression*>(astNode));
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
        std::cout << "Node kind: " << astNode->getKindName() << std::endl;
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