#pragma once

// #include <iostream> //TODO ядро не должно зависеть от ввода и вывода (сделать отдельный файл, где вынесены все операции)
#include <stdexcept>
#include <string>

class IndexOutOfRange : public std::out_of_range {
public:
    explicit IndexOutOfRange( const std::string& message )
        : std::out_of_range( message ) { }
};

template <class T>
class DynamicArray {
private:
    T*  data_;
    int size_;
    int capacity_;

    void CheckIndex( int index ) const {
        if ( index < 0 || index >= size_ ) {
            throw IndexOutOfRange( "DynamicArray: index out of range" );
        }
    }

public:
    DynamicArray( T* items, int count ) : data_( nullptr ), size_( count ), capacity_( count + 5 ) {
        if ( count < 0 ) {
            throw std::invalid_argument( "DynamicArray: count can't be negative" );
        }
        if ( count > 0 && items == nullptr ) {
            throw std::invalid_argument( "DynamicArray: items is nullptr" );
        }

        data_ = ( capacity_ > 0 ) ? new T[capacity_] : nullptr;
        for ( int idx = 0; idx < size_; ++idx ) {
            data_[idx] = items[idx];
        }
    }

    explicit DynamicArray( int size ) : data_( nullptr ), size_( size ), capacity_( size + 5) {
        if ( size < 0 ) {
            throw std::invalid_argument( "DynamicArray: size can't be negative" );
        }

        data_ = ( capacity_ > 0 ) ? new T[capacity_] : nullptr;
    }

    DynamicArray( const DynamicArray<T>& other ) : data_( nullptr ), size_( other.size_ ), capacity_( other.capacity_ ) {
        if ( capacity_ > 0 ) {
            data_ = new T[capacity_];
            for ( int idx = 0; idx < size_; ++idx ) {
                data_[idx] = other.data_[idx];
            }
        }
    }

    DynamicArray<T>& operator=( const DynamicArray<T>& other ) {
        if ( this != &other ) {
            T* new_data = nullptr;
            if ( other.capacity_ > 0 ) {
                new_data = new T[other.capacity_];
                for ( int idx = 0; idx < other.size_; ++idx ) {
                    new_data[idx] = other.data_[idx];
                }
            }
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.capacity_;
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
        if ( new_size < 0)
            throw std::invalid_argument( "DynamicArray: new_size can't be negative" );

        if ( new_size <= capacity_ ) {
            if ( new_size < size_ ) {
                for ( int i = new_size; i < size_; ++i ) {
                    data_[i].~T();
                }
            }
            size_ = new_size;
            return;
        }

        int new_capacity = capacity_;
        const int threshold = 1000000;
        while ( new_capacity < new_size ) {
            if ( new_capacity < threshold ) {
                new_capacity = ( new_capacity == 0 ) ? 1 : new_capacity * 2;
            } else {
                new_capacity += threshold / 2 ;
            }
        }
        T* new_data = new T[new_capacity];
        for ( int idx = 0; idx < size_; ++idx ) {
            new_data[idx] = data_[idx];
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
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
    //
    // // "IEnumerator" - хранит текущую позицию, умеет двигаться вперёд
    // class IEnumerator {
    // private:
    //     T* ptr_;
    // public:
    //     explicit IEnumerator( T* p ) : ptr_( p ) { }
    //
    //     T& operator*()                                { return *ptr_; }
    //     const T& operator*()                    const { return *ptr_; }
    //     IEnumerator& operator++()                     { ++ptr_; return *this; }
    //     bool operator!=( const IEnumerator& o ) const { return ptr_ != o.ptr_; }
    //     bool operator==( const IEnumerator& o ) const { return ptr_ == o.ptr_; }
    // };
    //
    // // "IEnumerable" - умеет создавать итераторы
    // IEnumerator begin()       { return IEnumerator( data_ ); }
    // IEnumerator end()         { return IEnumerator( data_ + size_ ); }
    // IEnumerator begin() const { return IEnumerator( data_ ); }
    // IEnumerator end()   const { return IEnumerator( data_ + size_ ); }
};
