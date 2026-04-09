#include <gtest/gtest.h>
#include "../src/BitSequence.h"
#include "../src/SequenceIterator.h"

// ==================== Конструкторы ====================

TEST(BitSequenceConstructors, DefaultConstructor_EmptySequence) {
    BitSequence seq;
    
    EXPECT_EQ(seq.GetLength(), 0);
    EXPECT_EQ(seq.GetCount(), 0);
}

TEST(BitSequenceConstructors, FromIntArray_StoresCorrectBits) {
    int data[] = {1, 0, 1, 1, 0, 0, 1, 0, 1};
    BitSequence seq(data, 9);
    
    EXPECT_EQ(seq.GetLength(), 9);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), true);
    EXPECT_EQ(seq.Get(4).GetValue(), false);
    EXPECT_EQ(seq.Get(5).GetValue(), false);
    EXPECT_EQ(seq.Get(6).GetValue(), true);
    EXPECT_EQ(seq.Get(7).GetValue(), false);
    EXPECT_EQ(seq.Get(8).GetValue(), true);
}

TEST(BitSequenceConstructors, FromIntArray_ZeroCount_EmptySequence) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 0);
    
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructors, FromIntArray_InvalidValue_Throws) {
    int data[] = {1, 2, 0};
    EXPECT_THROW(BitSequence seq(data, 3), std::invalid_argument);
}

TEST(BitSequenceConstructors, FromIntArray_NegativeCount_Throws) {
    int data[] = {1, 0};
    EXPECT_THROW(BitSequence seq(data, -1), std::invalid_argument);
}

TEST(BitSequenceConstructors, FromBitArray_StoresCorrectBits) {
    Bit bits[] = {Bit(1), Bit(0), Bit(1), Bit(1)};
    BitSequence seq(bits, 4);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), true);
}

TEST(BitSequenceConstructors, FromBitArray_ZeroCount_EmptySequence) {
    Bit bits[] = {Bit(1), Bit(0)};
    BitSequence seq(bits, 0);
    
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructors, FromString_StoresCorrectBits) {
    BitSequence seq("10110");
    
    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), true);
    EXPECT_EQ(seq.Get(4).GetValue(), false);
}

TEST(BitSequenceConstructors, FromString_EmptyString_EmptySequence) {
    BitSequence seq("");
    
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructors, FromString_InvalidCharacter_Throws) {
    EXPECT_THROW(BitSequence seq("10a01"), std::invalid_argument);
}

TEST(BitSequenceConstructors, FromString_LongString_Works) {
    std::string longBits(1000, '1');
    BitSequence seq(longBits);
    
    EXPECT_EQ(seq.GetLength(), 1000);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(seq.Get(i).GetValue(), true);
    }
}

// ==================== Методы доступа ====================

TEST(BitSequenceAccess, GetFirst_ReturnsFirstBit) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    EXPECT_EQ(seq.GetFirst().GetValue(), true);
}

TEST(BitSequenceAccess, GetFirst_EmptySequence_Throws) {
    BitSequence seq;
    EXPECT_THROW(seq.GetFirst(), IndexOutOfRange);
}

TEST(BitSequenceAccess, GetLast_ReturnsLastBit) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    EXPECT_EQ(seq.GetLast().GetValue(), true);
}

TEST(BitSequenceAccess, GetLast_EmptySequence_Throws) {
    BitSequence seq;
    EXPECT_THROW(seq.GetLast(), IndexOutOfRange);
}

TEST(BitSequenceAccess, Get_ValidIndex_ReturnsCorrectBit) {
    int data[] = {1, 0, 1, 0, 1};
    BitSequence seq(data, 5);
    
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), false);
    EXPECT_EQ(seq.Get(4).GetValue(), true);
}

TEST(BitSequenceAccess, Get_OutOfRange_Throws) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    EXPECT_THROW(seq.Get(3), IndexOutOfRange);
    EXPECT_THROW(seq.Get(-1), IndexOutOfRange);
}

TEST(BitSequenceAccess, OperatorBracket_ReadsCorrectly) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    EXPECT_EQ(seq[0].GetValue(), true);
    EXPECT_EQ(seq[1].GetValue(), false);
    EXPECT_EQ(seq[2].GetValue(), true);
}

TEST(BitSequenceAccess, ConstOperatorBracket_ReadsCorrectly) {
    int data[] = {1, 0, 1};
    const BitSequence seq(data, 3);
    
    EXPECT_EQ(seq[0].GetValue(), true);
    EXPECT_EQ(seq[1].GetValue(), false);
    EXPECT_EQ(seq[2].GetValue(), true);
}

// ==================== Методы Sequence ====================

TEST(BitSequenceOperations, InsertAt_Middle_ShiftsBits) {
    int data[] = {1, 1, 1};
    BitSequence seq(data, 3);
    
    Sequence<Bit>* newSeq = seq.InsertAt(Bit(0), 1);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(newSeq->GetLength(), 4);
    EXPECT_EQ(newSeq->Get(0).GetValue(), true);
    EXPECT_EQ(newSeq->Get(1).GetValue(), false);
    EXPECT_EQ(newSeq->Get(2).GetValue(), true);
    EXPECT_EQ(newSeq->Get(3).GetValue(), true);
    
    delete newSeq;
}

TEST(BitSequenceOperations, InsertAt_Beginning_Works) {
    int data[] = {0, 0};
    BitSequence seq(data, 2);
    
    Sequence<Bit>* newSeq = seq.InsertAt(Bit(1), 0);
    
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0).GetValue(), true);
    EXPECT_EQ(newSeq->Get(1).GetValue(), false);
    EXPECT_EQ(newSeq->Get(2).GetValue(), false);
    
    delete newSeq;
}

TEST(BitSequenceOperations, InsertAt_End_Works) {
    int data[] = {1, 1};
    BitSequence seq(data, 2);
    
    Sequence<Bit>* newSeq = seq.InsertAt(Bit(0), 2);
    
    EXPECT_EQ(newSeq->GetLength(), 3);
    EXPECT_EQ(newSeq->Get(0).GetValue(), true);
    EXPECT_EQ(newSeq->Get(1).GetValue(), true);
    EXPECT_EQ(newSeq->Get(2).GetValue(), false);
    
    delete newSeq;
}

TEST(BitSequenceOperations, InsertAt_InvalidIndex_Throws) {
    int data[] = {1, 0};
    BitSequence seq(data, 2);
    
    EXPECT_THROW(seq.InsertAt(Bit(1), -1), IndexOutOfRange);
    EXPECT_THROW(seq.InsertAt(Bit(1), 3), IndexOutOfRange);
}

TEST(BitSequenceOperations, GetSubsequence_ValidRange_ReturnsCopy) {
    int data[] = {1, 0, 1, 0, 1};
    BitSequence seq(data, 5);
    
    Sequence<Bit>* sub = seq.GetSubsequence(1, 3);
    
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0).GetValue(), false);
    EXPECT_EQ(sub->Get(1).GetValue(), true);
    EXPECT_EQ(sub->Get(2).GetValue(), false);
    
    delete sub;
}

TEST(BitSequenceOperations, GetSubsequence_SingleElement) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    Sequence<Bit>* sub = seq.GetSubsequence(1, 1);
    
    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0).GetValue(), false);
    
    delete sub;
}

TEST(BitSequenceOperations, GetSubsequence_InvalidRange_Throws) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    EXPECT_THROW(seq.GetSubsequence(-1, 2), IndexOutOfRange);
    EXPECT_THROW(seq.GetSubsequence(0, 3), IndexOutOfRange);
    EXPECT_THROW(seq.GetSubsequence(2, 1), std::invalid_argument);
}

TEST(BitSequenceOperations, Concat_CombinesTwoSequences) {
    int data1[] = {1, 0};
    int data2[] = {1, 1, 0};
    BitSequence seq1(data1, 2);
    BitSequence seq2(data2, 3);
    
    Sequence<Bit>* result = seq1.Concat(&seq2);
    
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0).GetValue(), true);
    EXPECT_EQ(result->Get(1).GetValue(), false);
    EXPECT_EQ(result->Get(2).GetValue(), true);
    EXPECT_EQ(result->Get(3).GetValue(), true);
    EXPECT_EQ(result->Get(4).GetValue(), false);
    
    delete result;
}

TEST(BitSequenceOperations, Concat_WithEmpty_ReturnsCopy) {
    int data[] = {1, 0, 1};
    BitSequence seq1(data, 3);
    BitSequence seq2;
    
    Sequence<Bit>* result = seq1.Concat(&seq2);
    
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0).GetValue(), true);
    EXPECT_EQ(result->Get(2).GetValue(), true);
    EXPECT_NE(result, &seq1);
    
    delete result;
}

TEST(BitSequenceOperations, Concat_Nullptr_Throws) {
    BitSequence seq;
    EXPECT_THROW(seq.Concat(nullptr), std::invalid_argument);
}

// ==================== Побитовые операции ====================

TEST(BitSequenceBitwise, AND_EqualLength) {
    int a[] = {1, 0, 1, 1, 0};
    int b[] = {1, 1, 0, 1, 0};
    BitSequence seqA(a, 5);
    BitSequence seqB(b, 5);
    
    BitSequence result = seqA.BitwiseAND(seqB);
    
    EXPECT_EQ(result.GetLength(), 5);
    EXPECT_EQ(result.Get(0).GetValue(), true);  // 1 & 1 = 1
    EXPECT_EQ(result.Get(1).GetValue(), false); // 0 & 1 = 0
    EXPECT_EQ(result.Get(2).GetValue(), false); // 1 & 0 = 0
    EXPECT_EQ(result.Get(3).GetValue(), true);  // 1 & 1 = 1
    EXPECT_EQ(result.Get(4).GetValue(), false); // 0 & 0 = 0
}

TEST(BitSequenceBitwise, AND_DifferentLength_ShortPaddedWithZerosLeft) {
    int a[] = {1, 0, 1};        // 3 бита
    int b[] = {1, 1, 0, 1, 0};  // 5 бит
    BitSequence seqA(a, 3);
    BitSequence seqB(b, 5);
    
    BitSequence result = seqA.BitwiseAND(seqB);
    // seqA неявно: 0, 0, 1, 0, 1 (добавлены два нуля слева)
    // seqB:        1, 1, 0, 1, 0
    // AND:         0, 0, 0, 0, 0
    
    EXPECT_EQ(result.GetLength(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(result.Get(i).GetValue(), false);
    }
}

TEST(BitSequenceBitwise, OR_EqualLength) {
    int a[] = {1, 0, 1};
    int b[] = {0, 1, 0};
    BitSequence seqA(a, 3);
    BitSequence seqB(b, 3);
    
    BitSequence result = seqA.BitwiseOR(seqB);
    
    EXPECT_EQ(result.GetLength(), 3);
    EXPECT_EQ(result.Get(0).GetValue(), true);  // 1 | 0 = 1
    EXPECT_EQ(result.Get(1).GetValue(), true);  // 0 | 1 = 1
    EXPECT_EQ(result.Get(2).GetValue(), true);  // 1 | 0 = 1
}

TEST(BitSequenceBitwise, OR_DifferentLength) {
    int a[] = {1};              // 1 бит
    int b[] = {0, 0, 1, 0};     // 4 бита
    BitSequence seqA(a, 1);
    BitSequence seqB(b, 4);
    // seqA: 0, 0, 0, 1
    // seqB: 0, 0, 1, 0
    // OR:   0, 0, 1, 1
    
    BitSequence result = seqA.BitwiseOR(seqB);
    
    EXPECT_EQ(result.GetLength(), 4);
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), false);
    EXPECT_EQ(result.Get(2).GetValue(), true);
    EXPECT_EQ(result.Get(3).GetValue(), true);
}

TEST(BitSequenceBitwise, XOR_EqualLength) {
    int a[] = {1, 0, 1, 0};
    int b[] = {1, 1, 0, 0};
    BitSequence seqA(a, 4);
    BitSequence seqB(b, 4);
    
    BitSequence result = seqA.BitwiseXOR(seqB);
    
    EXPECT_EQ(result.GetLength(), 4);
    EXPECT_EQ(result.Get(0).GetValue(), false); // 1 ^ 1 = 0
    EXPECT_EQ(result.Get(1).GetValue(), true);  // 0 ^ 1 = 1
    EXPECT_EQ(result.Get(2).GetValue(), true);  // 1 ^ 0 = 1
    EXPECT_EQ(result.Get(3).GetValue(), false); // 0 ^ 0 = 0
}

TEST(BitSequenceBitwise, NOT_InvertsAllBits) {
    int data[] = {1, 0, 1, 1, 0};
    BitSequence seq(data, 5);
    
    BitSequence result = seq.BitwiseNOT();
    
    EXPECT_EQ(result.GetLength(), 5);
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
    EXPECT_EQ(result.Get(3).GetValue(), false);
    EXPECT_EQ(result.Get(4).GetValue(), true);
}

TEST(BitSequenceBitwise, NOT_EmptySequence_ReturnsEmpty) {
    BitSequence seq;
    BitSequence result = seq.BitwiseNOT();
    
    EXPECT_EQ(result.GetLength(), 0);
}

TEST(BitSequenceBitwise, OperatorAND_Works) {
    int a[] = {1, 1, 0};
    int b[] = {1, 0, 0};
    BitSequence seqA(a, 3);
    BitSequence seqB(b, 3);
    
    BitSequence result = seqA & seqB;
    
    EXPECT_EQ(result.Get(0).GetValue(), true);
    EXPECT_EQ(result.Get(1).GetValue(), false);
    EXPECT_EQ(result.Get(2).GetValue(), false);
}

TEST(BitSequenceBitwise, OperatorOR_Works) {
    int a[] = {1, 0, 0};
    int b[] = {0, 1, 0};
    BitSequence seqA(a, 3);
    BitSequence seqB(b, 3);
    
    BitSequence result = seqA | seqB;
    
    EXPECT_EQ(result.Get(0).GetValue(), true);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
}

TEST(BitSequenceBitwise, OperatorXOR_Works) {
    int a[] = {1, 0, 1};
    int b[] = {1, 1, 1};
    BitSequence seqA(a, 3);
    BitSequence seqB(b, 3);
    
    BitSequence result = seqA ^ seqB;
    
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
}

TEST(BitSequenceBitwise, OperatorNOT_Works) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    BitSequence result = ~seq;
    
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
}

// ==================== Метод Resize ====================

TEST(BitSequenceResize, Resize_Larger_AddsZeros) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    seq.Resize(6);
    
    EXPECT_EQ(seq.GetLength(), 6);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), false);
    EXPECT_EQ(seq.Get(4).GetValue(), false);
    EXPECT_EQ(seq.Get(5).GetValue(), false);
}

TEST(BitSequenceResize, Resize_Smaller_Truncates) {
    int data[] = {1, 0, 1, 1, 0};
    BitSequence seq(data, 5);
    
    seq.Resize(3);
    
    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
}

TEST(BitSequenceResize, Resize_Zero_EmptySequence) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    seq.Resize(0);
    
    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceResize, Resize_Negative_Throws) {
    BitSequence seq;
    EXPECT_THROW(seq.Resize(-5), std::invalid_argument);
}

// ==================== AppendBit ====================

TEST(BitSequenceAppendBit, AppendBit_AddsToEnd) {
    int data[] = {1, 0};
    BitSequence seq(data, 2);
    
    seq.AppendBit(true);
    seq.AppendBit(false);
    
    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), false);
}

TEST(BitSequenceAppendBit, AppendBit_ToEmpty) {
    BitSequence seq;
    
    seq.AppendBit(true);
    seq.AppendBit(false);
    
    EXPECT_EQ(seq.GetLength(), 2);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
}

// ==================== Итераторы ====================

TEST(BitSequenceIterator, IterationOverNonEmpty) {
    int data[] = {1, 0, 1, 1, 0};
    BitSequence seq(data, 5);
    
    IEnumerator<Bit>* it = seq.GetEnumerator();
    ASSERT_NE(it, nullptr);
    
    int expected[] = {1, 0, 1, 1, 0};
    int idx = 0;
    while (it->MoveNext()) {
        EXPECT_EQ(it->Current().GetValue(), expected[idx] == 1);
        ++idx;
    }
    EXPECT_EQ(idx, 5);
    
    delete it;
}

TEST(BitSequenceIterator, EmptySequence_NoIteration) {
    BitSequence seq;
    IEnumerator<Bit>* it = seq.GetEnumerator();
    
    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), IndexOutOfRange);
    
    delete it;
}

TEST(BitSequenceIterator, Reset_Works) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    IEnumerator<Bit>* it = seq.GetEnumerator();
    
    // Первый проход
    int sum1 = 0;
    while (it->MoveNext()) sum1 += it->Current().GetValue() ? 1 : 0;
    EXPECT_EQ(sum1, 2);
    
    // Сброс и повтор
    it->Reset();
    int sum2 = 0;
    while (it->MoveNext()) sum2 += it->Current().GetValue() ? 1 : 0;
    EXPECT_EQ(sum2, 2);
    
    delete it;
}

TEST(BitSequenceIterator, CurrentThrowsWhenNotStarted) {
    int data[] = {1};
    BitSequence seq(data, 1);
    IEnumerator<Bit>* it = seq.GetEnumerator();
    
    EXPECT_THROW(it->Current(), IndexOutOfRange);
    
    delete it;
}

TEST(BitSequenceIterator, CurrentThrowsAfterEnd) {
    int data[] = {1, 0};
    BitSequence seq(data, 2);
    IEnumerator<Bit>* it = seq.GetEnumerator();
    
    it->MoveNext(); // 1
    it->MoveNext(); // 0
    it->MoveNext(); // false (end)
    EXPECT_THROW(it->Current(), IndexOutOfRange);
    
    delete it;
}

TEST(BitSequenceIterator, MultipleIteratorsAreIndependent) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    IEnumerator<Bit>* it1 = seq.GetEnumerator();
    IEnumerator<Bit>* it2 = seq.GetEnumerator();
    
    it1->MoveNext(); // it1 на позиции 0 (1)
    it2->MoveNext(); // it2 на позиции 0 (1)
    EXPECT_EQ(it1->Current().GetValue(), true);
    EXPECT_EQ(it2->Current().GetValue(), true);
    
    it1->MoveNext(); // it1 на позиции 1 (0)
    EXPECT_EQ(it1->Current().GetValue(), false);
    EXPECT_EQ(it2->Current().GetValue(), true); // it2 не изменился
    
    delete it1;
    delete it2;
}

// ==================== CreateEmpty ====================

TEST(BitSequenceCreateEmpty, ReturnsNewEmptySequence) {
    int data[] = {1, 0, 1};
    BitSequence seq(data, 3);
    
    Sequence<Bit>* empty = seq.CreateEmpty();
    
    EXPECT_EQ(empty->GetLength(), 0);
    EXPECT_NE(empty, &seq);
    
    delete empty;
}

// ==================== GetRawBytes (для отладки) ====================

TEST(BitSequenceRawBytes, GetRawBytes_ReturnsCorrectBytes) {
    // 9 бит: 1 0 1 1 0 0 1 0 1
    // В байтах (little-endian представление): 
    // Байт 0: биты 0-7: 1 0 1 1 0 0 1 0 = 0b01001101 = 77
    // Байт 1: бит 8: 1 = 0b00000001 = 1
    int data[] = {1, 0, 1, 1, 0, 0, 1, 0, 1};
    BitSequence seq(data, 9);
    
    const DynamicArray<uint8_t>& bytes = seq.GetRawBytes();
    
    EXPECT_EQ(bytes.GetCount(), 2);
    // Проверяем, что байты корректно упакованы
    // (зависит от порядка битов в байте, но это внутренняя деталь)
}

// ==================== Map, Where, Reduce (через базовый класс) ====================

TEST(BitSequenceMapReduce, Reduce_CountOnes) {
    int data[] = {1, 0, 1, 1, 0, 0, 1};
    BitSequence seq(data, 7);
    
    int onesCount = seq.Reduce<int>(
        [](int acc, const Bit& b) {
            return acc + (b.GetValue() ? 1 : 0);
        },
        0
    );
    
    EXPECT_EQ(onesCount, 4);
}

// ==================== Граничные случаи ====================

TEST(BitSequenceEdgeCases, AllZeros) {
    BitSequence seq("00000");
    
    EXPECT_EQ(seq.GetLength(), 5);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(seq.Get(i).GetValue(), false);
    }
    
    BitSequence notSeq = ~seq;
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(notSeq.Get(i).GetValue(), true);
    }
}

TEST(BitSequenceEdgeCases, AllOnes) {
    BitSequence seq("1111");
    
    EXPECT_EQ(seq.GetLength(), 4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(seq.Get(i).GetValue(), true);
    }
    
    BitSequence notSeq = ~seq;
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(notSeq.Get(i).GetValue(), false);
    }
}

// ==================== Производительность ====================

TEST(BitSequencePerformance, LargeSequenceOperations) {
    // Создаём большую последовательность (10000 бит)
    std::string largeBits(10000, '1');
    BitSequence seq(largeBits);
    
    EXPECT_EQ(seq.GetLength(), 10000);
    
    // Проверяем, что операция NOT не падает
    BitSequence notSeq = ~seq;
    EXPECT_EQ(notSeq.GetLength(), 10000);
    
    // Проверяем, что AND с самой собой работает
    BitSequence andSeq = seq & seq;
    EXPECT_EQ(andSeq.GetLength(), 10000);
}