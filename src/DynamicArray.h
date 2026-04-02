#pragma once

#include <iostream>
#include <stdexcept>
#include <string>

class IndexOutOfRange : public std::out_of_range {
public:
    explicit IndexOutOfRange( const std::string& message )
        : std::out_of_range( message ) {}
};

template <class T>
class DynamicArray {
private:
    T*  data_;
    int size_;

    void CheckIndex( int index ) const {
        if ( index < 0 || index >= size_ ) {
            throw IndexOutOfRange( "DynamicArray: index out of range" );
        }
    }

public:
    DynamicArray( T* items, int count ) : data_( nullptr ), size_( count ) {
        if ( count < 0 ) {
            throw std::invalid_argument( "DynamicArray: count can't be negative" );
        }
        if ( count > 0 && items == nullptr ) {
            throw std::invalid_argument( "DynamicArray: items is nullptr" );
        }

        if ( size_ > 0 ) {
            data_ = new T[size_];
            for ( int idx = 0; idx < size_; ++idx ) {
                data_[idx] = items[idx];
            }
        }
    }

    explicit DynamicArray( int size ) : data_( nullptr ), size_( size ) {
        if ( size < 0 ) {
            throw std::invalid_argument( "DynamicArray: size can't be negative" );
        }

        if ( size_ > 0 ) {
            data_ = new T[size_];
        }
    }

    DynamicArray( const DynamicArray<T>& other ) : data_( nullptr ), size_( other.size_ ) {
        if ( size_ > 0 ) {
            data_ = new T[size_];
            for ( int idx = 0; idx < size_; ++idx ) {
                data_[idx] = other.data_[idx];
            }
        }
    }

    DynamicArray<T>& operator=( const DynamicArray <T>& other ) {
        if ( this != &other ) {
            T* new_data = nullptr;

            if ( other.size_  > 0 ) {
                new_data = new T[other.size_];
                for ( int idx = 0; idx < other.size_; ++idx ) {
                    new_data[idx] = other.data_[idx];
                }
            }
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data_;
    }

    T Get( int index ) const {
        CheckIndex( index );
        return data_[index];
    }

    int GetSize() const {
        return size_;
    }

    void Set( int index, const T& value ) {
        CheckIndex( index );
        data_[index] = value;
    }

    void Resize( int new_size ) {
        if ( new_size < 0 ) {
            throw std::invalid_argument( "DynamicArray: new_size can't be negative" );
        }

        T* new_data = nullptr;

        if ( new_size > 0 ) {
            new_data = new T[new_size];
            int copy_count = ( new_size < size_ ) ? new_size : size_;

            for ( int idx = 0; idx < copy_count; ++idx ) {
                new_data[idx] = data_[idx];
            }
        }

        delete[] data_;
        data_ = new_data;
        size_ = new_size;
    }

    T& operator[]( int index ) {
        CheckIndex( index );
        return data_[index];
    }

    const T& operator[]( int index ) const {
        CheckIndex( index );
        return data_[index];
    }

    // "IEnumerator" - хранит текущую позицию, умеет двигаться вперёд
    class IEnumerator {
    private:
        T* ptr_;
    public:
        explicit IEnumerator( T* p ) : ptr_( p ) { }

        T& operator*()                                { return *ptr_; }
        const T& operator*()                    const { return *ptr_; }
        IEnumerator& operator++()                     { ++ptr_; return *this; }
        bool operator!=( const IEnumerator& o ) const { return ptr_ != o.ptr_; }
        bool operator==( const IEnumerator& o ) const { return ptr_ == o.ptr_; }
    };

    // "IEnumerable" - умеет создавать итераторы
    IEnumerator begin()       { return IEnumerator( data_ ); }
    IEnumerator end()         { return IEnumerator( data_ + size_ ); }
    IEnumerator begin() const { return IEnumerator( data_ ); }
    IEnumerator end()   const { return IEnumerator( data_ + size_ ); }
};
