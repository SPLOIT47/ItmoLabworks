#pragma once
#include <typeinfo>
#include <iostream>

class any {
public:
    any() : _value(nullptr) {}

    any(const any& other) : _value(other._value ? other._value->clone() : nullptr) {}

    template<typename T>
    any(const T& value) : _value(new holder<T>(value)) {}

    any& operator=(const any& other) {
        if (this != &other) {
            any temp(other);
            std::swap(_value, temp._value);
        }
        return *this;
    }

    template<typename T>
    any& operator=(const T& value) {
        any temp(value);
        std::swap(_value, temp._value);
        return *this;
    }

    const auto& type() const {
        return _value ? _value->type() : typeid(nullptr);
    }

    bool has_value() const {
        return _value != nullptr;
    }

    template<typename T>
    T any_cast() const {
        if (type() == typeid(T)) {
            return static_cast<T>(static_cast<holder<T>*>(_value)->_value);
        } else {
            throw std::bad_cast();
        }
    }

    template<typename T>
    operator T() const {
        return any_cast<T>();
    }

    void stream(std::ostream& os) const {
        if (_value) {
            _value->stream(os);
        }
    }

    void* raw() {
        return _value->raw_pointer();
    }

private:
    class placeholder {
    public:
        placeholder() {}
        virtual ~placeholder() {};
        virtual const std::type_info& type() const = 0;
        virtual placeholder* clone() const = 0;
        virtual void stream(std::ostream& os) const = 0;
        virtual void* raw_pointer() = 0;
    };

    template<typename T>
    class holder : public placeholder {
    public:
        holder() : _value() {}
        holder(const T& value) : _value(value) {}

        virtual const std::type_info& type() const override {
            return typeid(T);
        }

        virtual placeholder* clone() const override{
            return new holder(_value);
        }

        virtual void stream(std::ostream& os) const override {
            os << _value;
        }
        
        virtual void* raw_pointer() override {
            return &_value;
        }

        T _value;
    };

    placeholder* _value;
};

template<typename T>
T any_cast(const any& value) {
    return value.any_cast<T>();
}

std::ostream& operator<<(std::ostream& os, const any& value) {
    value.stream(os);
    return os;
}
