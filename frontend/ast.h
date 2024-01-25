#ifndef AST_H
#define AST_H

#include "lexer.h"

class Expression : public Statement {
  
};

class Program : public Statement {
public:
    Program();
    ~Program();

    void setKind(NodeType k);
    NodeType getKind();
    std::vector<Statement*>& getBody();

    std::vector<Statement*> body;
};

class VariableDeclaration : public Statement {
public:
    VariableDeclaration();
    VariableDeclaration(Expression* val, bool c);
    VariableDeclaration(Expression* val, bool c, std::string id);
    VariableDeclaration(std::string id, bool c);
    ~VariableDeclaration();

    void setKind(NodeType k);
    NodeType getKind();

    bool constant;
    std::string identifier;
    Expression* expressionValue;

    void toString();
};

class FunctionDeclaration : public Statement {
public:
    FunctionDeclaration();
    FunctionDeclaration(std::vector<std::string>& p);
    FunctionDeclaration(std::vector<std::string>& p, std::string& n);
    FunctionDeclaration(std::vector<std::string>& p, std::string& n, std::vector<Statement*>& b);
    ~FunctionDeclaration();

    std::vector<std::string> parameters;
    std::string name;
    std::vector<Statement*> body;

    void toString();
};

class IfStatement : public Statement {
public:
    IfStatement();
    IfStatement(Expression* t);
    IfStatement(Expression* t, std::vector<Statement*> b);
    IfStatement(Expression* t, std::vector<Statement*> b, std::vector<Statement*> a);

    Expression* test;
    std::vector<Statement*> body;
    std::vector<Statement*> alternate;

    void toString();
};

class Iden : public Expression {
public:
    Iden(std::string val);

    std::string value = "";

    void toString();
};

class NumericLiteral : public Expression {
public:
    NumericLiteral(float val);

    float value;

    void toString();
};

class StringLiteral : public Expression {
public:
    StringLiteral(std::string val);

    std::string value;
};

class AssignmentExpression : public Expression {
public:
    AssignmentExpression();
    AssignmentExpression(Expression* a, Expression* v);

    Expression* assigne;
    Expression* value;
};

class BinaryExpression : public Expression {
public:
    BinaryExpression(Expression* l, Expression* r, const std::string& op);

    Expression* left;
    Expression* right;
    std::string oper;

    void toString();
};

class MemberExpression : public Expression {
  private:
  public:
    MemberExpression();
    MemberExpression(Expression* o);
    MemberExpression(Expression* o, Expression* p);
    MemberExpression(Expression* o, Expression* p, bool c);
    Expression* object = nullptr;
    Expression* property = nullptr;
    bool computed;
};

class CallExpression : public Expression {
public:
    CallExpression();
    CallExpression(std::vector<Expression*> a);
    CallExpression(std::vector<Expression*> a, Expression* c);

    std::vector<Expression*> args;
    Expression* caller;
    std::string oper;

    void toString();
};

class Property : public Expression {
public:
    Property();
    Property(NodeType k, std::string ke);
    Property(NodeType k, std::string ke, Expression* val);

    std::string key;
    Expression* value;
};

class ObjectLiteral : public Expression {
public:
    ObjectLiteral();
    ObjectLiteral(std::vector<Property*> p);

    std::vector<Property*> properties;
};

#endif