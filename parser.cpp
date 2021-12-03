#include "parser.h"
#include "object.h"
#include "error.h"
#include <iostream>

int count = 0;
int check = 0;

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        count = 0;
        check = 0;
        throw SyntaxError("");
    } else {
        auto elem = tokenizer->GetToken();
        if (elem == Token{BracketToken::OPEN}) {
            ++count;
            ++check;
            tokenizer->Next();
            return ReadList(tokenizer);
        } else if (std::get_if<QuoteToken>(&elem)) {
            throw SyntaxError("");
        } else if (std::get_if<ConstantToken>(&elem)) {
            ConstantToken* x = std::get_if<ConstantToken>(&elem);
            tokenizer->Next();
            return std::make_shared<Number>(Number{x->value});
        } else if (std::get_if<SymbolToken>(&elem)) {
            SymbolToken* x = std::get_if<SymbolToken>(&elem);
            tokenizer->Next();
            return std::make_shared<Symbol>(Symbol{x->name});
        } else {
            throw SyntaxError("");
        }
    }
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    Cell res;
    res.Set1(nullptr);
    res.Set2(nullptr);
    while (!tokenizer->IsEnd() && tokenizer->GetToken() != Token{BracketToken::CLOSE}) {
        if (tokenizer->GetToken() == Token{DotToken()}) {
            if (res.GetFirst() == nullptr) {
                throw SyntaxError("");
            } else {
                tokenizer->Next();
                res.Set2(Read(tokenizer));
            }
        } else {
            if (res.GetFirst() == nullptr) {
                res.Set1(Read(tokenizer));
            } else {
                res.Set2(ReadList(tokenizer));
            }
        }
    }
    if (tokenizer->IsEnd()) {
        if (count == 0 && res.GetFirst() == nullptr && res.GetSecond() == nullptr) {
            count = 0;
            check = 0;
            return nullptr;
        }
        if (count == 0) {
            check = 0;
            return std::make_shared<Cell>(res);
        } else {
            check = 0;
            count = 0;
            throw SyntaxError("");
        }
    } else {
        --count;
        tokenizer->Next();
        if (res.GetFirst() == nullptr && res.GetSecond() == nullptr && check != 1 && count == 0) {
            return std::make_shared<Cell>(res);
        }
        if (res.GetFirst() == nullptr && res.GetSecond() == nullptr) {
            return nullptr;
        }
        return std::make_shared<Cell>(res);
    }
}
