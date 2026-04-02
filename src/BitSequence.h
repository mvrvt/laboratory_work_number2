#pragma once

#include <string>
#include "Bit.h"
#include "MutableArraySequence.h"

class BitSequence : public MutableArraySequence<Bit> {
private:
    // Проверка, что обе пос-ти одной длины
    void CheckSameLength( const BitSequence& other ) const {
        if ( GetLength() != other.GetLength() ) {
            throw std::invalid_argument(
                "BitSequence: sequence must have the same length for bitwise operations"
            );
        }
    }

public:
    // Конструкторы

    BitSequence() : MutableArraySequence<Bit>() {}

    // Создать из массива int'ов BitSequence( {1, 0, 1, 0}, 4 )
    BitSequence( int* values, int count ) : MutableArraySequence<Bit>() {
        if ( count < 0 ) {
            throw std::invalid_argument( "BitSequence: count can't be negative" );
        }
        if ( count > 0 && values == nullptr ) {
            throw std::invalid_argument( "BitSequence: values is nullptr" );
        }
        for ( int idx = 0; idx < count; ++idx ) {
            Append( Bit( values[idx] ) );
        }
    }

    // Создать из массива Bit'ов
    BitSequence( Bit* bits, int count ) : MutableArraySequence<Bit>( bits, count ) {}

    // Копирующий конструктор
    BitSequence( const BitSequence& other ) : MutableArraySequence<Bit>( other ) {}

    // Создать из строки: BitSequence("10101")
    BitSequence( const std::string& bits ) : MutableArraySequence<Bit>() {
        for ( char c : bits ) {
            if ( c != '0' && c != '1' ) {
                throw std::invalid_argument(
                    "BitSequence: string must contain only '0' or '1' symbols"
                );
            }
            Append( Bit( c == '1' ) );
        }
    }

    // Побитовые операции (возвращают по значению)

    BitSequence BitwiseAND( const BitSequence& other ) const {
        CheckSameLength( other );
        BitSequence result;
        for ( int idx = 0; idx < GetLength(); ++idx ) {
            result.Append( Get( idx ) & other.Get( idx ) );
        }
        return result;   // по значению, не new
    }

    BitSequence BitwiseOR( const BitSequence& other ) const {
        CheckSameLength( other );
        BitSequence result;
        for ( int idx = 0; idx < GetLength(); ++idx ) {
            result.Append( Get( idx ) | other.Get( idx ) );
        }
        return result;
    }

    BitSequence BitwiseXOR( const BitSequence& other ) const {
        CheckSameLength( other );
        BitSequence result;
        for ( int idx = 0; idx < GetLength(); ++idx ) {
            result.Append( Get( idx ) ^ other.Get( idx ) );
        }
        return result;
    }

    BitSequence BitwiseNOT() const {
        BitSequence result;
        for ( int idx = 0; idx < GetLength(); ++idx ) {
            result.Append( ~Get( idx ) );
        }
        return result;
    }

    // Операторы (возвращают новый объект BitSequence)
    BitSequence operator&( const BitSequence& other ) const { return BitwiseAND( other ); }
    BitSequence operator|( const BitSequence& other ) const { return BitwiseOR( other );  }
    BitSequence operator^( const BitSequence& other ) const { return BitwiseXOR( other ); }
    BitSequence operator~()                           const { return BitwiseNOT();        }
};
