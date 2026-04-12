#include <gtest/gtest.h>
#include "../src/Sequence.h"
#include "../src/Sequence_utils.h"
#include "../src/ListSequence.h"

// Map
TEST(MapTests, Map_MultiplyByTwo) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    Sequence<int> *result = seq.Map([]( const int &x ) { return x * 2; } );

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 6);
    EXPECT_EQ(result->Get(3), 8);

    delete result;
}

TEST(MapTests, Map_DoesNotModifyOriginal) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Map([]( const int &x ) { return x + 10; });

    EXPECT_EQ(seq.Get(0), 1); // оригинал не изменился
    EXPECT_EQ(seq.Get(1), 2);
    EXPECT_EQ(seq.Get(2), 3);

    delete result;
}

TEST(MapTests, Map_EmptySequence_ReturnsEmpty) {
    MutableArraySequence<int> seq;

    Sequence<int> *result = seq.Map([]( const int &x ) { return x * 2; });

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}

TEST(MapTests, Map_OnListSequence) {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);

    Sequence<int> *result = seq.Map([]( int x ) { return x * x; });

    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 9);

    delete result;
}

TEST(MapTests, Map_Immutable_ResultIsNewObject) {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Map([]( int x ) { return x + 1; });

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 3);
    EXPECT_EQ(result->Get(2), 4);

    delete result;
}

// Where
TEST(WhereTests, Where_FilterEvenNumbers) {
    int data[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> seq(data, 6);

    Sequence<int> *result = seq.Where([]( int x ) { return x % 2 == 0; });

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 4);
    EXPECT_EQ(result->Get(2), 6);

    delete result;
}


TEST(WhereTests, Where_NoneMatch_ReturnsEmpty) {
    int data[] = {1, 3, 5};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Where([]( const int &x ) { return x % 2 == 0; });

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}

TEST(WhereTests, Where_AllMatch_ReturnsAll) {
    int data[] = {2, 4, 6};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Where([]( const int &x ) { return x % 2 == 0; });

    EXPECT_EQ(result->GetLength(), 3);

    delete result;
}

TEST(WhereTests, Where_DoesNotModifyOriginal) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    Sequence<int> *result = seq.Where([]( const int &x ) { return x > 2; });

    EXPECT_EQ(seq.GetLength(), 4); // оригинал не изменился

    delete result;
}

TEST(WhereTests, Where_OnListSequence) {
    int data[] = {10, 20, 3, 40};
    MutableListSequence<int> seq(data, 4);

    Sequence<int> *result = seq.Where([]( const int &x ) { return x > 10; });

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Get(0), 20);
    EXPECT_EQ(result->Get(1), 40);

    delete result;
}

// Reduce
TEST(ReduceTests, Reduce_Sum) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    int sum = seq.Reduce<int>(
        []( int acc, const int &x ) { return acc + x; },
        0
    );

    EXPECT_EQ(sum, 15);
}

TEST(ReduceTests, Reduce_Product) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    int product = seq.Reduce<int>(
        []( int acc, const int &x ) { return acc * x; },
        1
    );

    EXPECT_EQ(product, 24);
}

TEST(ReduceTests, Reduce_Max) {
    int data[] = {3, 1, 4, 1, 5, 9, 2, 6};
    MutableArraySequence<int> seq(data, 8);

    int max = seq.Reduce<int>(
        []( int acc, const int &x ) { return x > acc ? x : acc; },
        data[0]
    );

    EXPECT_EQ(max, 9);
}

TEST(ReduceTests, Reduce_EmptySequence_ReturnsInitial) {
    MutableArraySequence<int> seq;

    int result = seq.Reduce<int>(
        []( int acc, const int &x ) { return acc + x; },
        42
    );

    EXPECT_EQ(result, 42); // вернули начальное значение
}

TEST(ReduceTests, Reduce_OnListSequence) {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);

    int sum = seq.Reduce<int>(
        []( int acc, const int &x ) { return acc + x; },
        0
    );

    EXPECT_EQ(sum, 6);
}

// Цепочки операций
TEST(ChainTests, MapThenWhere) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    Sequence<int> *mapped = seq.Map([]( int x ) { return x * 2; });
    Sequence<int> *filtered = mapped->Where([]( int x ) { return x > 5; });

    EXPECT_EQ(filtered->GetLength(), 3);
    EXPECT_EQ(filtered->Get(0), 6);
    EXPECT_EQ(filtered->Get(1), 8);
    EXPECT_EQ(filtered->Get(2), 10);

    delete mapped;
    delete filtered;
}


TEST(ChainTests, WhereThenReduce_SumOfEvens) {
    int data[] = {1, 2, 3, 4, 5, 6};
    MutableArraySequence<int> seq(data, 6);

    // Фильтруем чётные, считаем сумму
    Sequence<int> *evens = seq.Where([]( const int &x ) { return x % 2 == 0; });
    int sum = evens->Reduce<int>([]( int acc, const int &x ) { return acc + x; }, 0);

    EXPECT_EQ(sum, 12); // 2 + 4 + 6

    delete evens;
}

TEST(ChainTests, MapThenReduce_SumOfSquares) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    Sequence<int> *squares = seq.Map([]( const int &x ) { return x * x; });
    int sum = squares->Reduce<int>([]( int acc, const int &x ) { return acc + x; }, 0);

    EXPECT_EQ(sum, 30); // 1 + 4 + 9 + 16

    delete squares;
}

// П-5: префиксы и постфиксы

TEST(PrefixesTests, BasicPrefixes) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<Sequence<int> *> *prefixes = GetPrefixes(&seq);

    // Должно быть 3 префикса
    EXPECT_EQ(prefixes->GetLength(), 3);

    // [1]
    EXPECT_EQ(prefixes->Get(0)->GetLength(), 1);
    EXPECT_EQ(prefixes->Get(0)->Get(0), 1);

    // [1, 2]
    EXPECT_EQ(prefixes->Get(1)->GetLength(), 2);
    EXPECT_EQ(prefixes->Get(1)->Get(0), 1);
    EXPECT_EQ(prefixes->Get(1)->Get(1), 2);

    // [1, 2, 3]
    EXPECT_EQ(prefixes->Get(2)->GetLength(), 3);
    EXPECT_EQ(prefixes->Get(2)->Get(2), 3);

    for (int i = 0; i < prefixes->GetLength(); ++i) delete prefixes->Get(i);
    delete prefixes;
}

TEST(SuffixesTests, BasicSuffixes) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<Sequence<int> *> *postfixes = GetPostfixes(&seq);

    EXPECT_EQ(postfixes->GetLength(), 3); // ← было suffixes
    EXPECT_EQ(postfixes->Get(0)->GetLength(), 3);
    EXPECT_EQ(postfixes->Get(0)->Get(0), 1);
    EXPECT_EQ(postfixes->Get(1)->GetLength(), 2);
    EXPECT_EQ(postfixes->Get(1)->Get(0), 2);
    EXPECT_EQ(postfixes->Get(2)->GetLength(), 1);
    EXPECT_EQ(postfixes->Get(2)->Get(0), 3);

    for (int i = 0; i < postfixes->GetLength(); ++i) delete postfixes->Get(i);
    delete postfixes;
}

TEST(PrefixesTests, SingleElement) {
    int data[] = {42};
    MutableArraySequence<int> seq(data, 1);

    Sequence<Sequence<int> *> *prefixes = GetPrefixes(&seq);

    EXPECT_EQ(prefixes->GetLength(), 1);
    EXPECT_EQ(prefixes->Get(0)->Get(0), 42);

    delete prefixes->Get(0);
    delete prefixes;
}

TEST(PrefixesTests, EmptySequence) {
    MutableArraySequence<int> seq;

    Sequence<Sequence<int> *> *prefixes = GetPrefixes(&seq);
    EXPECT_EQ(prefixes->GetLength(), 0);

    delete prefixes;
}

// М-2.1: FlatMap

TEST(FlatMapTests, FlatMap_EachElementToTwoElements) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    // Каждый элемент x → [x, x*10]
    Sequence<int> *result = seq.FlatMap<int>(
        []( const int &x ) -> Sequence<int> * {
            int pair[] = {x, x * 10};
            return new MutableArraySequence<int>(pair, 2);
        });

    EXPECT_EQ(result->GetLength(), 6);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 10);
    EXPECT_EQ(result->Get(2), 2);
    EXPECT_EQ(result->Get(3), 20);
    delete result;
}

TEST(FlatMapTests, FlatMap_EmptyInnerSequence_Flattens) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    // Нечётные пропускаем, чётные дублируем
    Sequence<int> *result = seq.FlatMap<int>(
        []( const int &x ) -> Sequence<int> * {
            if (x % 2 == 0) {
                int pair[] = {x, x};
                return new MutableArraySequence<int>(pair, 2);
            }
            return new MutableArraySequence<int>();
        });

    EXPECT_EQ(result->GetLength(), 2); // только 2 → [2, 2]
    EXPECT_EQ(result->Get(0), 2);
    EXPECT_EQ(result->Get(1), 2);
    delete result;
}

// М-2.1: Zip / Unzip

TEST(ZipTests, Zip_EqualLength) {
    int a[] = {1, 2, 3};
    std::string b[] = {"one", "two", "three"};
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<std::string> seqB(b, 3);

    auto *result = seqA.Zip<std::string>(&seqB);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0).first, 1);
    EXPECT_EQ(result->Get(0).second, "one");
    EXPECT_EQ(result->Get(2).first, 3);
    delete result;
}

TEST(ZipTests, Zip_DifferentLength_TakesShorter) {
    int a[] = {1, 2, 3, 4, 5};
    int b[] = {10, 20};
    MutableArraySequence<int> seqA(a, 5);
    MutableArraySequence<int> seqB(b, 2);

    auto *result = seqA.Zip<int>(&seqB);

    EXPECT_EQ(result->GetLength(), 2);
    delete result;
}

TEST(UnzipTests, Unzip_RestoresOriginals) {
    int a[] = {1, 2, 3};
    int b[] = {4, 5, 6};
    MutableArraySequence<int> seqA(a, 3);
    MutableArraySequence<int> seqB(b, 3);

    auto *zipped = seqA.Zip<int>(&seqB);
    auto [first, second] = Unzip(zipped);

    EXPECT_EQ(first->Get(0), 1);
    EXPECT_EQ(second->Get(0), 4);
    EXPECT_EQ(first->Get(2), 3);
    EXPECT_EQ(second->Get(2), 6);

    delete zipped;
    delete first;
    delete second;
}

// М-2.1: Skip / Take

TEST(SkipTakeTests, Skip_ReturnsRemainder) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    Sequence<int> *result = seq.Skip(2);
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(0), 3);
    delete result;
}

TEST(SkipTakeTests, Take_ReturnsFirstN) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    Sequence<int> *result = seq.Take(3);
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get(2), 3);
    delete result;
}

TEST(SkipTakeTests, Skip_MoreThanLength_ReturnsEmpty) {
    int data[] = {1, 2};
    MutableArraySequence<int> seq(data, 2);

    Sequence<int> *result = seq.Skip(10);
    EXPECT_EQ(result->GetLength(), 0);
    delete result;
}

TEST(SkipTakeTest, Skip_Zero_ReturnsSameContent) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Skip(0);

    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_EQ(result->Get( 0 ), 1);
    EXPECT_EQ(result->Get( 2 ), 3);

    delete result;
}

TEST(SkipTakeTest, Take_Zero_ReturnsEmpty) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int> *result = seq.Take(0);

    EXPECT_EQ(result->GetLength(), 0);

    delete result;
}

// М-2.1: Split

TEST(SplitTests, Split_ByZero) {
    int data[] = {1, 0, 2, 0, 3};
    MutableArraySequence<int> seq(data, 5);

    auto *parts = Split<int>(&seq, []( const int &x ) { return x == 0; });

    EXPECT_EQ(parts->GetLength(), 3);
    EXPECT_EQ(parts->Get(0)->GetLength(), 1); // [1]
    EXPECT_EQ(parts->Get(1)->GetLength(), 1); // [2]
    EXPECT_EQ(parts->Get(2)->GetLength(), 1); // [3]

    for (int i = 0; i < parts->GetLength(); ++i) delete parts->Get(i);
    delete parts;
}

// М-2.1: Slice

TEST(SliceTests, Slice_RemoveAndInsert) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);
    int ins[] = {9, 10};
    MutableArraySequence<int> insSeq(ins, 2);

    Sequence<int> *result = Slice<int>(&seq, 1, 2, &insSeq);

    // {1, 9, 10, 4, 5}
    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(1), 9);
    EXPECT_EQ(result->Get(2), 10);
    EXPECT_EQ(result->Get(3), 4);
    delete result;
}

TEST(SliceTests, Slice_NegativeIndex_FromEnd) {
    int data[] = {1, 2, 3, 4, 5};
    MutableArraySequence<int> seq(data, 5);

    Sequence<int> *result = Slice<int>(&seq, -2, 1); // удалить 1 эл с позиции 3
    // {1, 2, 3, 5}
    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(3), 5);
    delete result;
}

TEST(SplitTests, Split_TrailingSeparator_NoEmptySegment) {
    int data[] = {1, 0};
    MutableArraySequence<int> seq(data, 2);

    auto *parts = Split<int>(&seq, []( const int &x ) { return x == 0; });

    EXPECT_EQ(parts->GetLength(), 1); // только [1], без []
    EXPECT_EQ(parts->Get(0)->Get(0), 1);

    delete parts->Get(0);
    delete parts;
}

// М-2.1: Range

TEST(RangeTests, Range_BasicIntRange) {
    Sequence<int> *result = Range<int>(1, 5); // [1, 2, 3, 4]

    EXPECT_EQ(result->GetLength(), 4);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    delete result;
}

TEST(RangeTests, Range_WithStep) {
    Sequence<int> *result = Range<int>(0, 10, 2); // [0,2,4,6,8]

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get(2), 4);
    delete result;
}

TEST(RangeTests, Range_ZeroStep_Throws) {
    EXPECT_THROW(Range<int>(0, 5, 0), std::invalid_argument);
}

TEST(RangeTest, NegativeStep_DescendingSequence) {
    // Range(5, 0, -1) → [5, 4, 3, 2, 1]  (0 не включается)
    Sequence<int> *result = Range<int>(5, 0, -1);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get( 0 ), 5);
    EXPECT_EQ(result->Get( 1 ), 4);
    EXPECT_EQ(result->Get( 4 ), 1);

    delete result;
}

TEST(RangeTest, NegativeStep_StepMinusTwo) {
    // Range(10, 0, -2) → [10, 8, 6, 4, 2]  (0 не включается)
    Sequence<int> *result = Range<int>(10, 0, -2);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_EQ(result->Get( 0 ), 10);
    EXPECT_EQ(result->Get( 2 ), 6);
    EXPECT_EQ(result->Get( 4 ), 2);

    delete result;
}

// П-6: скользящее среднее

TEST(SlidingAverageTests, BasicThreeElements) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<double> *result = SlidingAverage(&seq);

    // a[0]: (1+2)/2 = 1.5
    // a[1]: (1+2+3)/3 = 2.0
    // a[2]: (2+3)/2 = 2.5
    EXPECT_EQ(result->GetLength(), 3);
    EXPECT_DOUBLE_EQ(result->Get(0), 1.5);
    EXPECT_DOUBLE_EQ(result->Get(1), 2.0);
    EXPECT_DOUBLE_EQ(result->Get(2), 2.5);

    delete result;
}

TEST(SlidingAverageTests, FiveElements) {
    int data[] = {2, 4, 6, 8, 10};
    MutableArraySequence<int> seq(data, 5);

    Sequence<double> *result = SlidingAverage(&seq);

    EXPECT_EQ(result->GetLength(), 5);
    EXPECT_DOUBLE_EQ(result->Get(0), 3.0); // (2+4)/2
    EXPECT_DOUBLE_EQ(result->Get(1), 4.0); // (2+4+6)/3
    EXPECT_DOUBLE_EQ(result->Get(2), 6.0); // (4+6+8)/3
    EXPECT_DOUBLE_EQ(result->Get(3), 8.0); // (6+8+10)/3
    EXPECT_DOUBLE_EQ(result->Get(4), 9.0); // (8+10)/2

    delete result;
}

TEST(SlidingAverageTests, DoesNotModifyOriginal) {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    Sequence<double> *result = SlidingAverage(&seq);

    EXPECT_EQ(seq.GetLength(), 4);
    EXPECT_EQ(seq.Get(0), 1); // оригинал не изменился

    delete result;
}

TEST(SlidingAverageTests, TooShort_Throws) {
    int data[] = {42};
    MutableArraySequence<int> seq(data, 1);

    EXPECT_THROW(SlidingAverage(&seq), std::invalid_argument);
}

TEST(MinMaxAvgTest, AvgIsNotRounded) {
    int data[] = {1, 2};
    MutableArraySequence<int> seq(data, 2);

    // avg = (1+2)/2 = 1.5 - проверяем что нет целочисленного округления
    EXPECT_DOUBLE_EQ(seq.GetAvg(), 1.5);
}