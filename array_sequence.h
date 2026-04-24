#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "sequence.h"
#include "dynamic_array.h"
#include "linked_list.h"
#include "sequence_crtp.h"

template<class T>
class mutable_array_sequence;

template<class T>
class array_sequence : public sequence<T> {
protected:
    dynamic_array<T>* items_;
    virtual array_sequence<T>* instance() = 0;

public:
    array_sequence() : items_(new dynamic_array<T>()) {}
    array_sequence(const T* items, int count) : items_(new dynamic_array<T>(items, count)) {}
    array_sequence(const dynamic_array<T>& arr) : items_(new dynamic_array<T>(arr)) {}
    array_sequence(const linked_list<T>& list) : items_(new dynamic_array<T>()) {
        for (int i = 0; i < list.get_length(); i++) {
            items_->resize(items_->get_size() + 1);
            items_->set(items_->get_size() - 1, list.get(i));
        }
    }
    array_sequence(const array_sequence& other) : items_(new dynamic_array<T>(*other.items_)) {}
    virtual ~array_sequence() {
        delete items_;
    }

    T get_first() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(0);
    }
    T get_last() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(items_->get_size() - 1);
    }
    T get(int index) const override {
        return items_->get(index);
    }

    array_sequence<T>* get_subsequence(int start_index, int end_index) const override {
        if (start_index < 0 || end_index >= items_->get_size() || start_index > end_index) {
            throw index_out_of_range_exception("invalid indices");
        }
        T* sub = new T[end_index - start_index + 1];
        for (int i = start_index; i <= end_index; i++) {
            sub[i - start_index] = items_->get(i);
        }
        array_sequence<T>* result = new mutable_array_sequence<T>(sub, end_index - start_index + 1);
        delete[] sub;
        return result;
    }

    int get_length() const override {
        return items_->get_size();
    }

    virtual array_sequence<T>* append_impl(const T& item) = 0;
    virtual array_sequence<T>* prepend_impl(const T& item) = 0;
    virtual array_sequence<T>* insert_at_impl(const T& item, int index) = 0;

    array_sequence<T>* concat(const sequence<T>* other) override {
        array_sequence<T>* result = instance();
        for (int i = 0; i < other->get_length(); i++) {
            result = result->append_impl(other->get(i));
        }
        return result;
    }
};
//изменяемая версия
template<class T>
class mutable_array_sequence : public array_sequence<T> {
public:
    mutable_array_sequence() : array_sequence<T>() {}
    mutable_array_sequence(const T* items, int count) : array_sequence<T>(items, count) {}
    mutable_array_sequence(const dynamic_array<T>& arr) : array_sequence<T>(arr) {}
    mutable_array_sequence(const linked_list<T>& list) : array_sequence<T>(list) {}
    mutable_array_sequence(const array_sequence<T>& other) : array_sequence<T>(other) {}

    array_sequence<T>* append(const T& item) override {
        return append_impl(item);
    }
    array_sequence<T>* prepend(const T& item) override {
        return prepend_impl(item);
    }
    array_sequence<T>* insert_at(const T& item, int index) override {
        return insert_at_impl(item, index);
    }

    array_sequence<T>* clone() const override {
        return new mutable_array_sequence<T>(*this);
    }

protected:
    array_sequence<T>* instance() override {
        return this;
    }//указатель на себя, не создает новый объект

    array_sequence<T>* append_impl(const T& item) override {
        this->items_->resize(this->items_->get_size() + 1);
        this->items_->set(this->items_->get_size() - 1, item);
        return this;
    }

    array_sequence<T>* prepend_impl(const T& item) override {
        return insert_at_impl(item, 0);
    }

    array_sequence<T>* insert_at_impl(const T& item, int index) override {
        if (index < 0 || index > this->items_->get_size()) {
            throw index_out_of_range_exception("index out of range");
        }
        int old_size = this->items_->get_size();
        this->items_->resize(old_size + 1);
        for (int i = old_size; i > index; i--) {
            this->items_->set(i, this->items_->get(i - 1));
        }
        this->items_->set(index, item);
        return this;
    }
};
//неизменяемая версия
template<class T>
class immutable_array_sequence : public array_sequence<T> {
public:
    immutable_array_sequence() : array_sequence<T>() {}
    immutable_array_sequence(const T* items, int count) : array_sequence<T>(items, count) {}
    immutable_array_sequence(const dynamic_array<T>& arr) : array_sequence<T>(arr) {}
    immutable_array_sequence(const linked_list<T>& list) : array_sequence<T>(list) {}
    immutable_array_sequence(const array_sequence<T>& other) : array_sequence<T>(other) {}

    array_sequence<T>* append(const T& item) override {
        return append_impl(item);
    }
    array_sequence<T>* prepend(const T& item) override {
        return prepend_impl(item);
    }
    array_sequence<T>* insert_at(const T& item, int index) override {
        return insert_at_impl(item, index);
    }

    array_sequence<T>* clone() const override {
        return new immutable_array_sequence<T>(*this);
    }

protected:
    array_sequence<T>* instance() override {
        return new immutable_array_sequence<T>(*this);
    }//создает новый объект - копия текущего

    array_sequence<T>* append_impl(const T& item) override {
        immutable_array_sequence<T>* copy = new immutable_array_sequence<T>(*this);
        copy->items_->resize(copy->items_->get_size() + 1);
        copy->items_->set(copy->items_->get_size() - 1, item);
        return copy;
    }

    array_sequence<T>* prepend_impl(const T& item) override {
        return insert_at_impl(item, 0);
    }

    array_sequence<T>* insert_at_impl(const T& item, int index) override {
        if (index < 0 || index > this->items_->get_size()) {
            throw index_out_of_range_exception("index out of range");
        }
        immutable_array_sequence<T>* copy = new immutable_array_sequence<T>(*this);
        int old_size = copy->items_->get_size();
        copy->items_->resize(old_size + 1);
        for (int i = old_size; i > index; i--) {
            copy->items_->set(i, copy->items_->get(i - 1));
        }
        copy->items_->set(index, item);
        return copy;
    }
};

template<class T>
class mutable_array_sequence_crtp : public sequence_crtp<T, mutable_array_sequence_crtp<T>> {
private:
    dynamic_array<T>* items_;

public:
    mutable_array_sequence_crtp() : items_(new dynamic_array<T>()) {}
    mutable_array_sequence_crtp(const T* items, int count) : items_(new dynamic_array<T>(items, count)) {}
    mutable_array_sequence_crtp(const mutable_array_sequence_crtp& other) : items_(new dynamic_array<T>(*other.items_)) {}
    ~mutable_array_sequence_crtp() {
        delete items_;
    }

    T get_first() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(0);
    }
    T get_last() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(items_->get_size() - 1);
    }
    T get(int index) const override {
        return items_->get(index);
    }

    mutable_array_sequence_crtp* get_subsequence(int start_index, int end_index) const override {
        if (start_index < 0 || end_index >= items_->get_size() || start_index > end_index) {
            throw index_out_of_range_exception("invalid indices");
        }
        T* sub = new T[end_index - start_index + 1];
        for (int i = start_index; i <= end_index; i++) {
            sub[i - start_index] = items_->get(i);
        }
        mutable_array_sequence_crtp* result = new mutable_array_sequence_crtp(sub, end_index - start_index + 1);
        delete[] sub;
        return result;
    }

    int get_length() const override {
        return items_->get_size();
    }

    sequence<T>* append_impl(const T& item) override {
        items_->resize(items_->get_size() + 1);
        items_->set(items_->get_size() - 1, item);
        return this;
    }

    sequence<T>* prepend_impl(const T& item) override {
        return insert_at_impl(item, 0);
    }

    sequence<T>* insert_at_impl(const T& item, int index) override {
        if (index < 0 || index > items_->get_size()) {
            throw index_out_of_range_exception("index out of range");
        }
        int old_size = items_->get_size();
        items_->resize(old_size + 1);
        for (int i = old_size; i > index; i--) {
            items_->set(i, items_->get(i - 1));
        }
        items_->set(index, item);
        return this;
    }

    sequence<T>* concat_impl(const sequence<T>* other) override {
        for (int i = 0; i < other->get_length(); i++) {
            append_impl(other->get(i));
        }
        return this;
    }

    sequence<T>* clone() const override {
        return new mutable_array_sequence_crtp(*this);
    }
};

template<class T>
class immutable_array_sequence_crtp : public sequence_crtp<T, immutable_array_sequence_crtp<T>> {
private:
    dynamic_array<T>* items_;

public:
    immutable_array_sequence_crtp() : items_(new dynamic_array<T>()) {}
    immutable_array_sequence_crtp(const T* items, int count) : items_(new dynamic_array<T>(items, count)) {}
    immutable_array_sequence_crtp(const immutable_array_sequence_crtp& other) : items_(new dynamic_array<T>(*other.items_)) {}
    ~immutable_array_sequence_crtp() {
        delete items_;
    }

    T get_first() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(0);
    }
    T get_last() const override {
        if (items_->get_size() == 0) {
            throw empty_collection_exception("empty");
        }
        return items_->get(items_->get_size() - 1);
    }
    T get(int index) const override {
        return items_->get(index);
    }

    immutable_array_sequence_crtp* get_subsequence(int start_index, int end_index) const override {
        if (start_index < 0 || end_index >= items_->get_size() || start_index > end_index) {
            throw index_out_of_range_exception("invalid indices");
        }
        T* sub = new T[end_index - start_index + 1];
        for (int i = start_index; i <= end_index; i++) {
            sub[i - start_index] = items_->get(i);
        }
        immutable_array_sequence_crtp* result = new immutable_array_sequence_crtp(sub, end_index - start_index + 1);
        delete[] sub;
        return result;
    }

    int get_length() const override {
        return items_->get_size();
    }

    sequence<T>* append_impl(const T& item) override {
        immutable_array_sequence_crtp* copy = new immutable_array_sequence_crtp(*this);
        copy->items_->resize(copy->items_->get_size() + 1);
        copy->items_->set(copy->items_->get_size() - 1, item);
        return copy;
    }

    sequence<T>* prepend_impl(const T& item) override {
        return insert_at_impl(item, 0);
    }

    sequence<T>* insert_at_impl(const T& item, int index) override {
        if (index < 0 || index > items_->get_size()) {
            throw index_out_of_range_exception("index out of range");
        }
        immutable_array_sequence_crtp* copy = new immutable_array_sequence_crtp(*this);
        int old_size = copy->items_->get_size();
        copy->items_->resize(old_size + 1);
        for (int i = old_size; i > index; i--) {
            copy->items_->set(i, copy->items_->get(i - 1));
        }
        copy->items_->set(index, item);
        return copy;
    }

    sequence<T>* concat_impl(const sequence<T>* other) override {
        immutable_array_sequence_crtp* result = new immutable_array_sequence_crtp(*this);
        for (int i = 0; i < other->get_length(); i++) {
            immutable_array_sequence_crtp* new_result =
                dynamic_cast<immutable_array_sequence_crtp*>(result->append_impl(other->get(i)));
            if (new_result != result) {
                delete result;
                result = new_result;
            }
        }
        return result;
    }

    sequence<T>* clone() const override {
        return new immutable_array_sequence_crtp(*this);
    }
};

#endif