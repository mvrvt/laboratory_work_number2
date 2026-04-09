#include <gtest/gtest.h>
#include "../src/DynamicArray.h"
#include "../src/LinkedList.h"
#include "../src/MutableArraySequence.h"
#include "../src/ImmutableArraySequence.h"
#include "../src/ListSequence.h"

// ==================== DynamicArray::ArrayIterator ====================

TEST(DynamicArrayIterator, IterationOverNonEmpty) {
    int items[] = {10, 20, 30, 40};
    DynamicArray<int> arr(items, 4);

    IEnumerator<int>* it = arr.GetEnumerator();
    ASSERT_NE(it, nullptr);

    int expected[] = {10, 20, 30, 40};
    int idx = 0;
    while (it->MoveNext()) {
        EXPECT_EQ(it->Current(), expected[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 4);

    delete it;
}

TEST(DynamicArrayIterator, MoveNextReturnsFalseAfterEnd) {
    int items[] = {1, 2};
    DynamicArray<int> arr(items, 2);

    IEnumerator<int>* it = arr.GetEnumerator();
    EXPECT_TRUE(it->MoveNext()); // 1
    EXPECT_TRUE(it->MoveNext()); // 2
    EXPECT_FALSE(it->MoveNext()); // end
    EXPECT_FALSE(it->MoveNext()); // still end

    delete it;
}

TEST(DynamicArrayIterator, ResetWorks) {
    int items[] = {5, 6, 7};
    DynamicArray<int> arr(items, 3);

    IEnumerator<int>* it = arr.GetEnumerator();

    // Первый проход
    int sum1 = 0;
    while (it->MoveNext()) sum1 += it->Current();
    EXPECT_EQ(sum1, 5 + 6 + 7);

    // Сброс и повтор
    it->Reset();
    int sum2 = 0;
    while (it->MoveNext()) sum2 += it->Current();
    EXPECT_EQ(sum2, 5 + 6 + 7);

    delete it;
}

TEST(DynamicArrayIterator, EmptyArray) {
    DynamicArray<int> arr(0);
    IEnumerator<int>* it = arr.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(DynamicArrayIterator, CurrentThrowsWhenNotStarted) {
    int items[] = {42};
    DynamicArray<int> arr(items, 1);
    IEnumerator<int>* it = arr.GetEnumerator();

    // до первого MoveNext
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(DynamicArrayIterator, CurrentThrowsAfterEnd) {
    int items[] = {100, 200};
    DynamicArray<int> arr(items, 2);
    IEnumerator<int>* it = arr.GetEnumerator();

    it->MoveNext(); // 100
    it->MoveNext(); // 200
    it->MoveNext(); // false
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(DynamicArrayIterator, MultipleIteratorsAreIndependent) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);

    IEnumerator<int>* it1 = arr.GetEnumerator();
    IEnumerator<int>* it2 = arr.GetEnumerator();

    it1->MoveNext(); // it1 на 1
    it2->MoveNext(); // it2 на 1
    EXPECT_EQ(it1->Current(), 1);
    EXPECT_EQ(it2->Current(), 1);

    it1->MoveNext(); // it1 на 2
    EXPECT_EQ(it1->Current(), 2);
    EXPECT_EQ(it2->Current(), 1); // it2 не изменился

    delete it1;
    delete it2;
}

// ==================== LinkedList::ListIterator ====================

TEST(LinkedListIterator, IterationOverNonEmpty) {
    int items[] = {10, 20, 30, 40};
    LinkedList<int> list(items, 4);

    IEnumerator<int>* it = list.GetEnumerator();
    ASSERT_NE(it, nullptr);

    int expected[] = {10, 20, 30, 40};
    int idx = 0;
    while (it->MoveNext()) {
        EXPECT_EQ(it->Current(), expected[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 4);

    delete it;
}

TEST(LinkedListIterator, ResetWorks) {
    int items[] = {5, 6, 7};
    LinkedList<int> list(items, 3);

    IEnumerator<int>* it = list.GetEnumerator();

    int sum1 = 0;
    while (it->MoveNext()) sum1 += it->Current();
    EXPECT_EQ(sum1, 18);

    it->Reset();
    int sum2 = 0;
    while (it->MoveNext()) sum2 += it->Current();
    EXPECT_EQ(sum2, 18);

    delete it;
}

TEST(LinkedListIterator, EmptyList) {
    LinkedList<int> list;
    IEnumerator<int>* it = list.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(LinkedListIterator, CurrentThrowsWhenNotStarted) {
    int items[] = {99};
    LinkedList<int> list(items, 1);
    IEnumerator<int>* it = list.GetEnumerator();

    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(LinkedListIterator, CurrentThrowsAfterEnd) {
    int items[] = {10, 20};
    LinkedList<int> list(items, 2);
    IEnumerator<int>* it = list.GetEnumerator();

    it->MoveNext();
    it->MoveNext();
    it->MoveNext(); // false
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(LinkedListIterator, MultipleIteratorsAreIndependent) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    IEnumerator<int>* it1 = list.GetEnumerator();
    IEnumerator<int>* it2 = list.GetEnumerator();

    it1->MoveNext(); // it1 -> 1
    it2->MoveNext(); // it2 -> 1
    EXPECT_EQ(it1->Current(), 1);
    EXPECT_EQ(it2->Current(), 1);

    it1->MoveNext(); // it1 -> 2
    EXPECT_EQ(it1->Current(), 2);
    EXPECT_EQ(it2->Current(), 1);

    delete it1;
    delete it2;
}

// ==================== SequenceIterator (через ArraySequence) ====================

TEST(SequenceIterator_ArraySequence, IterationOverNonEmpty) {
    int data[] = {100, 200, 300};
    MutableArraySequence<int> seq(data, 3);

    IEnumerator<int>* it = seq.GetEnumerator();
    ASSERT_NE(it, nullptr);

    int expected[] = {100, 200, 300};
    int idx = 0;
    while (it->MoveNext()) {
        EXPECT_EQ(it->Current(), expected[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 3);

    delete it;
}

TEST(SequenceIterator_ArraySequence, EmptySequence) {
    MutableArraySequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(SequenceIterator_ArraySequence, ResetWorks) {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    IEnumerator<int>* it = seq.GetEnumerator();

    int sum1 = 0;
    while (it->MoveNext()) sum1 += it->Current();
    EXPECT_EQ(sum1, 6);

    it->Reset();
    int sum2 = 0;
    while (it->MoveNext()) sum2 += it->Current();
    EXPECT_EQ(sum2, 6);

    delete it;
}

TEST(SequenceIterator_ArraySequence, CurrentThrowsWhenNotStarted) {
    MutableArraySequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();
    EXPECT_THROW(it->Current(), IndexOutOfRange);
    delete it;
}

TEST(SequenceIterator_ArraySequence, CurrentThrowsAfterEnd) {
    int data[] = {5, 6};
    MutableArraySequence<int> seq(data, 2);
    IEnumerator<int>* it = seq.GetEnumerator();

    it->MoveNext();
    it->MoveNext();
    it->MoveNext(); // end
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(SequenceIterator_ArraySequence, MultipleIteratorsIndependent) {
    int data[] = {10, 20};
    MutableArraySequence<int> seq(data, 2);

    IEnumerator<int>* it1 = seq.GetEnumerator();
    IEnumerator<int>* it2 = seq.GetEnumerator();

    it1->MoveNext(); // it1 -> 10
    it2->MoveNext(); // it2 -> 10
    EXPECT_EQ(it1->Current(), 10);
    EXPECT_EQ(it2->Current(), 10);

    it1->MoveNext(); // it1 -> 20
    EXPECT_EQ(it1->Current(), 20);
    EXPECT_EQ(it2->Current(), 10);

    delete it1;
    delete it2;
}

// ==================== SequenceIterator (через ListSequence) ====================

TEST(SequenceIterator_ListSequence, IterationOverNonEmpty) {
    int data[] = {100, 200, 300};
    MutableListSequence<int> seq(data, 3);

    IEnumerator<int>* it = seq.GetEnumerator();
    ASSERT_NE(it, nullptr);

    int expected[] = {100, 200, 300};
    int idx = 0;
    while (it->MoveNext()) {
        EXPECT_EQ(it->Current(), expected[idx]);
        ++idx;
    }
    EXPECT_EQ(idx, 3);

    delete it;
}

TEST(SequenceIterator_ListSequence, EmptySequence) {
    MutableListSequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();

    EXPECT_FALSE(it->MoveNext());
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(SequenceIterator_ListSequence, ResetWorks) {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);

    IEnumerator<int>* it = seq.GetEnumerator();

    int sum1 = 0;
    while (it->MoveNext()) sum1 += it->Current();
    EXPECT_EQ(sum1, 6);

    it->Reset();
    int sum2 = 0;
    while (it->MoveNext()) sum2 += it->Current();
    EXPECT_EQ(sum2, 6);

    delete it;
}

TEST(SequenceIterator_ListSequence, CurrentThrowsWhenNotStarted) {
    MutableListSequence<int> seq;
    IEnumerator<int>* it = seq.GetEnumerator();
    EXPECT_THROW(it->Current(), IndexOutOfRange);
    delete it;
}

TEST(SequenceIterator_ListSequence, CurrentThrowsAfterEnd) {
    int data[] = {5, 6};
    MutableListSequence<int> seq(data, 2);
    IEnumerator<int>* it = seq.GetEnumerator();

    it->MoveNext();
    it->MoveNext();
    it->MoveNext(); // end
    EXPECT_THROW(it->Current(), IndexOutOfRange);

    delete it;
}

TEST(SequenceIterator_ListSequence, MultipleIteratorsIndependent) {
    int data[] = {10, 20};
    MutableListSequence<int> seq(data, 2);

    IEnumerator<int>* it1 = seq.GetEnumerator();
    IEnumerator<int>* it2 = seq.GetEnumerator();

    it1->MoveNext(); // it1 -> 10
    it2->MoveNext(); // it2 -> 10
    EXPECT_EQ(it1->Current(), 10);
    EXPECT_EQ(it2->Current(), 10);

    it1->MoveNext(); // it1 -> 20
    EXPECT_EQ(it1->Current(), 20);
    EXPECT_EQ(it2->Current(), 10);

    delete it1;
    delete it2;
}

// ==================== Дополнительно: итератор не модифицирует коллекцию ====================

TEST(DynamicArrayIterator, IterationDoesNotModify) {
    int items[] = {1, 2, 3};
    DynamicArray<int> arr(items, 3);

    IEnumerator<int>* it = arr.GetEnumerator();
    while (it->MoveNext()) {
        // просто читаем
        (void)it->Current();
    }
    delete it;

    // коллекция осталась неизменной
    EXPECT_EQ(arr.GetCount(), 3);
    EXPECT_EQ(arr.Get(0), 1);
    EXPECT_EQ(arr.Get(2), 3);
}

TEST(LinkedListIterator, IterationDoesNotModify) {
    int items[] = {1, 2, 3};
    LinkedList<int> list(items, 3);

    IEnumerator<int>* it = list.GetEnumerator();
    while (it->MoveNext()) {
        (void)it->Current();
    }
    delete it;

    EXPECT_EQ(list.GetLength(), 3);
    EXPECT_EQ(list.GetFirst(), 1);
    EXPECT_EQ(list.GetLast(), 3);
}

TEST(SequenceIterator_ArraySequence, IterationDoesNotModify) {
    int data[] = {4, 5, 6};
    MutableArraySequence<int> seq(data, 3);

    IEnumerator<int>* it = seq.GetEnumerator();
    while (it->MoveNext()) {
        (void)it->Current();
    }
    delete it;

    EXPECT_EQ(seq.GetLength(), 3);
    EXPECT_EQ(seq.Get(0), 4);
    EXPECT_EQ(seq.Get(2), 6);
}
