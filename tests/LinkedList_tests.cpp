#include <gtest/gtest.h>

#include "ListSequence.h"
#include "../src/LinkedList.h"

// ==================== Конструкторы ====================

TEST(LinkedListConstructors, DefaultConstructor_EmptyList) {
    LinkedList<int> list;

    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListConstructors, FromRawArray_StoresCorrectValues) {
    int items[] = {1, 2, 3, 4};
    LinkedList<int> list(items, 4);

    EXPECT_EQ(list.GetLength(), 4);
    EXPECT_EQ(list.Get(0), 1);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.Get(3), 4);
}

TEST(LinkedListConstructors, FromRawArray_ZeroCount_EmptyList) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 0);

    EXPECT_EQ(list.GetLength(), 0);
}

TEST(LinkedListConstructors, CopyConstructor_MakesDeepCopy) {
    int items[] = {10, 20, 30};
    LinkedList<int> original(items, 3);
    LinkedList<int> copy(original);

    // Меняем копию через InsertAt - оригинал не должен меняться
    copy.Prepend(999);

    EXPECT_EQ(original.GetLength(), 3);
    EXPECT_EQ(original.GetFirst(), 10);
    EXPECT_EQ(copy.GetFirst(), 999);
    EXPECT_EQ(copy.GetLength(), 4);
}

TEST(LinkedListConstructors, AssignmentOperator_MakesDeepCopy) {
    int items1[] = {1, 2, 3};
    int items2[] = {9, 9};

    LinkedList<int> a(items1, 3);
    LinkedList<int> b(items2, 2);

    b = a;
    b.Prepend(777);

    EXPECT_EQ(a.GetLength(), 3);
    EXPECT_EQ(a.GetFirst(), 1);
    EXPECT_EQ(b.GetFirst(), 777);
}

TEST(LinkedListConstructors, SelfAssignment_NoCorruption) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    list = list;

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.Get(0), 1);
}

// ==================== GetFirst / GetLast / Get ====================

TEST(LinkedListAccess, GetFirst_ReturnsFirstElement) {
    int items[] = {5, 10, 15};
    LinkedList<int> list(items, 3);

    EXPECT_EQ(list.GetFirst(), 5);
}

TEST(LinkedListAccess, GetLast_ReturnsLastElement) {
    int items[] = {5, 10, 15};
    LinkedList<int> list(items, 3);

    EXPECT_EQ(list.GetLast(), 15);
}

TEST(LinkedListAccess, Get_MiddleElement) {
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);

    EXPECT_EQ(list.Get(2), 3);
}

// ==================== Append / Prepend / InsertAt ====================

TEST(LinkedListOperations, Append_AddsToEnd) {
    LinkedList<int> list;
    list.Append(1);
    list.Append(2);
    list.Append(3);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetLast(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
}

TEST(LinkedListOperations, Prepend_AddsToBeginning) {
    LinkedList<int> list;
    list.Prepend(3);
    list.Prepend(2);
    list.Prepend(1);

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(LinkedListOperations, InsertAt_Middle_ShiftsElements) {
    int items[] = {1, 2, 4, 5};
    LinkedList<int> list(items, 4);

    list.InsertAt(3, 2);  // вставляем 3 на позицию 2

    EXPECT_EQ(list.GetLength(), 5);
    EXPECT_EQ(list.Get(1), 2);
    EXPECT_EQ(list.Get(2), 3);
    EXPECT_EQ(list.Get(3), 4);
}

TEST(LinkedListOperations, InsertAt_Zero_SameAsPrepend) {
    int items[] = {2, 3};
    LinkedList<int> list(items, 2);

    list.InsertAt(1, 0);

    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLength(), 3);
}

TEST(LinkedListOperations, InsertAt_End_SameAsAppend) {
    int items[] = {1, 2};
    LinkedList<int> list(items, 2);

    list.InsertAt(3, 2);

    EXPECT_EQ(list.GetLast(), 3);
    EXPECT_EQ(list.GetLength(), 3);
}

// ==================== GetSubList ====================

TEST(LinkedListSubList, GetSubList_ReturnsCorrectRange) {
    int items[] = {1, 2, 3, 4, 5};
    LinkedList<int> list(items, 5);

    LinkedList<int>* sub = list.GetSubList(1, 3);

    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Get(0), 2);
    EXPECT_EQ(sub->Get(1), 3);
    EXPECT_EQ(sub->Get(2), 4);

    delete sub;
}

TEST(LinkedListSubList, GetSubList_SingleElement) {
    int items[] = {10, 20, 30};
    LinkedList<int> list(items, 3);

    LinkedList<int>* sub = list.GetSubList(1, 1);

    EXPECT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Get(0), 20);

    delete sub;
}

// ==================== Concat ====================

TEST(LinkedListConcat, Concat_ReturnsCombinedList) {
    int items1[] = {1, 2, 3};
    int items2[] = {4, 5, 6};
    LinkedList<int> a(items1, 3);
    LinkedList<int> b(items2, 3);

    LinkedList<int>* result = a.Concat(&b);

    EXPECT_EQ(result->GetLength(), 6);
    EXPECT_EQ(result->Get(0), 1);
    EXPECT_EQ(result->Get(3), 4);
    EXPECT_EQ(result->Get(5), 6);

    delete result;
}

TEST(LinkedListConcat, Concat_DoesNotModifyOriginals) {
    int items1[] = {1, 2};
    int items2[] = {3, 4};
    LinkedList<int> a(items1, 2);
    LinkedList<int> b(items2, 2);

    LinkedList<int>* result = a.Concat(&b);

    EXPECT_EQ(a.GetLength(), 2);
    EXPECT_EQ(b.GetLength(), 2);

    delete result;
}

// ==================== Исключения ====================

TEST(LinkedListExceptions, GetFirst_EmptyList_ThrowsIndexOutOfRange) {
    LinkedList<int> list;

    EXPECT_THROW(list.GetFirst(), IndexOutOfRange);
}

TEST(LinkedListExceptions, GetLast_EmptyList_ThrowsIndexOutOfRange) {
    LinkedList<int> list;

    EXPECT_THROW(list.GetLast(), IndexOutOfRange);
}

TEST(LinkedListExceptions, Get_NegativeIndex_ThrowsIndexOutOfRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.Get(-1), IndexOutOfRange);
}

TEST(LinkedListExceptions, Get_IndexEqualLength_ThrowsIndexOutOfRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.Get(3), IndexOutOfRange);
}

TEST(LinkedListExceptions, InsertAt_NegativeIndex_ThrowsIndexOutOfRange) {
    LinkedList<int> list;

    EXPECT_THROW(list.InsertAt(1, -1), IndexOutOfRange);
}

TEST(LinkedListExceptions, InsertAt_TooLargeIndex_ThrowsIndexOutOfRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.InsertAt(99, 10), IndexOutOfRange);
}

TEST(LinkedListExceptions, GetSubList_StartOutOfRange_ThrowsIndexOutOfRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(-1, 2), IndexOutOfRange);
}

TEST(LinkedListExceptions, GetSubList_EndOutOfRange_ThrowsIndexOutOfRange) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    EXPECT_THROW(list.GetSubList(0, 5), IndexOutOfRange);
}

TEST(LinkedListExceptions, Constructor_NegativeCount_ThrowsInvalidArgument) {
    int items[] = {1, 2};

    EXPECT_THROW(LinkedList<int> list(items, -1), std::invalid_argument);
}

TEST(LinkedListExceptions, Concat_NullptrList_ThrowsInvalidArgument) {
    LinkedList<int> list;

    EXPECT_THROW(list.Concat(nullptr), std::invalid_argument);
}

// ==================== Шаблон с другим типом ====================

TEST(LinkedListTemplate, WorksWithDouble) {
    double items[] = {1.1, 2.2, 3.3};
    LinkedList<double> list(items, 3);

    EXPECT_DOUBLE_EQ(list.GetFirst(), 1.1);
    EXPECT_DOUBLE_EQ(list.GetLast(), 3.3);
}