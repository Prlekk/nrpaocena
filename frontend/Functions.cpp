#include "Functions.h"

std::string readFile(const std::string& filename) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error: Unable to open file: " << filename << std::endl;
        return "";  // Return an empty string on error
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}

std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token = "";

    for(const char& c : str) {
        tokens.push_back(std::string(1, c));
    }

    return tokens;
}

std::string shift(std::string& str) {
    std::string result = "";

    if (!str.empty()) {
        result = str[0];
        str.erase(str.begin());
    }

    return result;
}


Token token(std::string value, TokenType type) {
    return {type, value};
}

bool isBinaryOperator(char c) {
    switch (c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
            return true;
        default:
            return false;
    }
}

char toUpperCase(char c) {
    if (std::isalpha(c)) {
        return (char)(std::toupper(c));
    } else {
        return c;
    }
}

char toLowerCase(char c) {
    if (std::isalpha(c)) {
        return (char)(std::tolower(c));
    } else {
        return c;
    }
}

bool isAlpha(char c) {
    return std::isalpha(c) != 0;
}

bool isNumber(char c) {
    return (c >= '0' && c <= '9') || (c == '.');
}

std::string toUpperCase(std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = toUpperCase(c);
    }
    return result;
}

std::string toLowerCase(std::string& str) {
    std::string result = str;
    for (char& c : result) {
        c = toLowerCase(c);
    }
    return result;
}

bool isAlpha(const std::string& str) {
    for (char c : str) {
        if (!isAlpha(c)) {
            return false;
        }
    }
    return true;
}

bool isNumber(std::string& str) {
    if (str.empty()) {
        return false;
    }

    int sign = 0;
    if(str[0] == '-' || str[0] == '+') {
        sign = 1;
    }

    bool decimal = false;

    for(int i = sign; i < str.length(); i++) {
        if(!isNumber(str[i])) {
            if(str[i] == '.' && !decimal) {
                decimal = true;
            } else {
                return false;
            }
        }
    }

    return str.length() > sign;
}

bool isSkippable(char c) {
    return c == ' ' || c == '\n' || c == '\t' || c == '\0' || c == '\r';
}

std::string tokenTypeToString(Token token) {
    switch (token.type) {
        case TokenType::Number:
            return "Number";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::Equals:
            return "Equals";
        case TokenType::OpenParen:
            return "OpenParen";
        case TokenType::CloseParen:
            return "CloseParen";
        case TokenType::BinaryOperator:
            return "BinaryOperator";
        case TokenType::Let:
            return "Let";
        case TokenType::Const:
            return "Const";
        case TokenType::Fn:
            return "Fn";
        case TokenType::If:
            return "If";
        case TokenType::EndOfFile:
            return "EndOfFile";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Colon:
            return "Colon";
        case TokenType::Semicolon:
            return "Semicolon";
        case TokenType::OpenBrace:
            return "OpenBrace";
        case TokenType::CloseBrace:
            return "CloseBrace";
        case TokenType::OpenBracket:
            return "OpenBracket";
        case TokenType::CloseBracket:
            return "CloseBracket";
        case TokenType::Bang:
            return "Bang";
        case TokenType::BangEquals:
            return "BangEquals";
        case TokenType::EqualEquals:
            return "EqualEquals";
        case TokenType::LessEquals:
            return "LessEquals";
        case TokenType::GreaterEquals:
            return "GreaterEquals";
        case TokenType::Less:
            return "Less";
        case TokenType::Greater:
            return "Greater";
        default:
            return "Unknown";
    }
}

std::string tokenTypeToString(TokenType token) {
    switch (token) {
        case TokenType::Number:
            return "Number";
        case TokenType::Identifier:
            return "Identifier";
        case TokenType::String:
            return "String";
        case TokenType::Equals:
            return "Equals";
        case TokenType::OpenParen:
            return "OpenParen";
        case TokenType::CloseParen:
            return "CloseParen";
        case TokenType::BinaryOperator:
            return "BinaryOperator";
        case TokenType::Let:
            return "Let";
        case TokenType::Const:
            return "Const";
        case TokenType::Fn:
            return "Fn";
        case TokenType::If:
            return "If";
        case TokenType::Else:
            return "Else";
        case TokenType::EndOfFile:
            return "EndOfFile";
        case TokenType::Comma:
            return "Comma";
        case TokenType::Dot:
            return "Dot";
        case TokenType::Colon:
            return "Colon";
        case TokenType::Semicolon:
            return "Semicolon";
        case TokenType::OpenBrace:
            return "OpenBrace";
        case TokenType::CloseBrace:
            return "CloseBrace";
        case TokenType::OpenBracket:
            return "OpenBracket";
        case TokenType::CloseBracket:
            return "CloseBracket";
        case TokenType::Bang:
            return "Bang";
        case TokenType::BangEquals:
            return "BangEquals";
        case TokenType::EqualEquals:
            return "EqualEquals";
        case TokenType::LessEquals:
            return "LessEquals";
        case TokenType::GreaterEquals:
            return "GreaterEquals";
        case TokenType::Less:
            return "Less";
        case TokenType::Greater:
            return "Greater";
        default:
            return "Unknown";
    }
}


std::string ftos(float f) {
    std::ostringstream oss;
    oss << f;
    return oss.str();
}

template <typename T> T shiftVector(std::vector<T>& vec) {
    T frontElement = T();
    if (!vec.empty()) {
        vec.erase(vec.begin());
        frontElement = vec.front();
    }
    return frontElement;
}

Statement::Statement() : kind(), value(""), numberValue(0.0) {}

Statement::Statement(NodeType t, std::string val, float numVal) : kind(t), value(val), numberValue(numVal) {}

void Statement::setValue(std::string v) {
    value = v;
}

void Statement::setNumberValue(float v) {
    numberValue = v;
}

void Statement::setKind(NodeType k) {
    kind = k;
}

NodeType Statement::getKind() const {
    return kind;
}

std::string Statement::getValue() const {
    return value;
}

float Statement::getNumberValue() const {
    return numberValue;
}

std::string Statement::getKindName() const {
    switch (kind) {
        case NODE_PROGRAM:
            return "Program";
        case NODE_VARIABLEDECLARATION:
            return "VariableDeclaration";
        case NODE_FUNCTIONDECLARATION:
            return "FunctionDeclaration";
        case NODE_NUMERICLITERAL:
            return "NumericLiteral";
        case NODE_IDENTIFIER:
            return "Identifier";
        case NODE_BINARYEXPRESSION:
            return "BinaryExpression";
        case NODE_ASSIGNMENTEXPRESSION:
            return "AssignmentExpression";
        case NODE_PROPERTY:
            return "Property";
        case NODE_OBJECTLITERAL:
            return "ObjectLiteral";
        case NODE_IFEXPRESSION:
            return "IfExpression";
        case NODE_CALLEXPRESSION:
            return "CallExpression";
        case NODE_MEMBEREXPRESSION:
            return "MemberExpression";
        default:
            return "Unknown";
    }
}