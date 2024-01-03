// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "ast.cpp"



class Parser {
  private:
    std::vector<Token> tokens = {};

    Token at();
    Token eat();
    Token expect(TokenType type, std::string err);
    bool not_eof();
    Statement* parseStatement();
    Expression* parseExpression();
    Expression* parseObjectExpression();
    Expression* parseAssignmentExpression();
    Statement* parseVariableDeclaration();
    Statement* parseFunctionDeclaration();
    Expression* parseAdditiveExpression();
    Expression* parsePrimaryExpression();
    Expression* parseMultiplicativeExpression();
    Expression* parseCallMemberExpression();
    Expression* parseCallExpression(Expression* caller);
    std::vector<Expression*> parseArgs();
    std::vector<Expression*> parseArgumentList();
    Expression* parseMemberExpression();
    Statement* parseIfStatement();

  public:
    Program produceAST(std::string sourceCode);
};

void consoleLog(Program& program);

#endif  // PARSER_H
