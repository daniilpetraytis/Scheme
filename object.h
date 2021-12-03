#pragma once

#include <memory>
#include <string>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
public:
    int GetValue() const {
        return x_;
    }
    Number(long long int x) {
        x_ = x;
    }

private:
    long long x_;
};

class Symbol : public Object {
public:
    const std::string& GetName() const {
        return s_;
    }
    Symbol(std::string s) {
        s_ = s;
    }

private:
    std::string s_;
};

class Cell : public Object {
public:
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }
    void Set1(std::shared_ptr<Object> x) {
        first_ = x;
    }
    void Set2(std::shared_ptr<Object> x) {
        second_ = x;
    }

private:
    std::shared_ptr<Object> first_ = nullptr;
    std::shared_ptr<Object> second_ = nullptr;
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    auto x = dynamic_cast<T&>(*obj);
    return true;
}
