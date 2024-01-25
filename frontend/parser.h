// parser.h

#ifndef PARSER_H
#define PARSER_H

#include "ast.h"


// Parser 
// Razred za razèlembo doloèenega "token"-a (slov. ¾etona)

class Parser {
  private:

    // Polje tokenov
    std::vector<Token> tokens = {};

    // Trenuten token
    Token at();

    // Naslednji token
    Token eat();

    // Prièakuj doloèen token
    Token expect(TokenType type, std::string err);

    // Preveri ali je token EOF
    // Pomeni konec izvorne kode
    bool not_eof();

    // Razèlemba izjave
    /*
       Deklaracija spremenljivke  rezerviraj a = 10;
       Deklaracija funkcije       funkcija a() {}
       Odloèitveni stavki         ce() {}
       Izrazi                     a >= 10
    */
    Statement* parseStatement();

    // Razèlemba izraza
    Expression* parseExpression();

    // Razèlemba objekta
    Expression* parseObjectExpression();

    // Razèlemba prireditve
    Expression* parseAssignmentExpression();

    // Razèlemba deklaracije spremenljivke
    Statement* parseVariableDeclaration();

    // Razèlemba deklaracije funkcije
    Statement* parseFunctionDeclaration();

    // Razèlemba matematiènih izrazov ali primerjav
    Expression* parseAdditiveExpression();
    Expression* parseMultiplicativeExpression();

    // Razèlemba primarnih izrazov
    /*
       Identifikator
       ©tevilka
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

    // Razèlemba objekta
    Expression* parseMemberExpression();

    // Razèlemba odloèitvenega stavka
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
