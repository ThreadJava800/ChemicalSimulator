#ifndef _LISTpp_h_
#define _LISTpp_h_

#include <iostream>

#include "../src/objects/objects.h"

#ifndef ON_ERROR
#define ON_ERROR(expr, errStr, retVal) {                 \
    if (expr) {                                           \
        fprintf(stderr, "FATAL ERROR: %s\n", errStr);      \
        return retVal;                                      \
    }                                                        \
}                                                             \

#endif

typedef BaseMolecule* Elem_t;

class List {
private:
    Elem_t* values   = nullptr;
    size_t  capacity = 0;
    size_t  size     = 0;

public:
    List() :
        values  (nullptr),
        capacity(1),
        size    (0)
        {
            values = (Elem_t*) calloc(sizeof(Elem_t), 1);
            ON_ERROR(!values, "Unable to allocate memory",);
        }

    List(size_t _capacity) :
        capacity(_capacity),
        size    (0)
        {
            values = (Elem_t*) calloc(sizeof(Elem_t), _capacity);
            ON_ERROR(!values, "Unable to allocate memory",);
        }

    ~List() {
        delete[] values;

        capacity = 0;
        size     = 0;
        values   = nullptr;
    }

    size_t getCapacity() {
        return capacity;
    }

    size_t getSize() {
        return size;
    }

    void pushBack(Elem_t elem) {
        ON_ERROR(!values, "List was null",);

        if (capacity <= size) {
            values = (Elem_t*) realloc(values, capacity * 2 * sizeof(Elem_t));
            ON_ERROR(!values, "Unable to realloc mem",);

            capacity = 2 * capacity;
        }

        values[size] = elem;
        size++;
    }

    void remove(size_t index) {
        ON_ERROR(!values, "List was null",);

        for (size_t i = index; i < size; i++) {
            values[i] = values[i + 1];
        }

        if (size < capacity / 2 - 1) {
            values = (Elem_t*) realloc(values, capacity / 2 * sizeof(Elem_t));
            ON_ERROR(!values, "Unable to realloc mem",);

            capacity /= 2;
        }
        size--;
    }

    Elem_t operator[](const size_t index) {
        ON_ERROR(!values, "List was null", nullptr);

        return values[index];
    }

    void updateElem(Elem_t elem, size_t index) {
        ON_ERROR(!values, "List was null",);

        values[index] = elem;
    }

    void print() {
        for (size_t i = 0; i < size; i++) {
            std::cout << values[i] << ' ';
        }
        std::cout << '\n';
    }
};

#endif