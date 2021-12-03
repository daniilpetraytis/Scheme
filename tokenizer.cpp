#include "tokenizer.h"
#include <iostream>
#include <sstream>
#include "error.h"

bool SymbolToken::operator==(const SymbolToken &other) const {
    if (name == other.name) {
        return true;
    }
    return false;
}

bool ConstantToken::operator==(const ConstantToken &other) const {
    return value == other.value;
}

bool QuoteToken::operator==(const QuoteToken &) const {
    return true;
}

bool DotToken::operator==(const DotToken &) const {
    return true;
}

Tokenizer::Tokenizer(std::istream *in) {
    x_ = in;
    rec_.clear();
    symbol_ = true, const_ = true, minus_ = false;
    num_.clear();
    if (x_->peek() == EOF) {
        isend_ = true;
        return;
    }
    if (prev_flag_) {
        if (std::isdigit(prev_)) {
            num_ += prev_;
            const_ = true;
            prev_ = '0';
            prev_flag_ = false;
        } else if (prev_ == '\'') {
            res_ = Token{QuoteToken{}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '(') {
            res_ = Token{BracketToken::OPEN};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == ')') {
            res_ = Token{BracketToken::CLOSE};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '.') {
            res_ = Token{DotToken{}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '-') {
            minus_ = true;
            symbol_ = true;
            const_ = true;
            rec_ += prev_;
            prev_ = '0';
            prev_flag_ = false;
        } else {
            symbol_ = true;
            const_ = false;
            rec_ += prev_;
            prev_ = '0';
            prev_flag_ = false;
        }
    }
    while (x_->peek() != EOF) {
        char q = x_->get();
        if (q == '\n') {
            continue;
        }
        if (q == '\'') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{QuoteToken{}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (q == '(') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{BracketToken::OPEN};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (q == ')') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{BracketToken::CLOSE};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }

        } else if (q == '.') {
            if (rec_.empty() && num_.empty()) {
                res_ = Token{DotToken()};
                return;
            }
            if (const_) {
                if (minus_ && !num_.empty()) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{DotToken{}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (std::isdigit(q) && const_ && symbol_) {
            const_ = true;
            symbol_ = false;
            num_ += q;
        } else if (std::isdigit(q) && const_) {
            symbol_ = false;
            num_ += q;
        } else if (q == '-' && const_ && symbol_) {
            const_ = true;
            symbol_ = true;
            minus_ = true;
            rec_ += q;
        } else if (q == ' ') {
            if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            } else if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (symbol_) {
            rec_ += q;
            const_ = false;
        } else if (const_ && !symbol_) {
            if (q != '-' && q != '+') {
                throw SyntaxError("");
            }
            if (minus_) {
                res_ = Token{ConstantToken{-std::stoi(num_)}};
            } else {
                res_ = Token{ConstantToken{std::stoi(num_)}};
            }
            prev_ = q;
            prev_flag_ = true;
            return;
        }
    }
    if (x_->peek() == EOF) {
        if (symbol_ && !rec_.empty()) {
            res_ = Token{SymbolToken{rec_}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (const_ && !num_.empty()) {
            if (minus_) {
                res_ = Token{ConstantToken{-std::stoi(num_)}};
            } else {
                res_ = Token{ConstantToken{std::stoi(num_)}};
            }
            prev_ = '0';
            prev_flag_ = false;
            return;
        }
        isend_ = true;
    }
}

bool Tokenizer::IsEnd() {
    return isend_;
}

void Tokenizer::Next() {
    rec_.clear();
    symbol_ = true, const_ = true, minus_ = false;
    num_.clear();
    if (prev_flag_) {
        if (std::isdigit(prev_)) {
            num_ += prev_;
            const_ = true;
            prev_ = '0';
            prev_flag_ = false;
        } else if (prev_ == '\'') {
            res_ = Token{QuoteToken{}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '(') {
            res_ = Token{BracketToken::OPEN};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == ')') {
            res_ = Token{BracketToken::CLOSE};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '.') {
            res_ = Token{DotToken{}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (prev_ == '-') {
            minus_ = true;
            symbol_ = true;
            const_ = true;
            rec_ += prev_;
            prev_ = '0';
            prev_flag_ = false;
        } else {
            symbol_ = true;
            const_ = false;
            rec_ += prev_;
            prev_ = '0';
            prev_flag_ = false;
        }
    }
    while (x_->peek() != EOF) {
        char q = x_->get();
        if (q == '\n') {
            continue;
        }
        if (q == '\'') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{QuoteToken{}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (q == '(') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{BracketToken::OPEN};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (q == ')') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{BracketToken::CLOSE};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }

        } else if (q == '.') {
            if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = q;
                prev_flag_ = true;
                return;
            } else if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = q;
                prev_flag_ = true;
                return;
            } else {
                res_ = Token{DotToken{}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (std::isdigit(q) && const_ && symbol_) {
            const_ = true;
            symbol_ = false;
            num_ += q;
        } else if (std::isdigit(q) && const_) {
            symbol_ = false;
            num_ += q;
        } else if (q == '-' && const_ && symbol_) {
            const_ = true;
            symbol_ = true;
            minus_ = true;
            rec_ += q;
        } else if (q == ' ') {
            if (symbol_ && !rec_.empty()) {
                res_ = Token{SymbolToken{rec_}};
                prev_ = '0';
                prev_flag_ = false;
                return;
            } else if (const_ && !num_.empty()) {
                if (minus_) {
                    res_ = Token{ConstantToken{-std::stoi(num_)}};
                } else {
                    res_ = Token{ConstantToken{std::stoi(num_)}};
                }
                prev_ = '0';
                prev_flag_ = false;
                return;
            }
        } else if (symbol_) {
            rec_ += q;
            const_ = false;
        } else if (const_ && !symbol_) {
            if (q != '-' && q != '+') {
                throw SyntaxError("");
            }
            if (minus_) {
                res_ = Token{ConstantToken{-std::stoi(num_)}};
            } else {
                res_ = Token{ConstantToken{std::stoi(num_)}};
            }
            prev_ = q;
            prev_flag_ = true;
            return;
        }
    }
    if (x_->peek() == EOF) {
        if (symbol_ && !rec_.empty()) {
            res_ = Token{SymbolToken{rec_}};
            prev_ = '0';
            prev_flag_ = false;
            return;
        } else if (const_ && !num_.empty()) {
            if (minus_) {
                res_ = Token{ConstantToken{-std::stoi(num_)}};
            } else {
                res_ = Token{ConstantToken{std::stoi(num_)}};
            }
            prev_ = '0';
            prev_flag_ = false;
            return;
        }
        isend_ = true;
    }
}

Token Tokenizer::GetToken() {
    return res_;
}
