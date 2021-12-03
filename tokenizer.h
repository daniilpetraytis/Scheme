#pragma once

#include <variant>
#include <optional>
#include <istream>
#include "object.h"

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const;
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};

struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    long long int value;
    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);

    bool IsEnd();

    void Next();

    Token GetToken();

private:
    std::string rec_;
    bool isend_ = false;
    std::istream* x_;
    Token res_;
    bool symbol_ = false, const_ = false, minus_ = false;
    std::string num_;
    char prev_ = '0';
    bool prev_flag_ = false;
};
