#pragma once

#include <stdexcept>

class Bit {
private:
    bool value_;

public:
    Bit() : value_( false ) { }

    explicit Bit( bool value ) : value_( value ) { }

    // Для возможности писать Bit b = 1 или Bit a = 0
    Bit( int value ) : value_( value != 0 ) {
        if ( value != 0 && value != 1 ) {
            throw std::invalid_argument( "Bit: value must be 0 or 1" );
        }
    }
    //TODO проверить, что нигде нет квадратичных переборов

    bool GetValue() const { return value_; }

    // Побитовые операции между двумя битами
    Bit operator&( const Bit& other ) const { return Bit( value_ && other.value_); }

    Bit operator|( const Bit& other ) const { return Bit( value_ || other.value_); }

    Bit operator^( const Bit& other ) const { return Bit( value_ != other.value_); }

    Bit operator~() const { return Bit( !value_ ); }

    // Сравнение
    bool operator==( const Bit& other ) const { return value_ == other.value_; }
    bool operator!=( const Bit& other ) const { return value_ != other.value_; }

    operator int() const { return value_ ? 1 : 0; }
};
