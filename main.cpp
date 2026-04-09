#include <iostream>
#include <string>
#include <limits>
#include <random>   // std::mt19937, std::uniform_int_distribution для генерации случайных чисел
#include <chrono>   // performance-тесты
#include <iomanip>  // std::fixed, std::setprecision

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "BitSequence.h"
#include "Sequence_utils.h"  // Split, Slice, Range, GetPrefixes, GetPostfixes, SlidingAverage, Unzip

// Утилиты

void ClearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Реализован именно такой вариант ReadInt(), поскольку на семинаре Михаил Владиславович
int ReadInt( const std::string& prompt ) {
    while ( true ) {
        std::cout << prompt;
        std::string line;
        std::getline( std::cin, line );

        // Пропускаем пустые строки
        if ( line.empty() ) {
            continue;
        }

        try {
            size_t pos;
            int value = std::stoi( line, &pos );
            // Убеждаемся, что вся строка была числом (без лишних символов после числа)
            if ( pos != line.length() ) {
                throw std::invalid_argument( "trailing characters" );
            }
            return value;
        } catch ( const std::exception& ) {
            std::cout << " [!] Введите целое число.\n";
            // Очистка потока не требуется, так как мы уже прочитали строку
        }
    }
}
void PrintSequence( Sequence<int> *seq ) {
    std::cout << " Последовательность [" << seq->GetLength() << "]: [ ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << " ]\n";
}

void PrintDoubleSequence( Sequence<double> *seq ) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << " Результат [" << seq->GetLength() << "]: [ ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i);
        if (i < seq->GetLength() - 1) std::cout << ", ";
    }
    std::cout << " ]\n";
}

void PrintNestedSequence( Sequence<Sequence<int> *> *nested ) {
    std::cout << " Результат [" << nested->GetLength() << " сегментов]:\n";
    for (int i = 0; i < nested->GetLength(); ++i) {
        std::cout << "   [" << i << "]: ";
        PrintSequence(nested->Get(i));
    }
}

// Меню бонусных операций

void BonusMenu( Sequence<int> *seq ) {
    while (true) {
        std::cout << "\n";
        PrintSequence(seq);
        std::cout << "\n";
        std::cout << " |-- Бонусные операции --------------------------------------------|\n";
        std::cout << " |  1.  FlatMap        — каждый элемент → подпоследовательность    |\n";
        std::cout << " |  2.  Zip            — объединить с другой в список пар          |\n";
        std::cout << " |  3.  Split          — разбить по условию (0 = разделитель)      |\n";
        std::cout << " |  4.  Slice          — удалить/заменить фрагмент [i, i+n)        |\n";
        std::cout << " |  5.  Range          — сгенерировать [from, to) с шагом step     |\n";
        std::cout << " |  6.  Skip / Take    — пропустить/взять первые N элементов       |\n";
        std::cout << " |  7.  GetPrefixes    — все префиксы последовательности           |\n";
        std::cout << " |  8.  GetPostfixes   — все суффиксы (постфиксы)                  |\n";
        std::cout << " |  9.  SlidingAverage — скользящее среднее (окно 3: prev,x,next)  |\n";
        std::cout << " |  10. ZipWithReversed— попарно сложить с перевёрнутой копией     |\n";
        std::cout << " |  11. Min / Max / Avg— минимум, максимум, среднее за один проход |\n";
        std::cout << " |  0.  Назад                                                      |\n";
        std::cout << " |-----------------------------------------------------------------|\n";
        std::cout << "\n";

        int choice = ReadInt(" Выбор: ");

        try {
            if (choice == 1) {
                // FlatMap: каждый элемент x → [x, -x]
                std::cout << " FlatMap: каждый x → [x, -x]\n";
                Sequence<int> *result = seq->FlatMap<int>([]( const int &x ) -> Sequence<int> * {
                    int pair[] = {x, -x};
                    return new MutableArraySequence<int>(pair, 2);
                });
                std::cout << " Результат: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 2) {
                // Zip
                int n = ReadInt(" Сколько элементов во второй последовательности? ");
                MutableArraySequence<int> other;
                for (int i = 0; i < n; ++i) {
                    int val = ReadInt(" Элемент " + std::to_string(i) + ": ");
                    other.Append(val);
                }
                auto *zipped = seq->Zip<int>(&other);
                std::cout << " Результат Zip [" << zipped->GetLength() << " пар]:\n";
                for (int i = 0; i < zipped->GetLength(); ++i) {
                    std::cout << "   (" << zipped->Get(i).first
                            << ", " << zipped->Get(i).second << ")\n";
                }
                delete zipped;
            } else if (choice == 3) {
                // Split по нулю
                std::cout << " Split: разделитель = 0 (все нули убираются, остальное — сегменты)\n";
                auto *parts = Split<int>(seq, []( const int &x ) { return x == 0; });
                PrintNestedSequence(parts);
                for (int i = 0; i < parts->GetLength(); ++i)
                    delete parts->Get(i);
                delete parts;
            } else if (choice == 4) {
                // Slice
                std::cout << " Slice: удалить n элементов начиная с i, вставить replacement\n";
                int i = ReadInt(" Начальный индекс i: ");
                int n = ReadInt(" Количество удаляемых элементов n: ");
                int r = ReadInt(" Сколько элементов вставить (0 = просто удалить)? ");
                MutableArraySequence<int> replacement;
                for (int k = 0; k < r; ++k) {
                    int val = ReadInt(" Элемент замены " + std::to_string(k) + ": ");
                    replacement.Append(val);
                }
                Sequence<int> *rep = (r > 0) ? &replacement : nullptr;
                Sequence<int> *result = Slice<int>(seq, i, n, rep);
                std::cout << " Результат: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 5) {
                // Range
                int from = ReadInt(" from: ");
                int to = ReadInt(" to (не включается): ");
                int step = ReadInt(" step (≠0, может быть отрицательным): ");
                Sequence<int> *result = Range<int>(from, to, step);
                std::cout << " Результат Range(" << from << ", " << to << ", " << step << "): ";
                PrintSequence(result);
                delete result;
            } else if (choice == 6) {
                // Skip / Take
                std::cout << " 1 - Skip(n)   пропустить первые n элементов\n";
                std::cout << " 2 - Take(n)   взять только первые n элементов\n";
                int op = ReadInt(" Операция: ");
                int n = ReadInt(" n: ");
                Sequence<int> *result = nullptr;
                if (op == 1) result = seq->Skip(n);
                else result = seq->Take(n);
                std::cout << " Результат: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 7) {
                // GetPrefixes
                auto *prefixes = GetPrefixes<int>(seq);
                std::cout << " Все префиксы (" << prefixes->GetLength() << " шт.):\n";
                for (int i = 0; i < prefixes->GetLength(); ++i) {
                    std::cout << "   prefix[" << i << "]: ";
                    PrintSequence(prefixes->Get(i));
                }
                for (int i = 0; i < prefixes->GetLength(); ++i)
                    delete prefixes->Get(i);
                delete prefixes;
            } else if (choice == 8) {
                // GetPostfixes
                auto *postfixes = GetPostfixes<int>(seq);
                std::cout << " Все суффиксы (" << postfixes->GetLength() << " шт.):\n";
                for (int i = 0; i < postfixes->GetLength(); ++i) {
                    std::cout << "   suffix[" << i << "]: ";
                    PrintSequence(postfixes->Get(i));
                }
                for (int i = 0; i < postfixes->GetLength(); ++i)
                    delete postfixes->Get(i);
                delete postfixes;
            } else if (choice == 9) {
                // SlidingAverage
                std::cout << " SlidingAverage: a[i] = (a[i-1] + a[i] + a[i+1]) / 3\n";
                std::cout << " (для крайних: среднее двух)\n";
                Sequence<double> *result = SlidingAverage<int>(seq);
                PrintDoubleSequence(result);
                delete result;
            } else if (choice == 10) {
                // ZipWithReversed
                std::cout << " ZipWithReversed: result[i] = seq[i] + seq[n-i-1]\n";
                Sequence<int> *result = seq->ZipWithReversed();
                std::cout << " Результат: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 11) {
                // Min / Max / Avg
                auto r = seq->GetMinMaxAvg();
                std::cout << std::fixed << std::setprecision(4);
                std::cout << " Min = " << r.min << "\n";
                std::cout << " Max = " << r.max << "\n";
                std::cout << " Avg = " << (r.sum / r.count) << "\n";
            } else if (choice == 0) {
                break;
            } else {
                std::cout << " [!] Неизвестная команда.\n";
            }
        } catch (const std::exception &e) {
            std::cout << " [!] Ошибка: " << e.what() << "\n";
        }
    }
}

// Меню Performance-тестов
void PerformanceMenu() {
    std::cout << "\n";
    std::cout << "===============================================\n";
    std::cout << "         Тесты производительности\n";
    std::cout << "===============================================\n";
    std::cout << "\nВыберите размер N (рекомендуется 100000 - 500000):\n";
    int n = ReadInt("N = ");
    if (n <= 0) {
        std::cout << "[!] N должно быть > 0\n";
        return;
    }

    // Вспомогательная лямбда для замера времени
    auto measure = [&](const std::string& name, auto&& func) {
        // [&] - захват всех переменных по ссылке,
        // а auto&& fn универсальная ссылка (позволяет лямбде принимать любой вызываемый объект)
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        double ms = std::chrono::duration<double, std::milli>(end - start).count();
        std::cout << std::fixed << std::setprecision(2) << "   " << name << ": " << ms << " мс\n";
    };

    // 1. Append
    std::cout << "\n-- 1. Append (добавление N элементов) -----------------\n";
    measure("ArraySequence Append", [&]() {
        MutableArraySequence<int> seq;
        for (int i = 0; i < n; ++i) seq.Append(i);
    });
    measure("ListSequence  Append", [&]() {
        MutableListSequence<int> seq;
        for (int i = 0; i < n; ++i) seq.Append(i);
    });

    // 2. Get (случайный доступ)
    std::cout << "\n-- 2. Get (случайный доступ, N обращений) ------------\n";
    {
        MutableArraySequence<int> arr_seq;
        MutableListSequence<int> lst_seq;
        for (int i = 0; i < n; ++i) {
            arr_seq.Append(i);
            lst_seq.Append(i);
        }
        // Генерируем случайные индексы
        std::mt19937 rng(42);
        std::uniform_int_distribution<int> dist(0, n - 1);
        std::vector<int> indices(n);
        for (int i = 0; i < n; ++i) indices[i] = dist(rng);

        measure("ArraySequence Get", [&]() {
            volatile int x = 0;
            for (int idx : indices) x = arr_seq.Get(idx);
            (void)x;
        });
        measure("ListSequence  Get", [&]() {
            int limit = std::min(n, 1000);
            volatile int x = 0;
            for (int i = 0; i < limit; ++i) x = lst_seq.Get(indices[i]);
            std::cout << "   (ListSequence Get — O(n), замерено только 1000 обращений)\n";
            (void)x;
        });
    }

    // 3. Map
    std::cout << "\n-- 3. Map (применить функцию к каждому элементу) -----\n";
    {
        MutableArraySequence<int> arr_seq;
        MutableListSequence<int> lst_seq;
        for (int i = 0; i < n; ++i) {
            arr_seq.Append(i);
            lst_seq.Append(i);
        }
        measure("ArraySequence Map", [&]() {
            Sequence<int>* res = arr_seq.Map([](int x) { return x * 2; });
            delete res;
        });
        measure("ListSequence  Map", [&]() {
            Sequence<int>* res = lst_seq.Map([](int x) { return x * 2; });
            delete res;
        });
    }

    // 4. Reduce (сумма)
    std::cout << "\n-- 4. Reduce (свёртка — сумма всех элементов) -------\n";
    {
        MutableArraySequence<int> arr_seq;
        MutableListSequence<int> lst_seq;
        for (int i = 0; i < n; ++i) {
            arr_seq.Append(i);
            lst_seq.Append(i);
        }
        measure("ArraySequence Reduce", [&]() {
            volatile long long sum = arr_seq.Reduce<long long>(
                [](long long acc, int x) { return acc + x; }, 0LL);
            (void)sum;
        });
        measure("ListSequence  Reduce", [&]() {
            volatile long long sum = lst_seq.Reduce<long long>(
                [](long long acc, int x) { return acc + x; }, 0LL);
            (void)sum;
        });
    }

    // 5. Immutable Append (цепочка операций)
    std::cout << "\n-- 5. Immutable Append (каждый Append создаёт новый объект) --\n";
    {
        int limit = std::min(n, 5000);
        measure("ImmutableArraySeq Append (N=" + std::to_string(limit) + ")", [&]() {
            Sequence<int>* seq = new ImmutableArraySequence<int>();
            for (int i = 0; i < limit; ++i) {
                Sequence<int>* next = seq->Append(i);
                delete seq;
                seq = next;
            }
            delete seq;
        });
    }
    std::cout << "\n";
}

// Меню операций над Sequence<int>

void SequenceMenu( Sequence<int> *seq, bool isMutable ) {
    while (true) {
        std::cout << "\n";
        PrintSequence(seq);
        std::cout << " Тип: " << (isMutable ? "Mutable" : "Immutable") << "\n";
        std::cout << "\n";
        std::cout << " |-- Основные операции ------------------------------------------|\n";
        std::cout << " |  1.  Append         - добавить элемент в конец                |\n";
        std::cout << " |  2.  Prepend        - добавить элемент в начало               |\n";
        std::cout << " |  3.  InsertAt       - вставить по индексу                     |\n";
        std::cout << " |  4.  Get            - получить элемент по индексу             |\n";
        std::cout << " |  5.  GetSubsequence - подпоследовательность [start, end]      |\n";
        std::cout << " |  6.  Concat         - склеить с другой последовательностью    |\n";
        std::cout << " |  7.  Map            - применить функцию к каждому элементу    |\n";
        std::cout << " |  8.  Where          - оставить элементы по условию            |\n";
        std::cout << " |  9. Reduce         - свёртка (сумма всех элементов)           |\n";
        std::cout << " |  10. Бонусные операции ->                                     |\n";
        std::cout << " |  0.  Назад                                                    |\n";
        std::cout << " |---------------------------------------------------------------|\n";
        std::cout << "\n";

        int choice = ReadInt(" Выбор: ");

        try {
            if (choice == 1) {
                int val = ReadInt(" Значение: ");
                Sequence<int> *next = seq->Append(val);
                if (next != seq) {
                    delete seq;
                    seq = next;
                }
                std::cout << " Готово.\n";
            } else if (choice == 2) {
                int val = ReadInt(" Значение: ");
                Sequence<int> *next = seq->Prepend(val);
                if (next != seq) {
                    delete seq;
                    seq = next;
                }
                std::cout << " Готово.\n";
            } else if (choice == 3) {
                int val = ReadInt(" Значение: ");
                int index = ReadInt(" Индекс: ");
                Sequence<int> *next = seq->InsertAt(val, index);
                if (next != seq) {
                    delete seq;
                    seq = next;
                }
                std::cout << " Готово.\n";
            } else if (choice == 4) {
                int index = ReadInt(" Индекс: ");
                std::cout << " Get(" << index << ") = " << seq->Get(index) << "\n";
            } else if (choice == 5) {
                int start = ReadInt(" Начало: ");
                int end = ReadInt(" Конец: ");
                Sequence<int> *sub = seq->GetSubsequence(start, end);
                std::cout << " Результат: ";
                PrintSequence(sub);
                delete sub;
            } else if (choice == 6) {
                int n = ReadInt(" Сколько элементов в новой последовательности? ");
                MutableArraySequence<int> other;
                for (int i = 0; i < n; ++i) {
                    int val = ReadInt(" Элемент " + std::to_string(i) + ": ");
                    other.Append(val);
                }
                Sequence<int> *result = seq->Concat(&other);
                std::cout << " Результат: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 7) {
                std::cout << " 1 - умножить на константу\n";
                std::cout << " 2 - прибавить константу\n";
                int op = ReadInt(" Операция: ");
                int val = ReadInt(" Константа: ");
                Sequence<int> *result = nullptr;
                if (op == 1) result = seq->Map([val]( const int &x ) { return x * val; });
                else result = seq->Map([val]( const int &x ) { return x + val; });
                std::cout << " Результат Map: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 8) {
                int threshold = ReadInt(" Оставить элементы > порога. Порог: ");
                Sequence<int> *result = seq->Where([threshold]( const int &x ) {
                    return x > threshold;
                });
                std::cout << " Результат Where: ";
                PrintSequence(result);
                delete result;
            } else if (choice == 9) {
                int sum = seq->Reduce<int>([]( int acc, const int &x ) {
                    return acc + x;
                }, 0);
                std::cout << " Сумма всех элементов = " << sum << "\n";
            } else if (choice == 10) {
                BonusMenu(seq);
            } else if (choice == 0) {
                break;
            } else {
                std::cout << " [!] Неизвестная команда.\n";
            }
        } catch (const std::exception &e) {
            std::cout << " [!] Ошибка: " << e.what() << "\n";
        }
    }

    delete seq;
}

// Меню BitSequence

void PrintBitSequence( BitSequence *seq ) {
    std::cout << " BitSequence [" << seq->GetLength() << "]: ";
    for (int i = 0; i < seq->GetLength(); ++i) {
        std::cout << seq->Get(i).GetValue();
    }
    std::cout << "\n";
}

void BitSequenceMenu() {
    std::cout << "\n Введите биты (строка из 0 и 1, например 10110): ";
    std::string bits;
    std::cin >> bits;
    // Очищаем буфер после cin, чтобы getline в ReadInt работал корректно
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    BitSequence *seq = nullptr;
    try {
        seq = new BitSequence(bits);
    } catch (const std::exception &e) {
        std::cout << " [!] Ошибка: " << e.what() << "\n";
        return;
    }

    while (true) {
        std::cout << "\n";
        PrintBitSequence(seq);
        std::cout << "\n";
        std::cout << " |-- Битовые операции -------------------------------------------|\n";
        std::cout << " |  1. AND   — побитовое И с другой последовательностью          |\n";
        std::cout << " |  2. OR    — побитовое ИЛИ с другой последовательностью        |\n";
        std::cout << " |  3. XOR   — побитовое исключающее ИЛИ с другой                |\n";
        std::cout << " |  4. NOT   — побитовое НЕ (инверсия текущей)                   |\n";
        std::cout << " |  0. Назад                                                     |\n";
        std::cout << " |---------------------------------------------------------------|\n";
        std::cout << "\n";

        int choice = ReadInt(" Выбор: ");

        try {
            if (choice >= 1 && choice <= 3) {
                std::cout << " Введите вторую последовательность (можно другой длины, будет дополнена нулями слева): ";
                std::string bits2;
                std::cin >> bits2;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                BitSequence other(bits2);

                BitSequence result;
                if (choice == 1)
                    result = seq->BitwiseAND(other);
                else if (choice == 2)
                    result = seq->BitwiseOR(other);
                else
                    result = seq->BitwiseXOR(other);

                std::cout << " Результат: ";
                PrintBitSequence(&result);
            } else if (choice == 4) {
                BitSequence result = seq->BitwiseNOT();
                std::cout << " Результат NOT: ";
                PrintBitSequence(&result);
            } else if (choice == 0) {
                break;
            } else {
                std::cout << " [!] Неизвестная команда.\n";
            }
        } catch (const std::exception &e) {
            std::cout << " [!] Ошибка: " << e.what() << "\n";
        }
    }

    delete seq;
}

// Генератор случайных чисел (Mersenne Twister)

static std::mt19937 gRng{std::random_device{}()};

static int RandomInt( int lo, int hi ) {
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(gRng);
}

static void FillRandom( int *data, int n, int lo, int hi ) {
    for (int i = 0; i < n; ++i)
        data[i] = RandomInt(lo, hi);
}

// Создание начальной последовательности

Sequence<int> *CreateSequence( int seqType, bool isMutable ) {
    std::cout << "\n";
    std::cout << " Как заполнить последовательность?\n";
    std::cout << " 1. Ввести вручную\n";
    std::cout << " 2. Заполнить случайными числами\n";
    int fillMode = ReadInt(" Способ: ");

    int n = ReadInt(" Сколько начальных элементов? ");

    int *data = (n > 0) ? new int[n] : nullptr;

    if (fillMode == 2 && n > 0) {
        int lo = ReadInt(" Минимальное значение: ");
        int hi = ReadInt(" Максимальное значение: ");
        if (lo > hi) std::swap(lo, hi);
        FillRandom(data, n, lo, hi);

        std::cout << " Сгенерировано: [ ";
        for (int i = 0; i < n; ++i) {
            std::cout << data[i];
            if (i < n - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    } else if (n > 0) {
        for (int i = 0; i < n; ++i)
            data[i] = ReadInt(" Элемент " + std::to_string(i) + ": ");
    }

    Sequence<int> *seq = nullptr;

    if (n == 0) {
        if (seqType == 1)
            seq = isMutable
                      ? static_cast<Sequence<int> *>(new MutableArraySequence<int>())
                      : static_cast<Sequence<int> *>(new ImmutableArraySequence<int>());
        else
            seq = isMutable
                      ? static_cast<Sequence<int> *>(new MutableListSequence<int>())
                      : static_cast<Sequence<int> *>(new ImmutableListSequence<int>());
    } else {
        if (seqType == 1)
            seq = isMutable
                      ? static_cast<Sequence<int> *>(new MutableArraySequence<int>(data, n))
                      : static_cast<Sequence<int> *>(new ImmutableArraySequence<int>(data, n));
        else
            seq = isMutable
                      ? static_cast<Sequence<int> *>(new MutableListSequence<int>(data, n))
                      : static_cast<Sequence<int> *>(new ImmutableListSequence<int>(data, n));
    }

    delete[] data;
    return seq;
}

// Главное меню

int main() {
    while (true) {
        std::cout << "\n";
        std::cout << " |===========================================|\n";
        std::cout << " |           Лабораторная работа №2          |\n";
        std::cout << " |===========================================|\n";
        std::cout << " |  1. ArraySequence<int>                    |\n";
        std::cout << " |  2. ListSequence<int>                     |\n";
        std::cout << " |  3. BitSequence                           |\n";
        std::cout << " |  4. Тесты производительности              |\n";
        std::cout << " |  0. Выход                                 |\n";
        std::cout << " |===========================================|\n";
        std::cout << "\n";

        int choice = ReadInt(" Выбор: ");

        if (choice == 0) {
            std::cout << "\n До свидания!\n";
            break;
        } else if (choice == 1 || choice == 2) {
            std::cout << "\n";
            std::cout << " 1. Mutable   (операции изменяют текущий объект)\n";
            std::cout << " 2. Immutable (каждая операция возвращает новый объект)\n";
            int mutChoice = ReadInt(" Тип: ");
            bool isMutable = (mutChoice == 1);

            Sequence<int> *seq = CreateSequence(choice, isMutable);
            SequenceMenu(seq, isMutable);
        } else if (choice == 3) {
            BitSequenceMenu();
        } else if (choice == 4) {
            PerformanceMenu();
        } else {
            std::cout << " [!] Неизвестная команда.\n";
        }
    }

    return 0;
}
