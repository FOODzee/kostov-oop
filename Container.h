#ifndef KOSTOV_OOP_CONTAINER_H
#define KOSTOV_OOP_CONTAINER_H


#include <cassert>
#include <exception>
#include <functional>

template<typename T>
class Container {
public:
    void addFirst(const T& value);
    void addLast(const T& value);
    void delFirst();
    void delLast();
    const T& getFirst() const;
    const T& getLast() const;

    int  getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    void clear() {
        while (!isEmpty()) {
            delFirst();
        }
    }

    /** Apply specified function to every element of Container. */
    void forEach(std::function<void(const T&)> action, int skipFirstN = 0) const;

    /**
     * @param deleter function to be used to free element
     *                when removing it from Container, if needed.
     */
    Container(std::function<void(T)> deleter = nullptr) :
            first(nullptr),
            last(nullptr),
            size(0),
            deleter(deleter)
    {}
    Container(const Container& c) = default;

    ~Container() {
        clear();
    }

private:
    class Elem {
        Elem(const T& value) :
                next(nullptr),
                prev(nullptr),
                value(value)
        {}

        Elem* next;
        Elem* prev;
        const T value;

        friend class Container;
    };

    Elem* first;
    Elem* last;
    int size;

    std::function<void(T)> deleter;
};

class EmptyContainerException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Container is empty!";
    }
};


template<typename T>
void Container<T>::addFirst(const T& value) {
    auto e = new Elem(value);
    if (size == 0) {
        assert(first == nullptr && last == nullptr);
        first = last = e;
    } else {
        assert(first != nullptr && first->prev == nullptr);
        e->next = first;
        first->prev = e;
        first = e;
    }
    size++;
}

template<typename T>
void Container<T>::addLast(const T& value) {
    auto e = new Elem(value);
    if (size == 0) {
        assert(first == nullptr && last == nullptr);
        first = last = e;
    } else {
        assert(last != nullptr && last->next == nullptr);
        e->prev = last;
        last->next = e;
        last = e;
    }
    size++;
}


template<typename T>
void Container<T>::delFirst() {
    if (size == 0) throw EmptyContainerException();
    size--;
    auto e = first;

    first = first->next;
    if (size != 0) {
        first->prev = nullptr;
    } else {
        last = nullptr;
    }

    if (deleter != nullptr) {
        deleter(e->value);
    }
    delete e;
}

template<typename T>
void Container<T>::delLast() {
    if (size == 0) throw EmptyContainerException();
    size--;
    auto e = last;

    last = last->prev;
    if (size != 0) {
        last->next = nullptr;
    } else {
        first = nullptr;
    }

    if (deleter != nullptr) {
        deleter(e->value);
    }
    delete e;
}


template<typename T>
const T& Container<T>::getFirst() const {
    if (size == 0) throw EmptyContainerException();
    return first->value;
}

template<typename T>
const T& Container<T>::getLast() const {
    if (size == 0) throw EmptyContainerException();
    return last->value;
}


template<typename T>
void Container<T>::forEach(std::function<void(const T&)> action, int skipFirstN) const {
    for (auto p = first; p != nullptr; p = p->next) {
        if (skipFirstN-- > 0) continue;

        action(p->value);
    }
}


#endif //KOSTOV_OOP_CONTAINER_H
