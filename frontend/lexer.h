#ifndef LEXER_H
#define LEXER_H

#include "Functions.h"

template <typename T> T shiftVector(std::vector<T>& vec);

std::vector<Token> tokenize(std::string sourceCode);

#endif