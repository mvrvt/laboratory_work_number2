#pragma once

#include <utility>
#include "MutableArraySequence.h"

// П-5: список всех префиксов через Reduce
template <typename T>
Sequence<Sequence<T>*>* GetPrefixes( Sequence<T>* seq ) {
    if ( seq->GetLength() == 0 ) {
        return new MutableArraySequence<Sequence<T>*>();
    }

    // Аккумулятор: пара {результирующая список, текущий индекс}
    using Acc = std::pair<MutableArraySequence<Sequence<T>*>*, int>;

    Acc initial = { new MutableArraySequence<Sequence<T>*>(), 0 };

    Acc result = seq->template Reduce<Acc>(
        [seq]( Acc acc, const T& /*item*/ ) -> Acc {
            // Добавляем префикс [0, ..., current_index]
            acc.first->Append( seq->GetSubsequence( 0, acc.second ) );
            return { acc.first, acc.second+1 };
        },
        initial
    );
    return result.first;
}

// П-5: список всех постфиксов через Reduce
template <typename T>
Sequence<Sequence<T>*>* GetPostfixes( Sequence<T>* seq ) {
    if ( seq->GetLength() == 0 ) {
        return new MutableArraySequence<Sequence<T>*>();
    }

    using Acc = std::pair<MutableArraySequence<Sequence<T>*>*, int>;

    Acc initial = { new MutableArraySequence<Sequence<T>*>(), 0 };

    Acc result = seq->template Reduce<Acc>(
        [seq]( Acc acc, const T& /*item*/ ) -> Acc {
            // Добавляем постфикс [ current_index, ..., n-1 ]
            acc.first->Append( seq->GetSubsequence( acc.second, seq->GetLength()-1 ) );
            return { acc.first, acc.second+1 };
        },
        initial
    );
    return result.first;
}

// M-2.1: Unzip — разбивает список пар на два отдельных списка
template <typename T1, typename T2>
std::pair<Sequence<T1>*, Sequence<T2>*> Unzip( const Sequence<std::pair<T1, T2>>* seq ) {
    if ( seq == nullptr )
        throw std::invalid_argument( "Unzip: seq is nullptr" );
    auto* first  = new MutableArraySequence<T1>();
    auto* second = new MutableArraySequence<T2>();
    for ( int idx = 0; idx < seq->GetLength(); ++idx ) {
        first->Append(  seq->Get( idx ).first );
        second->Append( seq->Get( idx ).second );
    }
    return { first, second };
}

// М-2.1: Split — разбивает последовательность на фрагменты по разделителю.
// Элементы, удовлетворяющие pred, являются границами и не включаются в результат.
// Пустые сегменты в конце не добавляются (если последовательность заканчивается разделителем).
// Пустые сегменты в середине (два разделителя подряд) сохраняются.
// Пример: Split([1, 0, 2, 0, 3], x==0) == [[1], [2], [3]]
//         Split([1, 0],           x==0) == [[1]]
template <typename T>
Sequence<Sequence<T>*>* Split( Sequence<T>* seq, std::function<bool( const T& )> pred ) {
    if ( seq == nullptr )
        throw std::invalid_argument( "Split: seq is empty(nullptr)" );

    auto result  = new MutableArraySequence<Sequence<T>*>();
    auto current = new MutableArraySequence<T>();

    for ( int idx = 0; idx < seq->GetLength(); ++idx ) {
        if ( pred( seq->Get( idx ) ) ) {
            result->Append( current );
            current = new MutableArraySequence<T>();
        } else {
            current->Append( seq->Get( idx ) );
        }
    }

    // Добавляем последний сегмент только если он не пустой,
    // чтобы не создавать лишний [] в конце при хвостовом разделителе
    if ( current->GetLength() > 0 ) {
        result->Append( current );
    } else {
        delete current; // пустой сегмент — удаляем, чтобы не было утечки памяти
    }

    return result;
}

// M-2.1: Slice — удалить n элементов начиная с позиции i,
// вставить вместо них элементы из s (если s != nullptr)
// Отрицательный i — отсчёт с конца
template <typename T>
Sequence<T>* Slice( Sequence<T>* seq, int i, int n, Sequence<T>* s = nullptr ) {
    if ( seq == nullptr )
        throw std::invalid_argument( "Slice: seq is nullptr" );
    int len = seq->GetLength();
    if ( i < 0 ) i = len + i; // отрицательный индекс - с конца
    if ( i < 0 || i > len )
        throw IndexOutOfRange( "Slice: index out of range" );
    if ( n < 0 )
        throw std::invalid_argument( "Slice: n < 0" );
    if ( n > len - i )
        throw IndexOutOfRange( "Slice: range [" + std::to_string( i )
            + ", " + std::to_string( i + n ) + ") exceeds sequence length "
            + std::to_string( len ) );

    auto* result = new MutableArraySequence<T>();
    for ( int k = 0; k < i; ++k )
        result->Append( seq->Get( k ) );
    if ( s != nullptr ) {
        for ( int k = 0; k < s->GetLength(); ++k )
            result->Append( s->Get( k ) );
    }
    for ( int k = i + n; k < len; ++k )
        result->Append( seq->Get( k ) );
    return result;
}

// M-2.1: Range — генератор последовательности [start, end) с шагом step
template <typename T>
Sequence<T>* Range( T start, T end, T step = T(1) ) {
    if ( step == T(0) )
        throw std::invalid_argument( "Range: step cannot be 0" );
    auto* result = new MutableArraySequence<T>();
    if ( step > T(0) ) {
        for ( T val = start; val < end; val += step )
            result->Append( val );
    } else {
        for ( T val = start; val > end; val += step )
            result->Append( val );
    }
    return result;
}

// П-6: для каждого a_i: (a_i-1 + a_i + a_i+1) / 3.0
// Граничные элементы: (a[0]+a[1])/2 и (a[n-2]+a[n-1])/2
// Реализовано через MapIndexedTo
template <typename T>
Sequence<double>* SlidingAverage( const Sequence<T>* seq ) {
    if ( seq == nullptr )
        throw std::invalid_argument( "SlidingAverage: seq is empty(nullptr)" );

    int n = seq->GetLength();
    if ( n < 2 )
        throw std::invalid_argument( "SlidingAverage: needs at least 2 elements" );

    return seq->template MapIndexedTo<double>(
        [seq, n]( const T& val, int idx ) -> double {
            if ( idx == 0 )
                return ( static_cast<double>( val ) + seq->Get( 1 ) ) / 2.0;
            if ( idx == n - 1 )
                return ( static_cast<double>( seq->Get( n - 2 ) ) + val ) / 2.0;
            return ( static_cast<double>( seq->Get( idx - 1 ) ) + val + seq->Get( idx + 1 ) ) / 3.0;
        }
    );
}
