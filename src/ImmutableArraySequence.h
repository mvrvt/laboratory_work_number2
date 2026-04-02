#pragma once

#include "ArraySequence.h"

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
protected:
    // Возвращается новый объект
    ArraySequence<T>* Instance() override {
        return new ImmutableArraySequence<T>( *this );
    }

public:
    ImmutableArraySequence() : ArraySequence<T>() { }

    ImmutableArraySequence( T* data, int count ) : ArraySequence<T>( data, count ) { }

    ImmutableArraySequence( const ArraySequence<T>& other ) : ArraySequence<T>( other ) { }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }
};
