// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"


// Parser 
// Razred za raz�lembo dolo�enega "token"-a (slov. �etona)

class Parser {
  private:

    // Polje tokenov
    std::vector<Token> tokens = {};

    // Trenuten token
    Token at();

    // Naslednji token
    Token eat();

    // Pri�akuj dolo�en token
    Token expect(TokenType type, std::string err);

    // Preveri ali je token EOF
    // Pomeni konec izvorne kode
    bool not_eof();

    // Raz�lemba izjave
    /*
       Deklaracija spremenljivke  rezerviraj a = 10;
       Deklaracija funkcije       funkcija a() {}
       Odlo�itveni stavki         ce() {}
       Izrazi                     a >= 10
    */
    Statement* parseStatement();

    // Raz�lemba izraza
    Expression* parseExpression();

    // Raz�lemba objekta
    Expression* parseObjectExpression();

    // Raz�lemba prireditve
    Expression* parseAssignmentExpression();

    // Raz�lemba deklaracije spremenljivke
    Statement* parseVariableDeclaration();

    // Raz�lemba deklaracije funkcije
    Statement* parseFunctionDeclaration();

    // Raz�lemba matemati�nih izrazov ali primerjav
    Expression* parseAdditiveExpression();
    Expression* parseMultiplicativeExpression();

    // Raz�lemba primarnih izrazov
    /*
       Identifikator
       �tevilka
       Niz znakov
       Oklepaji () v katerih je izraz
    */
    Expression* parsePrimaryExpression();

    // objekt.member
    Expression* parseCallMemberExpression();

    // objekt.member(arg1, arg2, arg3)
    Expression* parseCallExpression(Expression* caller);

    // Argumenti
    std::vector<Expression*> parseArgs();

    // Polje argumentov
    std::vector<Expression*> parseArgumentList();

    // Raz�lemba objekta
    Expression* parseMemberExpression();

    // Raz�lemba odlo�itvenega stavka
    Statement* parseIfStatement();

  public:

    // izdelava AST
    /*
       Abstract Syntax Tree
    */
    Program produceAST(std::string sourceCode);
};

void consoleLog(Program& program);

#endif  // PARSER_H
