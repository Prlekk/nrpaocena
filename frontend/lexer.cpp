#include "lexer.h"

template <typename T> T shiftVector(std::vector<T>& vec) {
    T frontElement = {};
    if (!vec.empty()) {
        frontElement = vec.front();
        vec.erase(vec.begin());
    }
    return frontElement;
}

std::unordered_map<std::string, TokenType> keywords = {
    {"rezerviraj", Let},
    {"konstanta", Const},
    {"funkcija", Fn},
    {"ce", If},
    {"sicer", Else}
};

std::vector<Token> tokenize(std::string sourceCode) {
    std::vector<Token> tokens;
    std::vector<std::string> src = split(sourceCode);
    // std::cout << "Source code: " << sourceCode << std::endl;
    while (!src.empty()) {
        if (src[0][0] == '(') {
            tokens.emplace_back(token(shiftVector(src), TokenType::OpenParen));
        } else if (src[0][0] == ')') {
            tokens.emplace_back(token(shiftVector(src), TokenType::CloseParen));
        } else if (src[0][0] == '{') {
            tokens.emplace_back(token(shiftVector(src), TokenType::OpenBrace));
        } else if (src[0][0] == '}') {
            tokens.emplace_back(token(shiftVector(src), TokenType::CloseBrace));
        } else if (src[0][0] == '[') {
            tokens.emplace_back(token(shiftVector(src), TokenType::OpenBracket));
        } else if (src[0][0] == ']') {
            tokens.emplace_back(token(shiftVector(src), TokenType::CloseBracket));
        } else if (isBinaryOperator(src[0][0])) {
            tokens.emplace_back(token(shiftVector(src), TokenType::BinaryOperator));
        } else if (src[0][0] == '=') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Equals));
            if(src[0][0] == '=') {
                tokens.back().value += shiftVector(src);
                tokens.back().type = TokenType::EqualEquals;
            }
        } else if (src[0][0] == ';') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Semicolon));
        } else if (src[0][0] == ':') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Colon));
        } else if (src[0][0] == ',') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Comma));
        } else if (src[0][0] == '.') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Dot));
        } else if (src[0][0] == '"') {
            std::string str = "";
            shiftVector(src);
            while (src[0][0] != '"') {
                str += shiftVector(src);
            }
            shiftVector(src);
            tokens.emplace_back(token(str, TokenType::String));
        } else if (src[0][0] == '!') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Bang));
            if(src[0][0] == '=') {
                tokens.back().value += shiftVector(src);
                tokens.back().type = TokenType::BangEquals;
            }
        } else if (src[0][0] == '<') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Less));
            if(src[0][0] == '=') {
                tokens.back().value += shiftVector(src);
                tokens.back().type = TokenType::LessEquals;
            }
        } else if (src[0][0] == '>') {
            tokens.emplace_back(token(shiftVector(src), TokenType::Greater));
            if(src[0][0] == '=') {
                tokens.back().value += shiftVector(src);
                tokens.back().type = TokenType::GreaterEquals;
            }
        } else {
            // Check if a token is a number
            if (isInt(src[0][0])) {
                std::string num = "";
                while (!src.empty() && !src[0].empty() && isInt(src[0][0])) {
                    num += src[0].front();
                    src.erase(src.begin());
                }
                tokens.emplace_back(token(num, TokenType::Number));
            }
            // Check if a token is an identifier
            else if (isAlpha(src[0][0])) {
                std::string identifier = "";
                // std::cout << "Identifier: " << src[0][0] << std::endl;
                while (!src.empty() && !src[0].empty() && isAlpha(src[0][0])) {
                    // std::cout << "Shifting: " << src[0][0] << std::endl;
                    // std::cout << "Identifier: " << identifier << std::endl;
                    identifier += src[0].front();
                    src.erase(src.begin());
                    // std::cin.get();
                }
                bool isKeyword = false;
                for (const auto& pair : keywords) {
                    if(identifier == pair.first) {
                        isKeyword = true;
                        // std::cout << "Keyword: " << pair.first << std::endl;
                        tokens.emplace_back(token(identifier, pair.second));
                    }
                }
                if(!isKeyword) {
                    // std::cout << identifier << std::endl;
                    tokens.emplace_back(token(identifier, TokenType::Identifier));
                }
                // std::cout << type->second << std::endl;
                // if (it != keywords.end()) {
                    
                // 
                // } else {
                //     tokens.emplace_back(token(identifier, TokenType::Identifier));
                // 
                // }
            }
            // Check if a token is a skippable character
            else if (isSkippable(src[0][0])) {
                src.erase(src.begin());
            } else {
                exit(1);
            }
        }
    }
    tokens.emplace_back(token("EndOfFile", TokenType::EndOfFile));

    // for (const auto& token : tokens) {
    //     std::cout << "Token: " << tokenTypeToString(token.type) << " " << token.value << std::endl;
    // }

    return tokens;
}