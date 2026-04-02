#pragma once

#include "ArraySequence.h"

template <class T>
class MutableArraySequence : public ArraySequence<T> {
protected:
    // Возвращается изначальный, но уже измененный объект
    ArraySequence<T>* Instance() override {
        return this;
    }

public:
    MutableArraySequence() : ArraySequence<T>() { }

    MutableArraySequence( T* data, int count ) : ArraySequence<T>( data, count ) { }

    MutableArraySequence( const ArraySequence<T>& other ) : ArraySequence<T>( other ) { }

    Sequence<T>* CreateEmpty() const override {
        return new MutableArraySequence<T>();
    }
};
