#pragma once

#include <string>
#include "Sequence.h"
#include "DynamicArray.h"
#include "SequenceIterator.h"
#include "Bit.h"

class BitSequence : public Sequence<Bit> {
private:
    // Внутреннее хранилище: упакованные биты в байтах
    DynamicArray<uint8_t> bytes_;
    int bit_count_;

    // Вспомогательный mutable-объект для возврата по ссылке (компромисс константности)
    mutable Bit temp_bit_;

    // Преобразование битового индекса в байт и маску
    static int ByteIndex(int bit_index) { return bit_index / 8; }
    static int BitOffset(int bit_index) { return bit_index % 8; }
    static uint8_t BitMask(int bit_offset) { return 1 << bit_offset; }

    // Убедиться, что хранилище имеет достаточный размер байт
    void EnsureCapacity(int bits_needed) {
        int bytes_needed = (bits_needed + 7) / 8;
        if (bytes_needed > static_cast<int>(bytes_.GetCount())) {
            bytes_.Resize(bytes_needed);
        }
    }

    // Прочитать бит по индексу (без проверки границ)
    bool GetBitValue(int index) const {
        uint8_t byte = bytes_.Get(ByteIndex(index));
        return (byte >> BitOffset(index)) & 1;
    }

    // Записать бит по индексу (без проверки границ)
    void SetBitValue(int index, bool value) {
        int byte_idx = ByteIndex(index);
        int offset = BitOffset(index);
        uint8_t& byte = bytes_[byte_idx];
        if (value)
            byte |= BitMask(offset);
        else
            byte &= ~BitMask(offset);
    }

    // Выравнивание длин (добавление ведущих нулей) для побитовых операций
    static BitSequence AlignLeft(const BitSequence& a, const BitSequence& b) {
        int max_len = std::max(a.GetLength(), b.GetLength());
        BitSequence result;
        result.Resize(max_len);
        // Копируем a (выравнивая по правому краю? В левое выравнивание — ведущие нули слева)
        // Поскольку мы храним биты как последовательность от старшего к младшему?
        // В классическом представлении индекс 0 — это младший бит или старший?
        // Для простоты будем считать, что индекс 0 — это самый левый бит (старший).
        // Тогда при левом выравнивании мы добавляем нули в начало.
        int diff = max_len - a.GetLength();
        for (int i = 0; i < a.GetLength(); ++i)
            result.SetBitValue(diff + i, a.GetBitValue(i));
        diff = max_len - b.GetLength();
        for (int i = 0; i < b.GetLength(); ++i)
            result.SetBitValue(diff + i, b.GetBitValue(i));
        return result;
    }

public:
    // ==================== Конструкторы ====================

    BitSequence() : bytes_(0), bit_count_(0) {
    }

    BitSequence(int* values, int count) : bytes_(0), bit_count_(count) {
        if (count < 0) throw std::invalid_argument("BitSequence: count can't be negative");
        if (count > 0 && values == nullptr) throw std::invalid_argument("BitSequence: values is nullptr");
        int bytes_needed = (count + 7) / 8;
        bytes_.Resize(bytes_needed);
        for (int i = 0; i < count; ++i) {
            if (values[i] != 0 && values[i] != 1)
                throw std::invalid_argument("BitSequence: value must be 0 or 1");
            SetBitValue(i, values[i] != 0);
        }
    }

    BitSequence(Bit* bits, int count) : bytes_(0), bit_count_(count) {
        if (count < 0) throw std::invalid_argument("BitSequence: count can't be negative");
        if (count > 0 && bits == nullptr) throw std::invalid_argument("BitSequence: bits is nullptr");
        int bytes_needed = (count + 7) / 8;
        bytes_.Resize(bytes_needed);
        for (int i = 0; i < count; ++i)
            SetBitValue(i, bits[i].GetValue());
    }

    BitSequence(const BitSequence& other) : bytes_(other.bytes_), bit_count_(other.bit_count_) {}

    BitSequence(const std::string& bits) : bytes_(0), bit_count_(bits.length()) {
        int bytes_needed = (bit_count_ + 7) / 8;
        bytes_.Resize(bytes_needed);
        for (size_t i = 0; i < bits.size(); ++i) {
            if (bits[i] != '0' && bits[i] != '1')
                throw std::invalid_argument("BitSequence: string must contain only '0' or '1'");
            SetBitValue(static_cast<int>(i), bits[i] == '1');
        }
    }

    // ==================== Методы Sequence<Bit> ====================

    // ICollection
    Bit& Get(std::size_t index) override {
        if (index >= static_cast<std::size_t>(bit_count_))
            throw IndexOutOfRange("BitSequence::Get: index out of range");
        // mutable-компромисс: сохраняем вычисленный бит в temp_bit_
        temp_bit_ = Bit(GetBitValue(static_cast<int>(index)));
        return temp_bit_;
    }

    const Bit& Get(std::size_t index) const override {
        if (index >= static_cast<std::size_t>(bit_count_))
            throw IndexOutOfRange("BitSequence::Get: index out of range");
        temp_bit_ = Bit(GetBitValue(static_cast<int>(index)));
        return temp_bit_;
    }

    std::size_t GetCount() const override {
        return static_cast<std::size_t>(bit_count_);
    }

    // Декомпозиция
    Bit& GetFirst() const override {
        if (bit_count_ == 0)
            throw IndexOutOfRange("BitSequence: empty sequence");
        temp_bit_ = Bit(GetBitValue(0));
        return temp_bit_;
    }

    Bit& GetLast() const override {
        if (bit_count_ == 0)
            throw IndexOutOfRange("BitSequence: empty sequence");
        temp_bit_ = Bit(GetBitValue(bit_count_ - 1));
        return temp_bit_;
    }

    int GetLength() const override {
        return bit_count_;
    }

    Sequence<Bit>* GetSubsequence(int start, int end) const override {
        if (start < 0 || start >= bit_count_)
            throw IndexOutOfRange("BitSequence: start out of range");
        if (end < 0 || end >= bit_count_)
            throw IndexOutOfRange("BitSequence: end out of range");
        if (start > end)
            throw std::invalid_argument("BitSequence: start > end");

        BitSequence* result = new BitSequence();
        result->Resize(end - start + 1);
        for (int i = start; i <= end; ++i) {
            result->SetBitValue(i - start, GetBitValue(i));
        }
        return result;
    }

    // Операции (возвращают новые объекты, так как неизменяемые)
    Sequence<Bit>* Append(const Bit& item) override {
        BitSequence* copy = new BitSequence(*this);
        copy->InsertAt(item, copy->bit_count_);
        return copy;
    }

    Sequence<Bit>* Prepend(const Bit& item) override {
        BitSequence* copy = new BitSequence(*this);
        copy->InsertAt(item, 0);
        return copy;
    }

    Sequence<Bit>* InsertAt(const Bit& item, int index) override {
        if (index < 0 || index > bit_count_)
            throw IndexOutOfRange("BitSequence: index out of range");

        BitSequence* copy = new BitSequence(*this);
        copy->Resize(bit_count_ + 1);
        // Сдвигаем биты вправо, начиная с конца
        for (int i = bit_count_ - 1; i >= index; --i) {
            copy->SetBitValue(i + 1, GetBitValue(i));
        }
        copy->SetBitValue(index, item.GetValue());
        return copy;
    }

    Sequence<Bit>* Concat(Sequence<Bit>* other) const override {
        if (other == nullptr)
            throw std::invalid_argument("BitSequence::Concat: other is nullptr");

        BitSequence* result = new BitSequence(*this);
        int other_len = other->GetLength();
        result->Resize(bit_count_ + other_len);
        for (int i = 0; i < other_len; ++i) {
            result->SetBitValue(bit_count_ + i, other->Get(i).GetValue());
        }
        return result;
    }

    Sequence<Bit>* CreateEmpty() const override {
        return new BitSequence();
    }

    // ==================== Побитовые операции (побайтовая оптимизация) ====================

    BitSequence BitwiseAND(const BitSequence& other) const {
        int max_len = std::max(bit_count_, other.bit_count_);
        BitSequence result;
        result.Resize(max_len);

        // Выравниваем длины мысленно: короткая дополняется нулями слева
        int diff_this = max_len - bit_count_;
        int diff_other = max_len - other.bit_count_;

        // Обрабатываем побайтово, но проще побитово, т.к. выравнивание смещает биты
        // Для максимальной эффективности всё равно придётся проходить по каждому биту,
        // потому что выравнивание нарушает байтовую границу. Однако можно оптимизировать,
        // если выравнивание кратно 8. Для простоты и надёжности оставим побитовый цикл,
        // но с явным указанием, что в реальной реализации стоит сделать побайтово.
        // Ниже — чистая побитовая реализация (простая и понятная).
        for (int i = 0; i < max_len; ++i) {
            bool a_bit = (i < diff_this) ? false : GetBitValue(i - diff_this);
            bool b_bit = (i < diff_other) ? false : other.GetBitValue(i - diff_other);
            result.SetBitValue(i, a_bit && b_bit);
        }
        return result;
    }

    BitSequence BitwiseOR(const BitSequence& other) const {
        int max_len = std::max(bit_count_, other.bit_count_);
        BitSequence result;
        result.Resize(max_len);
        int diff_this = max_len - bit_count_;
        int diff_other = max_len - other.bit_count_;
        for (int i = 0; i < max_len; ++i) {
            bool a_bit = (i < diff_this) ? false : GetBitValue(i - diff_this);
            bool b_bit = (i < diff_other) ? false : other.GetBitValue(i - diff_other);
            result.SetBitValue(i, a_bit || b_bit);
        }
        return result;
    }

    BitSequence BitwiseXOR(const BitSequence& other) const {
        int max_len = std::max(bit_count_, other.bit_count_);
        BitSequence result;
        result.Resize(max_len);
        int diff_this = max_len - bit_count_;
        int diff_other = max_len - other.bit_count_;
        for (int i = 0; i < max_len; ++i) {
            bool a_bit = (i < diff_this) ? false : GetBitValue(i - diff_this);
            bool b_bit = (i < diff_other) ? false : other.GetBitValue(i - diff_other);
            result.SetBitValue(i, a_bit != b_bit);
        }
        return result;
    }

    BitSequence BitwiseNOT() const {
        BitSequence result;
        result.Resize(bit_count_);
        for (int i = 0; i < bit_count_; ++i) {
            result.SetBitValue(i, !GetBitValue(i));
        }
        return result;
    }

    // Операторы
    BitSequence operator&(const BitSequence& other) const { return BitwiseAND(other); }
    BitSequence operator|(const BitSequence& other) const { return BitwiseOR(other);  }
    BitSequence operator^(const BitSequence& other) const { return BitwiseXOR(other); }
    BitSequence operator~() const { return BitwiseNOT(); }

    // ==================== Вспомогательные методы ====================

    void Resize(int new_bit_count) {
        if (new_bit_count < 0) throw std::invalid_argument("BitSequence::Resize: negative size");
        int new_byte_count = (new_bit_count + 7) / 8;
        bytes_.Resize(new_byte_count);
        // Если увеличили, новые биты автоматически равны 0 (благодаря инициализации памяти)
        bit_count_ = new_bit_count;
    }

    // Для совместимости с консольным UI
    void AppendBit(bool value) {
        Resize(bit_count_ + 1);
        SetBitValue(bit_count_ - 1, value);
    }

    // Доступ к сырым байтам (для отладки)
    const DynamicArray<uint8_t>& GetRawBytes() const { return bytes_; }

    // IEnumerator (итератор)
    IEnumerator<Bit>* GetEnumerator() override {
        return new SequenceIterator<Bit>(this);
    }
};