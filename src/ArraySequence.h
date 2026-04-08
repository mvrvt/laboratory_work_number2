#pragma once

#include "Sequence.h"
#include "DynamicArray.h"

// forward declaration
template <class T> class MutableArraySequence;

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items_; //TODO здесь хранить size_ последовательности,

    //TODO у ар сек есть свой сайз, который является
    //TODO у dyn arr есть только size
    // Ключевой метод паттерна mutable/immutable
    // Mutable вернёт this, Immutable вернёт Clone()
    virtual ArraySequence<T>* Instance() = 0;

    ArraySequence<T>* AppendInternal( const T& item ) {
        items_->Resize( items_->GetSize() + 1 );
        items_->Set( items_->GetSize() - 1, item );
        return this;
    }

    ArraySequence<T>* PrependInternal( const T& item ) {
        items_->Resize( items_->GetSize() + 1 );
        for ( int idx = items_->GetSize() - 1; idx > 0; --idx ) {
            items_->Set( idx, items_->Get( idx-1 ) );
        }
        items_->Set( 0, item );
        return this;
    }

    ArraySequence<T>* InsertAtInternal( const T& item, int index ) {
        if ( index < 0 || index > items_->GetSize() ) {
            throw IndexOutOfRange( "ArraySequence: index out of range" );
        }
        items_->Resize( items_->GetSize() + 1 );
        for ( int idx = items_->GetSize() - 1; idx > index; --idx ) {
            items_->Set( idx, items_->Get( idx-1 ) );
        }
        items_->Set( index, item );
        return this;
    }

public:
    ArraySequence() : items_( new DynamicArray<T> ( 0 ) ) { }

    ArraySequence( T* data, int count ) : items_( new DynamicArray<T>( data, count ) ) { }

    ArraySequence( const ArraySequence<T>& other ) : items_( new DynamicArray<T>( *other.items_ ) ) { }

    ~ArraySequence() override {
        delete items_;
    }

    // Декомпозиция
    T GetFirst() const override {
        if ( items_->GetSize() == 0 ) {
            throw IndexOutOfRange( "ArraySequence: sequence is empty" );
        }
        return items_->Get( 0 );
    }

    T GetLast() const override {
        if ( items_->GetSize() == 0 ) {
            throw IndexOutOfRange( "ArraySequence: sequence is empty" );
        }
        return items_->Get( items_->GetSize() - 1 );
    }

    T Get( int index ) const override {
        return items_->Get( index );
    }

    int GetLength() const override {
        return items_->GetSize();
    }

    Sequence<T>* GetSubsequence( int start, int end ) const override {
        if ( start < 0 || start >= items_->GetSize() ) {
            throw IndexOutOfRange( "ArraySequence: start index out of range" );
        }
        if ( end < 0 || end >= items_->GetSize() ) {
            throw IndexOutOfRange( "ArraySequence: end index out of range" );
        }
        if ( start > end ) {
            throw std::invalid_argument( "ArraySequence: start > end" );
        }

        Sequence<T>* result = this->CreateEmpty();
        for ( int idx = start; idx <= end; ++idx ) {
            Sequence<T>* next = result->Append( items_->Get( idx ) );
            if ( next != result ) delete result; // для Immutable удаляем старую копию
            result = next;
        }
        return result;
    }

    const T& operator[]( int index ) const {
        return ( *items_ )[index];
    }

    // Оператор записи: seq[i] = value, только для ArraySequence
    T& operator[]( int index ) {
        return ( *items_ )[index]; // делегируем DynamicArray, т.к. там уже имеется T& operator[]
    }

    // IEnumerator
    auto begin()       { return items_->begin(); }
    auto end()         { return items_->end(); }
    auto begin() const { return items_->begin(); }
    auto end() const   { return items_->end(); }

    // Операции через паттерн Instance()
    Sequence<T>* Append( const T& item ) override {
        return Instance()->AppendInternal( item );
    }

    Sequence<T>* Prepend( const T& item ) override {
        return Instance()->PrependInternal( item );
    }

    Sequence<T>* InsertAt( const T& item, int index ) override {
        return Instance()->InsertAtInternal( item, index );
    }

    Sequence<T>* Concat( Sequence<T>* other ) const override {
        if ( other == nullptr )
            throw std::invalid_argument( "ArraySequence: other is nullptr" );

        Sequence<T>* result = this->CreateEmpty();

        for ( int idx = 0; idx < GetLength(); ++idx ) {
            Sequence<T>* next = result->Append( items_->Get( idx ) );
            if ( next != result ) delete result;
            result = next;
        }

        for ( int idx = 0; idx < other->GetLength(); ++idx ) {
            Sequence<T>* next = result->Append( other->Get( idx ) );
            if ( next != result ) delete result;
            result = next;
        }
        return result;
    }
};

#include "MutableArraySequence.h"
