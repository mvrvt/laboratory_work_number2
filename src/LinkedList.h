#pragma once

#include <stdexcept>
#include <string>
#include "DynamicArray.h"

template <class T>
class LinkedList {
private:
    struct Node {
        T     data;
        Node* next;

        explicit Node( const T& value ) : data( value ), next( nullptr ) { }
    };

    Node* head_;
    Node* tail_;
    int   size_;

    // Вспомогательный метод - получить указатель на узел по индексу
    Node* GetNode( int index ) const {
        if ( index < 0 || index >= size_ ) {
            throw IndexOutOfRange( "LinkedList: index out of range" );
        }

        Node* current = head_;
        for ( int idx = 0; idx < index; ++idx ) {
            current = current->next;
        }
        return current;
    }

    // Вспомогательный метод - освободить всю память списка
    void Clear() {
        Node* current = head_;
        while ( current != nullptr ) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

public:
    // Конструкторы
    LinkedList() : head_( nullptr ), tail_( nullptr ), size_( 0 ) { }

    LinkedList( T* items, int count ) : head_( nullptr ), tail_( nullptr ), size_( 0 ) {
        if ( count < 0 ) {
            throw std::invalid_argument( "LinkedList: count can't be negative" );
        }
        if ( count > 0 && items == nullptr ) {
            throw std::invalid_argument( "LinkedList: items is nullptr" );
        }

        for ( int idx = 0; idx < count; ++idx ) {
            Append( items[idx] );
        }
    }

    LinkedList( const LinkedList<T>& other ) : head_( nullptr ), tail_( nullptr ), size_( 0 ) {
        Node* current = other.head_;
        while ( current != nullptr ) {
            Append( current->data );
            current = current->next;
        }
    }

    LinkedList<T>& operator=( const LinkedList<T>& other ) {
        if ( this != &other ) {
            Clear();

            Node* current = other.head_;
            while ( current != nullptr ) {
                Append( current->data );
                current = current->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    // Декомпозиция

    T GetFirst() const {
        if ( size_ == 0 ) {
            throw IndexOutOfRange( "LinkedList: list is empty" );
        }
        return head_->data;
    }

    T GetLast() const {
        if ( size_ == 0 ) {
            throw IndexOutOfRange( "LinkedList: list is empty" );
        }
        return tail_->data;
    }

    T Get( int index ) const {
        return GetNode( index )->data;
    }

    int GetLength() const {
        return size_;
    }

    LinkedList<T>* GetSubList( int start_index, int end_index ) const {
        if ( start_index < 0 || start_index >= size_ ) {
            throw IndexOutOfRange( "LinkedList: start_index out of range" );
        }
        if ( end_index < 0 || end_index >= size_ ) {
            throw IndexOutOfRange( "LinkedList: end_index out of range" );
        }
        if ( start_index > end_index ) {
            throw std::invalid_argument( "LinkedList: start_index > end_index" );
        }

        LinkedList<T>* result = new LinkedList<T>();
        Node* current = GetNode( start_index );

        for ( int idx = start_index; idx <= end_index; ++idx ) {
            result->Append( current->data );
            current = current->next;
        }
        return result;
    }

    // Операции

    void Append( const T& item ) {
        Node* new_node = new Node( item );

        if ( head_ == nullptr ) {
            head_ = new_node;
            tail_ = new_node;
        } else {
            tail_->next = new_node;
            tail_ = new_node;
        }
        ++size_;
    }

    void Prepend( const T& item ) {
        Node* new_node = new Node( item );
        if ( head_ == nullptr ) {
            head_ = new_node;
            tail_ = new_node;
        } else {
            new_node->next = head_;
            head_ = new_node;
            // tail_ не меняется, хвост остался тем же
        }
        ++size_;
    }

    void InsertAt( const T& item, int index ) {
        if ( index < 0 || index > size_ ) {
            throw IndexOutOfRange( "LinkedList: index out of range");
        }

        if ( index == 0 ) {
            Prepend( item );
            return;
        }

        if ( index == size_ ) {
            Append( item );
            return;
        }

        // Вставка в середину
        Node* prev     = GetNode( index - 1 );
        Node* new_node = new Node( item );
        new_node->next = prev->next;
        prev->next     = new_node;
        // tail_ не меняется - вставка была не в конец
        ++size_;
    }

    LinkedList<T>* Concat( LinkedList<T>* list ) const {
        if ( list == nullptr ) {
            throw std::invalid_argument( "LinkedList: list is nullptr" );
        }

        LinkedList<T>* result = new LinkedList( *this );

        Node* current = list->head_;
        while ( current != nullptr ) {
            result->Append( current->data );
            current = current->next;
        }
        return result;
    }

    // IEnumerator
    class IEnumerator {
    private:
        Node* current_;
    public:
        explicit IEnumerator( Node* n ) : current_( n ) { }

        T&           operator*()                              { return current_->data; }
        const T&     operator*()                        const { return current_->data; }
        IEnumerator& operator++()                             { current_ = current_->next; return *this; }
        bool         operator!=( const IEnumerator& o ) const { return current_ != o.current_; }
        bool         operator==( const IEnumerator& o ) const { return current_ == o.current_; }
    };

    IEnumerator begin() { return IEnumerator( head_ ); }
    IEnumerator end() { return IEnumerator( nullptr ); } // конец - nullptr (след. элемента нет)
};
