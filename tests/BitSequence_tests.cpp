#include <gtest/gtest.h>
#include "../src/BitSequence.h"

// ==================== Конструкторы ====================

TEST(BitSequenceConstructors, DefaultConstructor_EmptySequence) {
    BitSequence seq;

    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructors, FromIntArray_StoresCorrectValues) {
    int values[] = {1, 0, 1, 1, 0};
    BitSequence seq(values, 5);

    EXPECT_EQ(seq.GetLength(), 5);
    EXPECT_EQ(seq.Get(0).GetValue(), true);
    EXPECT_EQ(seq.Get(1).GetValue(), false);
    EXPECT_EQ(seq.Get(2).GetValue(), true);
    EXPECT_EQ(seq.Get(3).GetValue(), true);
    EXPECT_EQ(seq.Get(4).GetValue(), false);
}

TEST(BitSequenceConstructors, FromString_StoresCorrectValues) {
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

TEST(BitSequenceConstructors, FromIntArray_ZeroCount_EmptySequence) {
    int values[] = {1, 0, 1};
    BitSequence seq(values, 0);

    EXPECT_EQ(seq.GetLength(), 0);
}

TEST(BitSequenceConstructors, CopyConstructor_MakesDeepCopy) {
    BitSequence original("101");
    BitSequence copy(original);

    copy.Append(Bit(1));

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(copy.GetLength(), 4);
    EXPECT_EQ(copy.GetLast().GetValue(), true);
}

// ==================== Побитовые операции (методы) ====================

TEST(BitSequenceBitwise, AND_CorrectResult) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence result = a.BitwiseAND(b);

    // 1&1=1, 0&1=0, 1&0=0, 0&0=0
    EXPECT_EQ(result.Get(0).GetValue(), true);
    EXPECT_EQ(result.Get(1).GetValue(), false);
    EXPECT_EQ(result.Get(2).GetValue(), false);
    EXPECT_EQ(result.Get(3).GetValue(), false);
}

TEST(BitSequenceBitwise, OR_CorrectResult) {
    BitSequence a("1010");
    BitSequence b("0100");

    BitSequence result = a.BitwiseOR(b);

    // 1|0=1, 0|1=1, 1|0=1, 0|0=0
    EXPECT_EQ(result.Get(0).GetValue(), true);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), true);
    EXPECT_EQ(result.Get(3).GetValue(), false);
}

TEST(BitSequenceBitwise, XOR_CorrectResult) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence result = a.BitwiseXOR(b);

    // 1^1=0, 0^1=1, 1^0=1, 0^0=0
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), true);
    EXPECT_EQ(result.Get(3).GetValue(), false);
}

TEST(BitSequenceBitwise, NOT_CorrectResult) {
    BitSequence seq("1010");

    BitSequence result = seq.BitwiseNOT();

    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
    EXPECT_EQ(result.Get(3).GetValue(), true);
}

TEST(BitSequenceBitwise, NOT_EmptySequence_ReturnsEmptySequence) {
    BitSequence seq;

    BitSequence result = seq.BitwiseNOT();

    EXPECT_EQ(result.GetLength(), 0);
}

TEST(BitSequenceBitwise, Operations_DoNotModifyOriginals) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence r1 = a.BitwiseAND(b);
    BitSequence r2 = a.BitwiseOR(b);
    BitSequence r3 = a.BitwiseXOR(b);
    BitSequence r4 = a.BitwiseNOT();

    // Оригиналы не изменились
    EXPECT_EQ(a.GetLength(), 4);
    EXPECT_EQ(a.Get(0).GetValue(), true);
    EXPECT_EQ(b.GetLength(), 4);
}

// ==================== Перегрузка операторов ====================

TEST(BitSequenceOperators, OperatorAND_SameResultAsMethod) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence r1 = a.BitwiseAND(b);
    BitSequence r2 = a & b;

    EXPECT_EQ(r1.GetLength(), r2.GetLength());
    for (int i = 0; i < r1.GetLength(); ++i) {
        EXPECT_EQ(r1.Get(i).GetValue(), r2.Get(i).GetValue());
    }
}

TEST(BitSequenceOperators, OperatorOR_SameResultAsMethod) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence r1 = a.BitwiseOR(b);
    BitSequence r2 = a | b;

    for (int i = 0; i < r1.GetLength(); ++i) {
        EXPECT_EQ(r1.Get(i).GetValue(), r2.Get(i).GetValue());
    }
}

TEST(BitSequenceOperators, OperatorXOR_SameResultAsMethod) {
    BitSequence a("1010");
    BitSequence b("1100");

    BitSequence r1 = a.BitwiseXOR(b);
    BitSequence r2 = a ^ b;

    for (int i = 0; i < r1.GetLength(); ++i) {
        EXPECT_EQ(r1.Get(i).GetValue(), r2.Get(i).GetValue());
    }
}

TEST(BitSequenceOperators, OperatorNOT_SameResultAsMethod) {
    BitSequence seq("1010");

    BitSequence r1 = seq.BitwiseNOT();
    BitSequence r2 = ~seq;

    for (int i = 0; i < r1.GetLength(); ++i) {
        EXPECT_EQ(r1.Get(i).GetValue(), r2.Get(i).GetValue());
    }
}

// ==================== Использование как битовая маска ====================

TEST(BitSequenceMask, AND_Mask_FiltersCorrectly) {
    BitSequence data("1111");
    BitSequence mask("1010");

    BitSequence result = data & mask;

    EXPECT_EQ(result.Get(0).GetValue(), true);   // 1 & 1 = 1
    EXPECT_EQ(result.Get(1).GetValue(), false);  // 1 & 0 = 0
    EXPECT_EQ(result.Get(2).GetValue(), true);   // 1 & 1 = 1
    EXPECT_EQ(result.Get(3).GetValue(), false);  // 1 & 0 = 0
}

TEST(BitSequenceMask, OR_Mask_SetsCorrectly) {
    BitSequence data("0000");
    BitSequence mask("1010");

    BitSequence result = data | mask;

    EXPECT_EQ(result.Get(0).GetValue(), true);
    EXPECT_EQ(result.Get(1).GetValue(), false);
    EXPECT_EQ(result.Get(2).GetValue(), true);
    EXPECT_EQ(result.Get(3).GetValue(), false);
}

TEST(BitSequenceMask, XOR_Mask_TogglesCorrectly) {
    BitSequence data("1010");
    BitSequence mask("1111");  // инвертируем все биты через XOR

    BitSequence result = data ^ mask;

    // XOR с 1 инвертирует бит
    EXPECT_EQ(result.Get(0).GetValue(), false);
    EXPECT_EQ(result.Get(1).GetValue(), true);
    EXPECT_EQ(result.Get(2).GetValue(), false);
    EXPECT_EQ(result.Get(3).GetValue(), true);
}

// ==================== Исключения ====================

TEST(BitSequenceExceptions, AND_DifferentLengths_ThrowsInvalidArgument) {
    BitSequence a("101");
    BitSequence b("10");

    EXPECT_THROW(a.BitwiseAND(b), std::invalid_argument);
}

TEST(BitSequenceExceptions, OR_DifferentLengths_ThrowsInvalidArgument) {
    BitSequence a("10");
    BitSequence b("101");

    EXPECT_THROW(a.BitwiseOR(b), std::invalid_argument);
}

TEST(BitSequenceExceptions, XOR_DifferentLengths_ThrowsInvalidArgument) {
    BitSequence a("1010");
    BitSequence b("10");

    EXPECT_THROW(a.BitwiseXOR(b), std::invalid_argument);
}

TEST(BitSequenceExceptions, Constructor_NegativeCount_ThrowsInvalidArgument) {
    int values[] = {1, 0};

    EXPECT_THROW(BitSequence(values, -1), std::invalid_argument);
}

TEST(BitSequenceExceptions, Constructor_NullptrWithPositiveCount_ThrowsInvalidArgument) {
    EXPECT_THROW(BitSequence(static_cast<int*>(nullptr), 3), std::invalid_argument);
}

TEST(BitSequenceExceptions, Constructor_InvalidStringChar_ThrowsInvalidArgument) {
    EXPECT_THROW(BitSequence("10201"), std::invalid_argument);
    EXPECT_THROW(BitSequence("abc"), std::invalid_argument);
}

TEST(BitSequenceExceptions, Bit_InvalidValue_ThrowsInvalidArgument) {
    EXPECT_THROW(Bit(5), std::invalid_argument);
    EXPECT_THROW(Bit(-1), std::invalid_argument);
}

// ==================== Совместимость с Sequence<Bit>* ====================

TEST(BitSequencePolymorphism, WorksThroughSequencePointer) {
    BitSequence bits("1011");
    Sequence<Bit>* seq = &bits;

    EXPECT_EQ(seq->GetLength(), 4);
    EXPECT_EQ(seq->GetFirst().GetValue(), true);
    EXPECT_EQ(seq->GetLast().GetValue(), true);
    EXPECT_EQ(seq->Get(1).GetValue(), false);
}
