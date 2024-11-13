#pragma once
#include <iostream>

template<typename T>
class function;

template <typename Ret, typename... Args>
class function<Ret(Args...)> {
public:
    function() : func(nullptr) {}

    template <typename Func>
    function(Func f) : func(new FuncHolder<Func>(f)) {}


    Ret operator()(Args... args) {
        if (func) {
            return func->invoke(args...);
        } else {
            throw std::bad_function_call();
        }
    }

private:
    struct FuncBase {
        virtual Ret invoke(Args...) = 0;
        virtual ~FuncBase() {}
    };

    template <typename Func>
    struct FuncHolder : FuncBase {
        FuncHolder(Func f) : func(f) {}

        Ret invoke(Args... args) override {
            return func(args...);
        }

        Func func;
    };

    FuncBase* func;
};
