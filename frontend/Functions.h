#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cctype>
#include <unordered_map>
#include <set>
#include <functional>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <cmath>

enum NodeType {
    // Statements
    NODE_PROGRAM,
    NODE_VARIABLEDECLARATION,
    NODE_FUNCTIONDECLARATION,
    NODE_IFEXPRESSION,

    // Expressions
    NODE_ASSIGNMENTEXPRESSION,
    NODE_MEMBEREXPRESSION,
    NODE_CALLEXPRESSION,

    // Literals
    NODE_PROPERTY,
    NODE_OBJECTLITERAL,
    NODE_NUMERICLITERAL,
    NODE_STRINGLITERAL,
    NODE_IDENTIFIER,
    NODE_BINARYEXPRESSION,
};

enum TokenType {
    // Literal Types
    Number,             // stevilka
    Identifier,         // identifikator
    String,             // niz

    // Keywords
    Let,                // rezerviraj
    Const,              // konstanta
    Fn,                 // funkcija
    If,                 // ce
    Else,               // sicer

    // Grouping * Operators
    Quotation,          // ""
    BinaryOperator,     // + - * / 
    Equals,             // =
    Comma,              // ,
    Dot,                // .
    Colon,              // :
    Semicolon,          // ;
    OpenParen,          // (
    CloseParen,         // )
    OpenBrace,          // {
    CloseBrace,         // }
    OpenBracket,        // [
    CloseBracket,       // ]
    Bang,               // !
    Less,               // <
    Greater,            // >
    BangEquals,         // !=
    EqualEquals,        // ==
    LessEquals,         // <=
    GreaterEquals,      // >=
    EndOfFile,          // EOF
};

struct Token {
    TokenType type;
    std::string value = "";
};

std::string readFile(const std::string& filename);
std::vector<std::string> split(const std::string& str);
std::string shift(std::string& str);
Token token(std::string value, TokenType type);
bool isBinaryOperator(char c);

char toUpperCase(char c);
char toLowerCase(char c);
bool isAlpha(char c);
bool isInt(char c);
std::string toUpperCase(std::string& str);
std::string toLowerCase(std::string& str);
bool isAlpha(std::string& str);
bool isInt(std::string& str);

bool isSkippable(char c);
std::string tokenTypeToString(Token token);
std::string tokenTypeToString(TokenType token);
std::string ftos(float f);

template <typename T> T shiftVector(std::vector<T>& vec);

class Statement {
private:
protected:
public:
    NodeType kind;
    std::string value;
    float numberValue;

    Statement();
    Statement(NodeType t, std::string val, float numVal);
    virtual ~Statement() = default;

    void setValue(std::string v);
    void setNumberValue(float v);
    void setKind(NodeType k);
    NodeType getKind() const;
    std::string getValue() const;
    float getNumberValue() const;
    std::string getKindName() const;
};

#endif