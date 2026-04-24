#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "option.h"
#include "exceptions.h"
#include <functional>
#include <utility>
#include <iostream>

template<class T>
class mutable_array_sequence;

template<class T>
class sequence {
public:
    virtual ~sequence() = default;
//чисто виртуальные методы
    virtual T get_first() const = 0;
    virtual T get_last() const = 0;
    virtual T get(int index) const = 0;
    virtual sequence<T>* get_subsequence(int start_index, int end_index) const = 0;
    virtual int get_length() const = 0;

    virtual sequence<T>* append(const T& item) = 0;
    virtual sequence<T>* prepend(const T& item) = 0;
    virtual sequence<T>* insert_at(const T& item, int index) = 0;
    virtual sequence<T>* concat(const sequence<T>* other) = 0;

    template<class T2>
    sequence<T2>* map(T2 (*func)(const T&)) {
        sequence<T2>* result = new mutable_array_sequence<T2>();
        for (int i = 0; i < get_length(); i++) {
            result = result->append(func(get(i)));
        }
        return result;
    }

    template<class T2>
    sequence<T2>* map(std::function<T2(const T&)> func) {
        sequence<T2>* result = new mutable_array_sequence<T2>();
        for (int i = 0; i < get_length(); i++) {
            result = result->append(func(get(i)));
        }
        return result;
    } //с оберткой для любого вызываемого объекта

    sequence<T>* where(std::function<bool(const T&)> predicate) {
        sequence<T>* result = new mutable_array_sequence<T>();
        for (int i = 0; i < get_length(); i++) {
            T item = get(i);
            if (predicate(item)) {
                result = result->append(item);
            }
        }
        return result;
    }

    sequence<T>* where(bool (*predicate)(const T&)) {
        sequence<T>* result = new mutable_array_sequence<T>();
        for (int i = 0; i < get_length(); i++) {
            T item = get(i);
            if (predicate(item)) {
                result = result->append(item);
            }
        }
        return result;
    }

    template<class T2>
    T2 reduce(std::function<T2(const T2&, const T&)> func, const T2& initial) {
        T2 result = initial;
        for (int i = 0; i < get_length(); i++) {
            result = func(result, get(i));
        }
        return result;
    }

    template<class T2>
    T2 reduce(T2 (*func)(const T2&, const T&), const T2& initial) {
        T2 result = initial;
        for (int i = 0; i < get_length(); i++) {
            result = func(result, get(i));
        }
        return result;
    }

    option<T> try_get_first(std::function<bool(const T&)> predicate = nullptr) {
        for (int i = 0; i < get_length(); i++) {
            T item = get(i);
            if (predicate == nullptr || predicate(item)) {
                return option<T>::some(item);
            }
        }
        return option<T>::none();
    }

    template<class T2>
    sequence<std::pair<T, T2>>* zip(const sequence<T2>* other) const {
        sequence<std::pair<T, T2>>* result = new mutable_array_sequence<std::pair<T, T2>>();
        int min_len = std::min(get_length(), other->get_length());
        for (int i = 0; i < min_len; i++) {
            result = result->append(std::make_pair(get(i), other->get(i)));
        }
        return result;
    } //объединяет 2 послед в послед пар

    sequence<T>* skip(int count) const {
        if (count < 0) {
            count = 0;
        }
        if (count >= get_length()) {
            return new mutable_array_sequence<T>();
        }
        sequence<T>* result = new mutable_array_sequence<T>();
        for (int i = count; i < get_length(); i++) {
            result = result->append(get(i));
        }
        return result;
    }

    sequence<T>* take(int count) const {
        if (count < 0) {
            count = 0;
        }
        if (count > get_length()) {
            count = get_length();
        }
        sequence<T>* result = new mutable_array_sequence<T>();
        for (int i = 0; i < count; i++) {
            result = result->append(get(i));
        }
        return result;
    }

    template<class T2>
    sequence<T2>* map_with_index(T2 (*func)(const T&, int)) const {
        sequence<T2>* result = new mutable_array_sequence<T2>();
        for (int i = 0; i < get_length(); i++) {
            result = result->append(func(get(i), i));
        }
        return result;
    }

    template<class T2>
    sequence<T2>* map_with_index(std::function<T2(const T&, int)> func) const {
        sequence<T2>* result = new mutable_array_sequence<T2>();
        for (int i = 0; i < get_length(); i++) {
            result = result->append(func(get(i), i));
        }
        return result;
    }

    sequence<sequence<T>*>* split(std::function<bool(const T&)> delimiter) const {
        sequence<sequence<T>*>* result = new mutable_array_sequence<sequence<T>*>();
        sequence<T>* current = new mutable_array_sequence<T>();
        for (int i = 0; i < get_length(); i++) {
            T item = get(i);
            if (delimiter(item)) {
                result = result->append(current);
                current = new mutable_array_sequence<T>();
            }
            else {
                current = current->append(item);
            }
        }
        if (current->get_length() > 0) {
            result = result->append(current);
        }
        else {
            delete current;
        }
        return result;
    }

    sequence<T>* slice(int index, int count, const sequence<T>* replacement = nullptr) {
        if (index < 0) {
            index = get_length() + index;
        }
        if (index < 0 || index > get_length()) {
            throw index_out_of_range_exception("invalid index");
        }
        if (count < 0) {
            count = 0;
        }
        sequence<T>* result = new mutable_array_sequence<T>();
        for (int i = 0; i < index; i++) {
            result = result->append(get(i));
        }
        if (replacement != nullptr) {
            for (int i = 0; i < replacement->get_length(); i++) {
                result = result->append(replacement->get(i));
            }
        }
        for (int i = index + count; i < get_length(); i++) {
            result = result->append(get(i));
        }
        return result;
    }

    virtual sequence<T>* clone() const = 0;
};

template<class T>
bool operator==(const sequence<T>& lhs, const sequence<T>& rhs) {
    if (lhs.get_length() != rhs.get_length()) {
        return false;
    }
    for (int i = 0; i < lhs.get_length(); i++) {
        if (lhs.get(i) != rhs.get(i)) {
            return false;
        }
    }
    return true;
}

template<class T>
bool operator!=(const sequence<T>& lhs, const sequence<T>& rhs) {
    return !(lhs == rhs);
}

template<class T>
sequence<T>* operator+(const sequence<T>& lhs, const sequence<T>& rhs) {
    sequence<T>* result = lhs.clone();
    sequence<T>* concat_result = result->concat(&rhs);
    if (concat_result != result) {
        delete result;
    }
    return concat_result;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const sequence<T>& seq) {
    os << "{";
    for (int i = 0; i < seq.get_length(); i++) {
        os << seq.get(i);
        if (i < seq.get_length() - 1) {
            os << ", ";
        }
    }
    os << "}";
    return os;
}

#endif