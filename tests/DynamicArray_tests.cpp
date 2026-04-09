#include <gtest/gtest.h>
#include "../src/DynamicArray.h"

// Конструкторы

TEST( DynamicArrayConstructors, FromRawArray_StoresCorrectValue ) {
    int items[] = {1, 2, 3, 4};
    DynamicArray<int> arr( items, 4 );

    EXPECT_EQ( arr.GetCount(), 4 );
    EXPECT_EQ( arr.Get(0 ), 1 );
    EXPECT_EQ( arr.Get( 1 ), 2 );
    EXPECT_EQ( arr.Get( 2 ), 3 );
    EXPECT_EQ( arr.Get( 3 ), 4 );
}

TEST( DynamicArrayConstructors, FromRawArray_ZeroCount_EmptyArray ) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr( items, 0 );

    EXPECT_EQ( arr.GetCount(), 0 );
}

TEST( DynamicArrayConstructors, FromSize_CorrectSize ) {
    DynamicArray<int> arr( 5 );

    EXPECT_EQ( arr.GetCount(), 5 );
}

TEST( DynamicArrayConstructors, FromSize_Zero_EmptyArray ) {
    DynamicArray<int> arr( 0 );

    EXPECT_EQ( arr.GetCount(), 0 );
}

TEST( DynamicArrayConstructors, CopyConstructor_MakesDeepCope ) {
    int items[] = {10, 20, 30};
    DynamicArray<int> original( items, 3 );
    DynamicArray<int> copied( original );

    // При измене копии оригинал не должен измениться
    copied.Set( 0, 999 );

    EXPECT_EQ( original.Get( 0 ), 10 );
    EXPECT_EQ( copied.Get( 0 ), 999 );
    EXPECT_EQ( copied.GetCount(), 3 );
}

TEST( DynamicArrayConstructors, CopyConstructor_EmptyArray ) {
    DynamicArray<int> original( 0 );
    DynamicArray<int> copied( original );

    EXPECT_EQ( copied.GetCount(), 0 );
}

// Оператор присваивания

TEST( DynamicArrayAssignment, AssignmentOperator_MakesDeepCopy ) {
    int items1[] = {1, 2, 3};
    int items2[] = {9, 9};

    DynamicArray<int> a( items1, 3 );
    DynamicArray<int> b( items2, 2 );

    b = a;
    b.Set( 0, 777 );

    EXPECT_EQ( a.Get(0), 1 );
    EXPECT_EQ( b.Get( 0 ), 777 );
    EXPECT_EQ( b.GetCount(), 3 );
}

TEST( DynamicArrayAssignment, SelfAssignment_NoCorruption ) {
    int items[] = {1 ,2 ,3};
    DynamicArray<int> arr( items, 3 );

    arr = arr; // Самоприсваивание не должно ничего ломать

    EXPECT_EQ( arr.GetCount(), 3 );
    EXPECT_EQ( arr.Get( 0 ), 1 );
    EXPECT_EQ( arr.Get( 2 ), 3 );
}

// Геттеры и Сеттеры
TEST( DynamicArrayAccess, Set_ThenGet_ReturnCorrectValue ) {
    DynamicArray<int> arr( 3 );

    arr.Set( 0, 100 );
    arr.Set( 1, 200 );
    arr.Set( 2, 300 );

    EXPECT_EQ( arr.Get( 0 ), 100 );
    EXPECT_EQ( arr.Get( 1 ), 200 );
    EXPECT_EQ( arr.Get( 2 ), 300 );
}

TEST( DynamicArrayAccess, OperatorBracket_ReadAndWrite ) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr( items, 3 );

    arr[1] = 52;

    EXPECT_EQ( arr[0], 1 );
    EXPECT_EQ( arr[1], 52 );
    EXPECT_EQ( arr[2], 3 );
}

TEST( DynamicArrayAccess, ConstArray_OperatorBracket_ReadsCorretly ) {
    int items[] = {5, 6, 7};
    const DynamicArray<int> arr( items, 3 );

    EXPECT_EQ( arr[0], 5 );
    EXPECT_EQ( arr[2], 7 );
}

// Resize

TEST(DynamicArrayResize, Grow_PreservesOldElements) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);

    arr.Resize(5);

    EXPECT_EQ(arr.GetCount(), 5);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(DynamicArrayResize, Shrink_KeepsFirstElements) {
    int items[] = {1, 2, 3, 4, 5};
    DynamicArray<int> arr(items, 5);

    arr.Resize(2);

    EXPECT_EQ(arr.GetCount(), 2);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(1), 2);
}

TEST(DynamicArrayResize, ToZero_EmptyArray) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);

    arr.Resize(0);

    EXPECT_EQ(arr.GetCount(), 0);
}

TEST(DynamicArrayResize, FromZero_Grow) {
    DynamicArray<int> arr(0);

    arr.Resize(3);
    arr.Set(0, 10);
    arr.Set(2, 30);

    EXPECT_EQ(arr.GetCount(), 3);
    EXPECT_EQ(arr.Get(0), 10);
    EXPECT_EQ(arr.Get(2), 30);
}

// Исключения

TEST(DynamicArrayExceptions, Get_NegativeIndex_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(3);

    EXPECT_THROW(arr.Get(-1), IndexOutOfRange);
}

TEST(DynamicArrayExceptions, Get_IndexEqualSize_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(3);

    EXPECT_THROW(arr.Get(3), IndexOutOfRange);  // допустимы только 0,1,2
}

TEST(DynamicArrayExceptions, Get_EmptyArray_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(0);

    EXPECT_THROW(arr.Get(0), IndexOutOfRange);
}

TEST(DynamicArrayExceptions, Set_NegativeIndex_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(3);

    EXPECT_THROW(arr.Set(-1, 5), IndexOutOfRange);
}

TEST(DynamicArrayExceptions, Set_IndexEqualSize_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(3);

    EXPECT_THROW(arr.Set(3, 5), IndexOutOfRange);
}

TEST(DynamicArrayExceptions, OperatorBracket_OutOfRange_ThrowsIndexOutOfRange) {
    DynamicArray<int> arr(2);

    EXPECT_THROW(arr[5], IndexOutOfRange);
}

TEST(DynamicArrayExceptions, Constructor_NegativeSize_ThrowsInvalidArgument) {
    EXPECT_THROW(DynamicArray<int> arr(-1), std::invalid_argument);
}

TEST(DynamicArrayExceptions, Constructor_NegativeCount_ThrowsInvalidArgument) {
    int items[] = {1, 2, 3};

    EXPECT_THROW(DynamicArray<int> arr(items, -1), std::invalid_argument);
}

TEST(DynamicArrayExceptions, Constructor_NullptrWithPositiveCount_ThrowsInvalidArgument) {
    EXPECT_THROW(DynamicArray<int> arr(nullptr, 3), std::invalid_argument);
}

TEST(DynamicArrayExceptions, Resize_NegativeSize_ThrowsInvalidArgument) {
    DynamicArray<int> arr(3);

    EXPECT_THROW(arr.Resize(-1), std::invalid_argument);
}

// Шаблон с другим типом

TEST(DynamicArrayTemplate, WorksWithDouble) {
    double items[] = {1.1, 2.2, 3.3};
    DynamicArray<double> arr(items, 3);

    EXPECT_DOUBLE_EQ(arr.Get(0), 1.1);
    EXPECT_DOUBLE_EQ(arr.Get(2), 3.3);
}

TEST(DynamicArrayTemplate, WorksWithString) {
    std::string items[] = {"hello", "world"};
    DynamicArray<std::string> arr(items, 2);

    EXPECT_EQ(arr.Get(0), "hello");
    EXPECT_EQ(arr.Get(1), "world");
}