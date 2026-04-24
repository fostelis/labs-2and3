#ifndef OPTION_H
#define OPTION_H

// не выбрасываем искл а возвращаем спец контейнер
template<class T>
class option {
private:
    T value_; // хранимое значкние, всегда сущ, но мб none
    bool has_value_; // флаг, есть ли знач? т.е валидно ли оно
public:
    option() : has_value_(false) {}
    option(T val) : value_(val), has_value_(true) {}

    bool is_some() const {
        return has_value_;
    }
    bool is_none() const {
        return !has_value_;
    }
    T value() const {
        return value_;
    }

    static option<T> none() {
        return option<T>();
    }
    static option<T> some(T val) {
        return option<T>(val);
    }
};

#endif