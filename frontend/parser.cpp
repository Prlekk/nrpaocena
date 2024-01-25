#include "parser.h"

Token Parser::at() {
    return this->tokens[0];
}

Token Parser::eat() {
    Token prev = *tokens.begin();
    this->tokens.erase(this->tokens.begin());
    return prev;
}

Token Parser::expect(TokenType type, std::string err) {
    Token prev = this->eat();
    if (prev.type != type) {
        throw std::runtime_error("Parser error: " + err + " - Expecting " + tokenTypeToString(type) + " got " + tokenTypeToString(prev.type));
    }
    return prev;
}


bool Parser::not_eof() {
    return this->tokens[0].type != EndOfFile;
}

Statement* Parser::parseStatement() {
    // std::cout << "Parsing statement" << std::endl;
    switch (this->at().type) {
    case Let:
    case Const:
        // std::cout << "Parsing variable declaration" << std::endl;
        return this->parseVariableDeclaration();
    case Fn:
        // std::cout << "Parsing function declaration" << std::endl;
        return this->parseFunctionDeclaration();
    case If:
        // std::cout << "Parsing if statement" << std::endl;
        return this->parseIfStatement();
    default:
        //  std::cout << "Parsing expression" << std::endl;
        return this->parseExpression();
    }
}

Statement* Parser::parseIfStatement() {
    this->eat();
    this->expect(OpenParen, "Expected '(' after if keyword.");

    Expression* test = this->parseExpression();
    this->expect(CloseParen, "Expected ')' after if condition.");

    std::vector<Statement*> body = {};
    
    this->expect(OpenBrace, "Expected '{' after if condition.");

    while(this->not_eof() && this->at().type != CloseBrace) {
        body.push_back(this->parseStatement());
    }

    this->expect(CloseBrace, "Expected '}' after if statement body.");

    std::vector<Statement*> alternate = {};

    if(this->at().type == Else) {
        this->eat();
        if(this->at().type == If) {
            alternate.push_back(this->parseIfStatement());
        }else {
            this->eat();
            while(this->not_eof() && this->at().type != CloseBrace) {
                alternate.push_back(this->parseStatement());
            }
            this->eat();
        }
    }
    // std::cout << "Success!" << std::endl;
    // std::cout << "Test expression: " << test->getKindName() << std::endl;
    return dynamic_cast<Statement*>(new IfStatement(test, body, alternate));
}

Statement* Parser::parseFunctionDeclaration() {
    this->eat();
    std::string name = this->expect(Identifier, "Expected function name following funkcija keyword.").value;
    std::vector<Expression*> args = this->parseArgs();
    std::vector<std::string> params = {};
    for(auto arg : args) {
        if(arg->getKind() != NodeType::NODE_IDENTIFIER) {
            throw std::runtime_error("Inside function declaration expected to be of type string.");
        }
        params.push_back(dynamic_cast<Iden*>(arg)->value);
    }
    this->expect(OpenBrace, "Expected function body declaration.");

    std::vector<Statement*> body = {};

    while (this->at().type != EndOfFile && this->at().type != CloseBrace) {
        body.push_back(this->parseStatement());
    }

    this->expect(CloseBrace, "Expected closing brace inside function declaration.");
    
    return dynamic_cast<Statement*>(new FunctionDeclaration(params, name, body));
}

Statement* Parser::parseVariableDeclaration() {
    bool isConstant = this->eat().type == Const;
    std::string identifier = this->expect(Identifier, "Expected identifier name following rezerviraj | konstanta keywords.").value;
    if(this->at().type == Semicolon) {
        this->eat();
        if(isConstant) {
            throw std::runtime_error("Constants must be initialized. No value provided.");
        }
        VariableDeclaration* dec = new VariableDeclaration(
            this->parseExpression(),
            isConstant,
            identifier
        );
        return dynamic_cast<Statement*>(dec);
    }
    this->expect(Equals, "Expected equals sign following identifier in var declaration.");
    VariableDeclaration* declaration = new VariableDeclaration(
        this->parseExpression(),
        isConstant,
        identifier
    );
    this->expect(Semicolon, "Variable declaration statement must end with semicolon.");
    return dynamic_cast<Statement*>(declaration);
}

Expression* Parser::parseExpression() {
    return this->parseAssignmentExpression();
}

Expression*  Parser::parseAssignmentExpression() {
    Expression* left = this->parseObjectExpression();
    if(this->at().type == Equals) {
        this->eat();
        Expression* value = this->parseAssignmentExpression();
        return dynamic_cast<Expression*>(new AssignmentExpression(left, value));
    }
    // if(this->at().type == GreaterEquals) {
    //     this->eat();
    //     Expression* value = this->parseAssignmentExpression();
    //     std::cout << "Value kind name: " << value->getKindName() << std::endl;
    //     return dynamic_cast<Expression*>(new IfStatement(value));
    // }

    return left;
}

Expression* Parser::parseObjectExpression() {
    if(this->at().type != OpenBrace) {
        return this->parseAdditiveExpression();
    }

    this->eat(); // Advance past open brace
    std::vector<Property*> properties;

    while(this->not_eof() && this->at().type != CloseBrace) {

        // { key: value, key2: value2 }

        // { key }

        std::string key = this->expect(Identifier, "Object literal key expected.").value;

        // Allows shorthand: key: pair -> key
        if(this->at().type == Comma) {
            this->eat();
            properties.push_back(new Property(NODE_PROPERTY, key, nullptr));
            continue;
        } else if(this->at().type == CloseBrace) {
            properties.push_back(new Property(NODE_PROPERTY, key, nullptr));
            continue;
        }

        // { key: value }

        this->expect(Colon, "Missing colon following identifier in ObjectExpression.");
        Expression* value = this->parseExpression();

        properties.push_back(new Property(NODE_PROPERTY, key, value));
        // std::cout << "key: " << key << "\tvalue: " << value->getValue() << std::endl;

        if(this->at().type != CloseBrace) {
            this->expect(Comma, "Expected comma or closing brace following property.");
        }
    }

    this->expect(CloseBrace, "Expected close brace to terminate object expression.");

    return dynamic_cast<Expression*>(new ObjectLiteral(properties));
}

Expression* Parser::parseAdditiveExpression() {
    Expression* left = this->parseMultiplicativeExpression();

    while(this->at().value == "+" || 
          this->at().value == "-" || 
          this->at().value == "==" || 
          this->at().value == "!=" || 
          this->at().value == ">" || 
          this->at().value == "<" || 
          this->at().value == ">=" || 
          this->at().value == "<=") 
    {
        std::string op = this->eat().value;
        Expression* right = this->parseMultiplicativeExpression();
        left = new BinaryExpression(left, right, op);
    }

    return left;
}

Expression* Parser::parseMultiplicativeExpression() {
    Expression* left = this->parseCallMemberExpression();

    while(this->at().value == "/" || this->at().value == "*" || this->at().value == "%") {
        std::string op = this->eat().value;
        Expression* right = this->parseCallMemberExpression();
        left = new BinaryExpression(left, right, op);
    }

    return left;
}

/*
        Parse left-hand-side before the parenthesis.
        foo.x()
        ^---^
*/
Expression* Parser::parseCallMemberExpression() {
    //  Parse a member expression
    //  Which allows values like 
    //      "foo.bar.baz"
    //  To be computed
    Expression* member = this->parseMemberExpression();

    //  If we are at a open parenthesis
    //  Then we call a call expression

    if(this->at().type == OpenParen) {
        return this->parseCallExpression(member);
    }

    return member;
}

/*
        The caller is the left-hand-side of the expression
        foo(x, y, z)
        ^--^
*/
Expression* Parser::parseCallExpression(Expression* caller) {
    Expression* callExpr = dynamic_cast<Expression*>(new CallExpression(this->parseArgs(), caller));

    //  If there are more open parentheses
    //  We recursively call this function

    if(this->at().type == OpenParen) {
        callExpr = this->parseCallExpression(callExpr);
    }

    return callExpr;
}

//  function add(x, y) => parameters
//  add(a, b) => arguments


/*
        We parse the arguments list
        Which is going to be 1 or more expressions
        Seperated by commas
*/
std::vector<Expression*> Parser::parseArgs() {
    this->expect(OpenParen, "Expected opening parenthesis.");
    std::vector<Expression*> args = this->at().type == CloseParen ? std::vector<Expression*>() : this->parseArgumentList();
    this->expect(CloseParen, "Missing closing parenthesis inside arguments list.");
    return args;
}

std::vector<Expression*> Parser::parseArgumentList() {
    std::vector<Expression*> args = {this->parseAssignmentExpression()};
    while(this->at().type == Comma) {
        this->eat();
        args.push_back(this->parseAssignmentExpression());
    }

    return args;
}

Expression* Parser::parseMemberExpression() {
    Expression* object = this->parsePrimaryExpression();

    while (this->at().type == Dot || this->at().type == OpenBracket) {
        Token oper = this->eat();   // Current operator
        Expression* property = nullptr;
        bool computed;

        // Non-computed values AKA "." - obj.expr

        if(oper.type == Dot) {
            // Get identifier
            computed = false;
            property = this->parsePrimaryExpression();
            if(property->getKind() != NODE_IDENTIFIER) {
                throw std::runtime_error("Cannot use dot operator without right hand side being an identifier.");
            }
        }else {         // Allows obj[computedValue]
                        //        obj[1]
                        //        obj["key"]
                        //        obj[fn()]         . . .
            // Get expression
            computed = true;
            property = this->parseExpression();
            this->expect(CloseBracket, "Missing closing bracket in computed value.");
        }
        object = dynamic_cast<Expression*>(new MemberExpression(object, property, computed));
    }
    return dynamic_cast<Expression*>(object);
    
}

// Orders of precedence: https://en.wikipedia.org/wiki/Operator_precedence

/*
AssignmentExpression        |
MemberExpression            |
FunctionCall                |
LogicalExpression           |
ComparisonExpression        | 
AdditiveExpression          |
MultiplicativeExpression    |
CallExpression              |
MemberExpression            |
PrimaryExpression          \/
*/

Expression* Parser::parsePrimaryExpression() {
    TokenType tk = this->at().type;
    Expression* value = nullptr;
    switch (tk) {
        case Identifier:
            return new Iden(this->eat().value);
        case Number:
            return new NumericLiteral(std::stof(this->eat().value));
        case String:
            return new StringLiteral(this->eat().value);
        case OpenParen:
            this->eat();
            value = this->parseExpression();
            this->expect(
                CloseParen,
                "Unexpected token found inside parenthesised expression. Expected closing parenthesis."
            );
            return value;
        default:
            std::cerr << "Unexpected token found during parsing: Token: " << "{ " << "type: " << tokenTypeToString(this->at()) << ", " << "value: \"" << this->at().value << "\""  " }" << std::endl;
            exit(1);
    }
}

Program Parser::produceAST(std::string sourceCode) {
    // std::cout << "Producing AST..." << std::endl;
    // std::cout << sourceCode << std::endl;
    this->tokens = tokenize(sourceCode);
    Program program;
    program.setKind(NodeType::NODE_PROGRAM);
    program.body = {};

    // // Parse until end of file
    //   std::cout << "Parsing statement..." << std::endl;
    while (this->not_eof()) {
        program.body.push_back(this->parseStatement());
    }

    return program;
}

void consoleLog(Program& program) {
    std::cout << "{\n";
    std::cout << "  kind: \"" << program.getKindName() << "\",\n";
    std::cout << "  body: [\n";

    for (auto stmt : program.body) {
        std::cout << "    ";

        switch (stmt->getKind()) {
            case NODE_IDENTIFIER:
                // std::cout << *dynamic_cast<Iden*>(stmt);
                break;
            case NODE_NUMERICLITERAL:
                // std::cout << *dynamic_cast<NumericLiteral*>(stmt);
                break;
            case NODE_OBJECTLITERAL:
                // std::cout << dynamic_cast<ObjectLiteral*>(stmt)->getKindName() << "\n";
                break;
            case NODE_BINARYEXPRESSION:
                // std::cout << *dynamic_cast<BinaryExpression*>(stmt);
                break;
            case NODE_VARIABLEDECLARATION:
                dynamic_cast<VariableDeclaration*>(stmt)->toString();
                break;
            case NODE_PROGRAM:
                // std::cout << *dynamic_cast<Program*>(stmt);
                break;
            case NODE_ASSIGNMENTEXPRESSION:
                // std::cout << *dynamic_cast<AssignmentExpression*>(stmt);
                break;
            case NODE_PROPERTY:
                // // std::cout << *dynamic_cast<Property*>(stmt);
                break;
            // Handle other cases if needed...

            default:
                std::cerr << "Unhandled node type during logging.\n";
                exit(1);
        }
    }

    std::cout << "  ]\n";
    std::cout << "}\n";
}
