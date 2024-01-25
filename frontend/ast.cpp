#include "lexer.h"
#include "ast.h"

Program::Program() { kind = NodeType::NODE_PROGRAM; }
Program::~Program() = default;
void Program::setKind(NodeType k) { kind = k; }
NodeType Program::getKind() { return kind; }

VariableDeclaration::VariableDeclaration() { kind = NodeType::NODE_VARIABLEDECLARATION; }
VariableDeclaration::VariableDeclaration(Expression* val, bool c) { 
  kind = NodeType::NODE_VARIABLEDECLARATION; 
  expressionValue = val;
  constant = c; 
}

VariableDeclaration::VariableDeclaration(Expression* val, bool c, std::string id) {
  kind = NodeType::NODE_VARIABLEDECLARATION;
  expressionValue = val;
  constant = c;
  identifier = id;
}

VariableDeclaration::VariableDeclaration(std::string id, bool c) : constant(c), identifier(id) { 
  kind = NodeType::NODE_VARIABLEDECLARATION; 
  constant = c;
}
VariableDeclaration::~VariableDeclaration() = default;
void VariableDeclaration::setKind(NodeType k) { kind = k; }
NodeType VariableDeclaration::getKind() { return kind; }

void VariableDeclaration::toString() {
  std::cout << "{\n";
  std::cout << "  kind: \"" << this->getKindName() << "\",\n";
  std::cout << "  constant: " << this->constant << ",\n";
  std::cout << "  identifier: \"" << this->identifier << "\",\n";  
  std::cout << "}\n";
}

FunctionDeclaration::FunctionDeclaration() { kind = NodeType::NODE_FUNCTIONDECLARATION; }
FunctionDeclaration::FunctionDeclaration(std::vector<std::string>& p) {
  kind = NodeType::NODE_FUNCTIONDECLARATION;
  parameters = p;
}
FunctionDeclaration::FunctionDeclaration(std::vector<std::string>& p, std::string& n) {
  kind = NodeType::NODE_FUNCTIONDECLARATION;
  parameters = p;
  name = n;
}
FunctionDeclaration::FunctionDeclaration(std::vector<std::string>& p, std::string& n, std::vector<Statement*>& b) {
  kind = NodeType::NODE_FUNCTIONDECLARATION;
  parameters = p;
  name = n;
  body = b;
}
FunctionDeclaration::~FunctionDeclaration() = default;
std::vector<std::string> parameters = {};
std::string name = "";
std::vector<Statement*> body;

void FunctionDeclaration::toString() {
  std::cout << "{\n";
  std::cout << "  kind: \"" << this->getKindName() << "\",\n";
  std::cout << "  parameters: [ ";
  for(auto param : this->parameters) {
    std::cout << "\"" << param << "\", ";
  }
  std::cout << " ],\n";
  std::cout << "  name: \"" << this->name << "\",\n";
  std::cout << "  body: [ ";
  for(auto statement : this->body) {
    if(statement->getKind() == NodeType::NODE_VARIABLEDECLARATION) {
      dynamic_cast<VariableDeclaration*>(statement)->toString();
    }
  }
  std::cout << "}\n";
}

IfStatement::IfStatement() {
  kind = NODE_IFEXPRESSION;
}
IfStatement::IfStatement(Expression* t) {
  kind = NODE_IFEXPRESSION;
  test = t;
}
IfStatement::IfStatement(Expression* t, std::vector<Statement*> b) {
  kind = NODE_IFEXPRESSION;
  test = t;
  body = b;
}
IfStatement::IfStatement(Expression* t, std::vector<Statement*> b, std::vector<Statement*> a) {
  kind = NODE_IFEXPRESSION;
  test = t;
  body = b;
  alternate = a;
}

void IfStatement::toString() {
  std::cout << "{\n";
  std::cout << "  kind: \"" << this->getKindName() << "\",\n";
  std::cout << "  test: ";  // Print the test expression
  // Add code to print the test expression (not shown in the provided code)
  std::cout << ",\n";
  std::cout << "  body: [ ";
  for (auto statement : this->body) {
    if (statement->getKind() == NodeType::NODE_VARIABLEDECLARATION) {
      dynamic_cast<VariableDeclaration*>(statement)->toString();
    }
  }
  std::cout << " ],\n";
  std::cout << "  alternate: [ ";
  for (auto statement : this->alternate) {
    // Similar to the loop for body, add code to print alternate statements
  }
  std::cout << " ]\n";  // Add a closing square bracket for alternate
  std::cout << "}\n";
}

Iden::Iden(std::string val) : value(val) { kind = NodeType::NODE_IDENTIFIER; }

void Iden::toString() {
  std::cout << "{\n";
  std::cout << "  kind: \"" << this->getKindName() << "\",\n";
  std::cout << "  value: \"" << this->value << "\"\n";
  std::cout << "}";
}

NumericLiteral::NumericLiteral(float val) : value(val) {
    kind = NodeType::NODE_NUMERICLITERAL;
}

void NumericLiteral::toString() {
    std::cout << "{\n";
    std::cout << "  kind: \"" << this->getKindName() << "\",\n";
    std::cout << "  value: \"" << this->value << "\"\n";
    std::cout << "}";
}

StringLiteral::StringLiteral(std::string val) : value(val) { kind = NodeType::NODE_STRINGLITERAL; }

AssignmentExpression::AssignmentExpression() {
  kind = NodeType::NODE_ASSIGNMENTEXPRESSION;
}
AssignmentExpression::AssignmentExpression(Expression* a, Expression* v) {
  kind = NodeType::NODE_ASSIGNMENTEXPRESSION;
  assigne = a;
  value = v;
}

BinaryExpression::BinaryExpression(Expression* l, Expression* r, const std::string& op)
    : left(l), right(r), oper(op) {
    kind = NodeType::NODE_BINARYEXPRESSION;
}

void BinaryExpression::toString() {
    std::cout << "{\n";
    std::cout << "  kind: \"" << this->getKindName() << "\",\n";
    
    if (dynamic_cast<Iden*>(this->left)) {
        std::cout << "  left:";
        dynamic_cast<Iden*>(this->left)->toString();
    } else if (dynamic_cast<NumericLiteral*>(this->left)) {
        std::cout << "  left:";
        dynamic_cast<NumericLiteral*>(this->left)->toString();
    }

    std::cout << "  oper: \"" << this->oper << "\"\n";
    std::cout << "}";
}

MemberExpression::MemberExpression() {
    kind = NodeType::NODE_MEMBEREXPRESSION;
    object = nullptr;
    property = nullptr;
    computed = false;
}

MemberExpression::MemberExpression(Expression* o) {
    kind = NodeType::NODE_MEMBEREXPRESSION;
    object = o;
    property = nullptr;
    computed = false;
}

MemberExpression::MemberExpression(Expression* o, Expression* p) {
    kind = NodeType::NODE_MEMBEREXPRESSION;
    object = o;
    property = p;
    computed = false;
}

MemberExpression::MemberExpression(Expression* o, Expression* p, bool c) {
    kind = NodeType::NODE_MEMBEREXPRESSION;
    object = o;
    property = p;
    computed = c;
}

CallExpression::CallExpression() {
    kind = NodeType::NODE_CALLEXPRESSION;
}

CallExpression::CallExpression(std::vector<Expression*> a) : args(a), caller(nullptr) {
    kind = NodeType::NODE_CALLEXPRESSION;
}

CallExpression::CallExpression(std::vector<Expression*> a, Expression* c) : args(a), caller(c) {
    kind = NodeType::NODE_CALLEXPRESSION;
}

void CallExpression::toString() {
    std::cout << "{\n";
    std::cout << "  kind: \"" << this->getKindName() << "\",\n";
    
    // Print arguments
    for (auto a : this->args) {
        if(dynamic_cast<Iden*>(a)) {
            std::cout << "  args:";
            dynamic_cast<Iden*>(a)->toString();
        } else if(dynamic_cast<NumericLiteral*>(a)) {
            std::cout << "  args:";
            dynamic_cast<NumericLiteral*>(a)->toString();
        }
    }
    
    // Print caller
    if (dynamic_cast<Iden*>(this->caller)) {
        std::cout << "  caller:";
        dynamic_cast<Iden*>(this->caller)->toString();
    } else if (dynamic_cast<NumericLiteral*>(this->caller)) {
        std::cout << "  caller:";
        dynamic_cast<NumericLiteral*>(this->caller)->toString();
    }

    std::cout << "  oper: \"" << this->oper << "\"\n";
    std::cout << "}";
}

Property::Property() {
    this->kind = NodeType::NODE_PROPERTY;
    this->key = "";
    this->value = nullptr;
}

Property::Property(NodeType k, std::string ke) {
    this->kind = k;
    this->key = ke;
    this->value = nullptr;
}

Property::Property(NodeType k, std::string ke, Expression* val) {
    this->kind = k;
    this->key = ke;
    this->value = val;
}

ObjectLiteral::ObjectLiteral() {
    kind = NodeType::NODE_OBJECTLITERAL;
    properties = {};
}

ObjectLiteral::ObjectLiteral(std::vector<Property*> p) {
    kind = NodeType::NODE_OBJECTLITERAL;
    properties = p;
}