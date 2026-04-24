#ifndef BIT_H
#define BIT_H

class bit {
private:
    bool value_;
public:
    bit() : value_(false) {}
    bit(bool val) : value_(val) {}
    bit(int val) : value_(val != 0) {}

    bool get_value() const {
        return value_;
    }
    void set_value(bool val) {
        value_ = val;
    }

    bit operator&(const bit& other) const {
        return bit(value_ && other.value_);//и
    }
    bit operator|(const bit& other) const {
        return bit(value_ || other.value_);//или
    }
    bit operator^(const bit& other) const {
        return bit(value_ != other.value_);//xor
    }
    bit operator!() const {
        return bit(!value_);//не
    }

    bool operator==(const bit& other) const {
        return value_ == other.value_;//операторы сравнения
    }
    bool operator!=(const bit& other) const {
        return value_ != other.value_;
    }
};

#endif