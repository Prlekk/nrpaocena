#ifndef AST_H
#define AST_H

#include "lexer.h"

class Expression : public Statement {
  
};

class Program : public Statement {
public:
    Program();
    virtual ~Program() = default;

    void setKind(NodeType k);
    NodeType getKind() const;
    std::vector<Statement*>& getBody();

    std::vector<Statement*> body;

    friend std::ostream& operator<<(std::ostream& os, const Program& e);
};

class VariableDeclaration : public Statement {
public:
    VariableDeclaration();
    VariableDeclaration(NodeType k, Expression* val, bool c);
    VariableDeclaration(NodeType k, Expression* val, bool c, std::string id);
    VariableDeclaration(std::string id, bool c);
    virtual ~VariableDeclaration() = default;

    void setKind(NodeType k);
    NodeType getKind() const;

    bool constant;
    std::string identifier;
    Expression* expressionValue;

    void toString() const;
};

class FunctionDeclaration : public Statement {
public:
    FunctionDeclaration();
    FunctionDeclaration(const std::vector<std::string>& p);
    FunctionDeclaration(const std::vector<std::string>& p, const std::string& n);
    FunctionDeclaration(const std::vector<std::string>& p, const std::string& n, const std::vector<Statement*>& b);
    virtual ~FunctionDeclaration() = default;

    std::vector<std::string> parameters;
    std::string name;
    std::vector<Statement*> body;

    void toString() const;
};

#endif