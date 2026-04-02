#include <gtest/gtest.h>
#include "../src/ArraySequence.h"
#include "../src/MutableArraySequence.h"
#include "../src/ImmutableArraySequence.h"
#include "../src/ListSequence.h"

// |==================================================================|
// |              MutableArraySequence                                |
// |==================================================================|


TEST(MutableArraySequence, DefaultConstructor_EmptySequence) {
    MutableArraySequence<int> seq;

    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(MutableArraySequence, FromRawArray_StoresCorrectValues) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLast(), 4);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(MutableArraySequence, Append_AddToEnd) {
    MutableArraySequence<int> seq;
    seq.Append(10);
    seq.Append(20);
    seq.Append(30);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.GetFirst(), 10);
}

TEST(MutableArraySequence, Append_ReturnsSameObject) {
    MutableArraySequence<int> seq;

    // Mutable должен возвращать СЕБЯ ЖЕ, а не новый объект
    Sequence<int>* returned = seq.Append(1);

    EXPECT_EQ(returned, &seq);
}

TEST(MutableArraySequence, Prepend_AddToBeginning) {
    int data[] = {2, 3};
    MutableArraySequence<int> seq(data, 2);

    seq.Prepend(1);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.Get(1), 2);
}

TEST(MutableArraySequence, InsertAt_Middle) {
    int data[] = {1, 2, 4, 5};
    MutableArraySequence<int> seq(data, 4);

    seq.InsertAt(3, 2);

    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(MutableArraySequence, InsertAt_Begin) {
    int data[] = {2, 3};
    MutableArraySequence<int> seq(data, 2);

    seq.InsertAt(1, 0);

    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, InsertAt_End) {
    int data[] = {1, 2};
    MutableArraySequence<int> seq(data, 2);

    seq.InsertAt(3, 2);

    EXPECT_EQ(seq.GetLast(), 3);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableArraySequence, GetSubsequence_CorrectRange) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->Get(2), 4);

    delete sub;
}

TEST(MutableArraySequence, GetSubsequence_SingleElement) {
    int data[] = {10, 20, 30};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* sub = seq.GetSubsequence(1, 1);

    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 20);

    delete sub;
}

TEST(MutableArraySequence, Concat_ReturnsCombinedSequence) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5, 6};
    MutableArraySequence<int> a(data1, 3);
    MutableArraySequence<int> b(data2, 3);

    Sequence<int>* result = a.Concat(&b);

    EXPECT_EQ(result->GetLength(), 6);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(5), 6);

    delete result;
}

TEST(MutableArraySequence, Concat_DoesNotModifyOriginals) {
    int data1[] = {1, 2};
    int data2[] = {3, 4};
    MutableArraySequence<int> a(data1, 2);
    MutableArraySequence<int> b(data2, 2);

    Sequence<int>* result = a.Concat(&b);

    EXPECT_EQ(a.GetLength(), 2);
    EXPECT_EQ(b.GetLength(), 2);

    delete result;
}

// |==================================================================|
// |              ImmutableArraySequence                              |
// |==================================================================|

TEST(ImmutableArraySequence, Append_DoesNotModifyOriginal) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    // Append возвращает НОВЫЙ объект, исходный не меняется
    Sequence<int>* newSeq = seq.Append(4);

    EXPECT_EQ(seq.GetLength(), 3);       // оригинал не изменился
    EXPECT_EQ(newSeq->GetLength(), 4);   // новый объект с добавленным элементом
    EXPECT_EQ(newSeq->GetLast(), 4);

    delete newSeq;
}

TEST(ImmutableArraySequence, Append_ReturnsDifferentObject) {
    ImmutableArraySequence<int> seq;

    Sequence<int>* returned = seq.Append(1);

    // Immutable должен возвращать НОВЫЙ объект, не себя
    EXPECT_NE(returned, &seq);

    delete returned;
}

TEST(ImmutableArraySequence, Prepend_DoesNotModifyOriginal) {
    int data[] = {2, 3};
    ImmutableArraySequence<int> seq(data, 2);

    Sequence<int>* newSeq = seq.Prepend(1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 2);        // оригинал не изменился
    EXPECT_EQ(newSeq->GetFirst(), 1);
    EXPECT_EQ(newSeq->GetLength(), 3);

    delete newSeq;
}

TEST(ImmutableArraySequence, InsertAt_DoesNotModifyOriginal) {
    int data[] = {1, 3};
    ImmutableArraySequence<int> seq(data, 2);

    Sequence<int>* newSeq = seq.InsertAt(2, 1);

    EXPECT_EQ(seq.GetLength(), 2);      // оригинал не изменился
    EXPECT_EQ(seq.Get(1), 3);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(1), 2);

    delete newSeq;
}

TEST(ImmutableArraySequence, ChainedOperations_EachCallReturnsNewObject) {
    ImmutableArraySequence<int> seq;

    Sequence<int>* s1 = seq.Append(1);
    Sequence<int>* s2 = s1->Append(2);
    Sequence<int>* s3 = s2->Append(3);

    EXPECT_EQ(seq.GetLength(), 0);  // исходный пустой
    EXPECT_EQ(s1->GetLength(), 1);
    EXPECT_EQ(s2->GetLength(), 2);
    EXPECT_EQ(s3->GetLength(), 3);
    EXPECT_EQ(s3->GetLast(), 3);

    delete s1;
    delete s2;
    delete s3;
}

// |==================================================================|
// |              MutableListSequence                                 |
// |==================================================================|

TEST(MutableListSequence, DefaultConstructor_EmptySequence) {
    MutableListSequence<int> seq;

    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(MutableListSequence, FromRawArray_StoresCorrectValues) {
    int data[] = {10, 20, 30};
    MutableListSequence<int> seq(data, 3);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetFirst(), 10);
    EXPECT_EQ(seq.GetLast(), 30);
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(MutableListSequence, Append_AddToEnd) {
    MutableListSequence<int> seq;
    seq.Append(1);
    seq.Append(2);
    seq.Append(3);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.GetLast(), 3);
}

TEST(MutableListSequence, Append_ReturnsSameObject) {
    MutableListSequence<int> seq;

    Sequence<int>* returned = seq.Append(1);

    EXPECT_EQ(returned, &seq);
}

TEST(MutableListSequence, Prepend_AddToBeginning) {
    int data[] = {2, 3};
    MutableListSequence<int> seq(data, 2);

    seq.Prepend(1);

    EXPECT_EQ(seq.GetFirst(), 1);
    EXPECT_EQ(seq.GetLength(), 3);
}

TEST(MutableListSequence, InsertAt_Middle) {
    int data[] = {1, 2, 4};
    MutableListSequence<int> seq(data, 3);

    seq.InsertAt(3, 2);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(2), 3);
    EXPECT_EQ(seq.Get(3), 4);
}

TEST(MutableListSequence, GetSubsequence_CorrectRange) {
    int data[] = {1, 2, 3, 4, 5};
    MutableListSequence<int> seq(data, 5);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(2), 4);

    delete sub;
}

TEST(MutableListSequence, Concat_ReturnsCombinedSequence) {
    int data1[] = {1, 2};
    int data2[] = {3, 4};
    MutableListSequence<int> a(data1, 2);
    MutableListSequence<int> b(data2, 2);

    Sequence<int>* result = a.Concat(&b);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);

    delete result;
}

// |==================================================================|
// |              ImmutableListSequence                               |
// |==================================================================|

TEST(ImmutableListSequence, Append_DoesNotModifyOriginal) {
    int data[] = {1, 2, 3};
    ImmutableListSequence<int> seq(data, 3);

    Sequence<int>* newSeq = seq.Append(4);

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(newSeq->GetLength(), 4);
    EXPECT_EQ(newSeq->GetLast(), 4);

    delete newSeq;
}

TEST(ImmutableListSequence, Append_ReturnsDifferentObject) {
    ImmutableListSequence<int> seq;

    Sequence<int>* returned = seq.Append(1);

    EXPECT_NE(returned, &seq);

    delete returned;
}

TEST(ImmutableListSequence, Prepend_DoesNotModifyOriginal) {
    int data[] = {2, 3};
    ImmutableListSequence<int> seq(data, 2);

    Sequence<int>* newSeq = seq.Prepend(1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.GetFirst(), 2);
    EXPECT_EQ(newSeq->GetFirst(), 1);
    EXPECT_EQ(newSeq->GetLength(), 3);

    delete newSeq;
}

TEST(ImmutableListSequence, InsertAt_DoesNotModifyOriginal) {
    int data[] = {1, 3};
    ImmutableListSequence<int> seq(data, 2);

    Sequence<int>* newSeq = seq.InsertAt(2, 1);

    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(1), 3);
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(1), 2);

    delete newSeq;
}

TEST(ImmutableListSequence, ChainedOperations_EachCallReturnsNewObject) {
    ImmutableListSequence<int> seq;

    Sequence<int>* s1 = seq.Append(1);
    Sequence<int>* s2 = s1->Append(2);
    Sequence<int>* s3 = s2->Append(3);

    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(s1->GetLength(), 1);
    EXPECT_EQ(s2->GetLength(), 2);
    EXPECT_EQ(s3->GetLength(), 3);

    delete s1;
    delete s2;
    delete s3;
}

// Полиморфизм через Sequence<T>*

TEST(SequencePolymorphism, ArrayAndListBehaveTheSameThroughBasePointer) {
    int data[] = {1, 2, 3};

    Sequence<int>* arr  = new MutableArraySequence<int>(data, 3);
    Sequence<int>* list = new MutableListSequence<int>(data, 3);

    // Оба работают одинаково через базовый указатель
    EXPECT_EQ(arr->GetLength(),  list->GetLength());
    EXPECT_EQ(arr->GetFirst(),   list->GetFirst());
    EXPECT_EQ(arr->GetLast(),    list->GetLast());
    EXPECT_EQ(arr->Get(1),       list->Get(1));

    delete arr;
    delete list;
}

TEST(SequencePolymorphism, ConcatArrayWithList) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};

    Sequence<int>* arr  = new MutableArraySequence<int>(data1, 3);
    Sequence<int>* list = new MutableListSequence<int>(data2, 2);

    // Concat принимает Sequence<T>* — тип реализации не важен
    Sequence<int>* result = arr->Concat(list);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);

    delete arr;
    delete list;
    delete result;
}

// |==================================================================|
// |              Исключения                                          |
// |==================================================================|

TEST(SequenceExceptions, GetFirst_EmptySequence_ThrowsIndexOutOfRange) {
    MutableArraySequence<int> seq;

    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
}

TEST(SequenceExceptions, GetLast_EmptySequence_ThrowsIndexOutOfRange) {
    MutableListSequence<int> seq;

    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(SequenceExceptions, Get_NegativeIndex_ThrowsIndexOutOfRange) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
}

TEST(SequenceExceptions, GetSubsequence_StartGreaterThanEnd_ThrowsInvalidArgument) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq.GetSubsequence(2, 1), std::invalid_argument);
}

TEST(SequenceExceptions, GetSubsequence_OutOfRange_ThrowsIndexOutOfRange) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq.GetSubsequence(0, 10), IndexOutOfRange);
}

TEST(SequenceExceptions, Concat_Nullptr_ThrowsInvalidArgument) {
    MutableArraySequence<int> seq;

    EXPECT_THROW(seq.Concat(nullptr), std::invalid_argument);
}

TEST(SequenceExceptions, InsertAt_NegativeIndex_ThrowsIndexOutOfRange) {
    MutableArraySequence<int> seq;

    EXPECT_THROW(seq.InsertAt(1, -1), IndexOutOfRange);
}

// operator[]

TEST(SequenceOperatorTests, ReadByIndex) {
    int data[] = {10, 20, 30};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_EQ(seq[0], 10);
    EXPECT_EQ(seq[1], 20);
    EXPECT_EQ(seq[2], 30);
}

TEST(SequenceOperatorTests, WriteByIndex) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    seq[1] = 99;

    EXPECT_EQ(seq[1], 99);
}

TEST(SequenceOperatorTests, ReadOnListSequence) {
    int data[] = {5, 6, 7};
    MutableListSequence<int> seq(data, 3);

    EXPECT_EQ(seq[0], 5);
    EXPECT_EQ(seq[2], 7);
}

TEST(SequenceOperatorTests, OutOfRangeThrows) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_THROW(seq[5], IndexOutOfRange);
    EXPECT_THROW(seq[-1], IndexOutOfRange);
}

// ==================== MapIndexed (М-2.2) ====================

TEST(MapIndexedTests, AddsIndexToElement) {
    int data[] = {10, 20, 30};
    MutableArraySequence<int> seq(data, 3);

    // f(val, idx) = val + idx → {10+0, 20+1, 30+2} = {10, 21, 32}
    Sequence<int>* result = seq.MapIndexed( [](int val, int idx) {
        return val + idx;
    });

    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 21);
    EXPECT_EQ(result->Get(2), 32);

    delete result;
}

// ==================== ZipWithReversed (П-8) ====================

TEST(ZipWithReversedTests, SumWithMirror) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    Sequence<int>* result = seq.ZipWithReversed();

    // [1+4, 2+3, 3+2, 4+1] = [5, 5, 5, 5]
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(1), 5);
    EXPECT_EQ(result->Get(2), 5);
    EXPECT_EQ(result->Get(3), 5);

    delete result;
}

TEST(ZipWithReversedTests, OddLength) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* result = seq.ZipWithReversed();

    // [1+3, 2+2, 3+1] = [4, 4, 4]
    EXPECT_EQ(result->Get(0), 4);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 4);

    delete result;
}

TEST(ZipWithReversedTests, SingleElement) {
    int data[] = {42};
    MutableArraySequence<int> seq(data, 1);

    Sequence<int>* result = seq.ZipWithReversed();

    EXPECT_EQ(result->Get(0), 84); // 42 + 42

    delete result;
}

TEST( ZipWithReversedTest, WorksForListSequence ) {
    int data[] = { 1, 2, 3 };
    MutableListSequence<int> seq( data, 3 );

    Sequence<int>* result = seq.ZipWithReversed();

    // [1+3, 2+2, 3+1] = [4, 4, 4]
    EXPECT_EQ( result->GetLength(), 3 );
    EXPECT_EQ( result->Get( 0 ), 4 );
    EXPECT_EQ( result->Get( 1 ), 4 );
    EXPECT_EQ( result->Get( 2 ), 4 );

    delete result;
}

// П-1: MinMaxAvg

TEST(MinMaxAvgTests, BasicTest) {
    int data[] = {3, 1, 5};
    MutableArraySequence<int> seq(data, 3);

    EXPECT_EQ(seq.GetMin(), 1);
    EXPECT_EQ(seq.GetMax(), 5);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 3.0);  // 9/3 = 3.0
}

TEST(MinMaxAvgTests, AllSameElements) {
    int data[] = {7, 7, 7, 7};
    MutableArraySequence<int> seq(data, 4);

    EXPECT_EQ(seq.GetMin(), 7);
    EXPECT_EQ(seq.GetMax(), 7);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 7.0);
}

TEST(MinMaxAvgTests, SingleElement) {
    int data[] = {42};
    MutableArraySequence<int> seq(data, 1);

    EXPECT_EQ(seq.GetMin(), 42);
    EXPECT_EQ(seq.GetMax(), 42);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 42.0);
}

TEST(MinMaxAvgTests, NegativeNumbers) {
    int data[] = {-5, -3, -10, -1};
    MutableArraySequence<int> seq(data, 4);

    EXPECT_EQ(seq.GetMin(), -10);
    EXPECT_EQ(seq.GetMax(), -1);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), -19.0 / 4.0);
}

TEST(MinMaxAvgTests, MixedNegativePositive) {
    int data[] = {-3, 0, 5, -1, 2};
    MutableArraySequence<int> seq(data, 5);

    EXPECT_EQ(seq.GetMin(), -3);
    EXPECT_EQ(seq.GetMax(), 5);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 3.0 / 5.0);
}

TEST(MinMaxAvgTests, WorksOnListSequence) {
    int data[] = {2, 8, 4};
    MutableListSequence<int> seq(data, 3);

    EXPECT_EQ(seq.GetMin(), 2);
    EXPECT_EQ(seq.GetMax(), 8);
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 14.0 / 3.0);
}

TEST( MinMaxAvgTests, EmptyThrowsOnMin ) {
    MutableArraySequence<int> seq;
    EXPECT_THROW( seq.GetMin(), IndexOutOfRange );
}

TEST( MinMaxAvgTests, EmptyThrowsOnMax ) {
    MutableArraySequence<int> seq;\
    EXPECT_THROW( seq.GetMax(), IndexOutOfRange );
}

TEST( MinMaxAvgTests, EmptyThrowsOnAvg ) {
    MutableArraySequence<int> seq;
    EXPECT_THROW( seq.GetAvg(), IndexOutOfRange );
}

TEST(MinMaxAvgTests, AvgIsNotRounded) {
    int data[] = {1, 2};
    MutableArraySequence<int> seq(data, 2);

    // avg = 3/2 = 1.5, не 1 (целочисленное деление)
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 1.5);
}

// IEnumerator
TEST(SequenceIterator, ArraySequence_SupportsRangeBasedFor) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    std::vector<int> result;
    for (int val : seq) {  // через прокси begin()/end()
        result.push_back(val);
    }

    EXPECT_EQ(result, std::vector<int>({1, 2, 3}));
}

TEST(SequenceIterator, ListSequence_SupportsRangeBasedFor) {
    int data[] = {5, 6, 7};
    MutableListSequence<int> seq(data, 3);

    std::vector<int> result;
    for (int val : seq) {
        result.push_back(val);
    }

    EXPECT_EQ(result, std::vector<int>({5, 6, 7}));
}

TEST(SequenceIterator, EmptySequence_IterationSafe) {
    MutableArraySequence<int> seq;

    int count = 0;
    for (int val : seq) {
        ++count;
    }
    EXPECT_EQ(count, 0);
}

// Дополнительно: проверка модификации через итератор (для mutable)
TEST(SequenceIterator, CanModifyElementsViaIterator) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    for (int& val : seq) {  // non-const reference
        val *= 2;
    }

    EXPECT_EQ(seq.Get(0), 2);
    EXPECT_EQ(seq.Get(1), 4);
    EXPECT_EQ(seq.Get(2), 6);
}

// ==================== Immutable Concat Tests ====================

TEST(ImmutableArraySequence, Concat_DoesNotModifyOriginal) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    ImmutableArraySequence<int> seq1(data1, 3);
    ImmutableArraySequence<int> seq2(data2, 2);

    Sequence<int>* result = seq1.Concat(&seq2);

    // Оригиналы не изменились
    EXPECT_EQ(seq1.GetLength(), 3);
    EXPECT_EQ(seq1.Get(0), 1);
    EXPECT_EQ(seq1.Get(2), 3);
    EXPECT_EQ(seq2.GetLength(), 2);
    EXPECT_EQ(seq2.Get(0), 4);
    EXPECT_EQ(seq2.Get(1), 5);

    // Результат корректен
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);

    delete result;
}

TEST(ImmutableArraySequence, Concat_WithEmpty_ReturnsCopyOfNonEmpty) {
    int data[] = {10, 20};
    ImmutableArraySequence<int> seq1(data, 2);
    ImmutableArraySequence<int> seq2; // empty

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 20);
    // Результат — новый объект, не совпадает с seq1
    EXPECT_NE(result, &seq1);

    delete result;
}

TEST(ImmutableArraySequence, Concat_EmptyWithNonEmpty_ReturnsCopy) {
    int data[] = {5, 6, 7};
    ImmutableArraySequence<int> seq1; // empty
    ImmutableArraySequence<int> seq2(data, 3);

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 5);
    EXPECT_EQ(result->Get(2), 7);
    EXPECT_NE(result, &seq2);

    delete result;
}

TEST(ImmutableArraySequence, Concat_BothEmpty_ReturnsEmpty) {
    ImmutableArraySequence<int> seq1;
    ImmutableArraySequence<int> seq2;

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(result->GetLength(), 0);
    // Должен быть новый пустой объект
    EXPECT_NE(result, &seq1);
    EXPECT_NE(result, &seq2);

    delete result;
}

TEST(ImmutableArraySequence, Concat_ResultIsIndependentOfOriginals) {
    int data1[] = {1, 2};
    int data2[] = {3, 4};
    ImmutableArraySequence<int> seq1(data1, 2);
    ImmutableArraySequence<int> seq2(data2, 2);

    Sequence<int>* result = seq1.Concat(&seq2);

    // Модификация оригиналов (через создание новых объектов) не влияет на результат
    Sequence<int>* newSeq1 = seq1.Append(99);
    Sequence<int>* newSeq2 = seq2.Prepend(0);

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);

    delete result;
    delete newSeq1;
    delete newSeq2;
}

// ==================== ImmutableListSequence Concat Tests ====================

TEST(ImmutableListSequence, Concat_DoesNotModifyOriginal) {
    int data1[] = {1, 2, 3};
    int data2[] = {4, 5};
    ImmutableListSequence<int> seq1(data1, 3);
    ImmutableListSequence<int> seq2(data2, 2);

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(seq1.GetLength(), 3);
    EXPECT_EQ(seq1.Get(0), 1);
    EXPECT_EQ(seq2.GetLength(), 2);
    EXPECT_EQ(seq2.Get(0), 4);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(4), 5);

    delete result;
}

TEST(ImmutableListSequence, Concat_WithEmpty_ReturnsCopy) {
    int data[] = {42};
    ImmutableListSequence<int> seq1(data, 1);
    ImmutableListSequence<int> seq2;

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(result->GetLength(), 1);
    EXPECT_EQ(result->Get(0), 42);
    EXPECT_NE(result, &seq1);

    delete result;
}

TEST(ImmutableListSequence, Concat_BothEmpty_ReturnsEmpty) {
    ImmutableListSequence<int> seq1;
    ImmutableListSequence<int> seq2;

    Sequence<int>* result = seq1.Concat(&seq2);

    EXPECT_EQ(result->GetLength(), 0);
    delete result;
}

// ==================== Cross-type Concat (polymorphic) ====================

TEST(ImmutableSequencePolymorphism, Concat_ArrayWithList) {
    int data1[] = {1, 2};
    int data2[] = {3, 4, 5};
    ImmutableArraySequence<int> arrSeq(data1, 2);
    ImmutableListSequence<int> listSeq(data2, 3);

    // Используем базовые указатели
    Sequence<int>* seq1 = &arrSeq;
    Sequence<int>* seq2 = &listSeq;

    Sequence<int>* result = seq1->Concat(seq2);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(2), 3);
    EXPECT_EQ(result->Get(4), 5);

    // Оригиналы не изменились
    EXPECT_EQ(arrSeq.GetLength(), 2);
    EXPECT_EQ(listSeq.GetLength(), 3);

    delete result;
}

TEST(ImmutableSequencePolymorphism, Concat_ListWithArray) {
    int data1[] = {10, 20};
    int data2[] = {30};
    ImmutableListSequence<int> listSeq(data1, 2);
    ImmutableArraySequence<int> arrSeq(data2, 1);

    Sequence<int>* result = listSeq.Concat(&arrSeq);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 10);
    EXPECT_EQ(result->Get(1), 20);
    EXPECT_EQ(result->Get(2), 30);

    delete result;
}
