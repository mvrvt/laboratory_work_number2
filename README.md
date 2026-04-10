# Лабораторная работа №2

## Структуры данных, полиморфизм, Map‑Reduce, битовые последовательности

---

### 📋 Общее описание

Проект представляет собой реализацию на **C++20** полиморфных контейнеров с единым интерфейсом `Sequence<T>`.  
В работе реализованы:

- **Динамический массив** (`DynamicArray<T>`)
- **Линейный связанный список** (`LinkedList<T>`)
- **Последовательности** (`ArraySequence<T>`, `ListSequence<T>`) – каждая в двух вариантах:
  - *изменяемая (mutable)* – операции модифицируют текущий объект
  - *неизменяемая (immutable)* – каждая операция возвращает новый объект, исходный остаётся неизменным
- **Битовая последовательность** (`BitSequence`) – специализация `Sequence<Bit>` с эффективной побитовой упаковкой и поддержкой AND, OR, XOR, NOT

Помимо базовых операций (доступ, вставка, удаление, конкатенация), реализован **функциональный стиль** обработки коллекций:

- `Map`, `MapIndexed`, `Where`, `Reduce`, `FlatMap`
- `Zip` / `Unzip`, `Skip` / `Take`, `Split`, `Slice`, `Range`
- `GetPrefixes` / `GetPostfixes`, `SlidingAverage`, `ZipWithReversed`
- Вычисление `min` / `max` / `avg` за один проход

Проект снабжён **консольным UI** для ручного тестирования всех операций, **тестами производительности** (замеры Append, Get, Map, Reduce для разных N) и **модульными тестами** (Google Test), покрывающими основные сценарии, граничные случаи и исключения.

---

### ✅ Выполненные задания и дополнительные баллы

Работа выполнена в соответствии с требованиями из **Таблицы 2** (ЛР-2.pdf).  
Ниже перечислены **базовые** и **дополнительные** пункты, за которые получены баллы.

#### 🔹 Базовые требования (обязательные)
- Реализация `DynamicArray<T>`, `LinkedList<T>`, `Sequence<T>`
- Два варианта `ArraySequence<T>` и `ListSequence<T>`: **mutable** и **immutable**
- `BitSequence` с побитовыми операциями AND, OR, XOR, NOT
- Консольный UI для тестирования всех операций
- Модульные тесты (Google Test)
- Обработка исключений (проверка границ, некорректных параметров)

#### 🔹 Дополнительные пункты (повышение оценки)

| Код       | Описание                                                                 | Баллы |
|-----------|-------------------------------------------------------------------------|-------|
| **М-1**   | Интерфейсы `ICollection<T>`, `IEnumerable<T>`, `IEnumerator<T>`         | 2     |
| **М-2.1** | Расширенные map‑reduce: `Zip`/`Unzip`, `Skip`/`Take`, `Split`, `Slice`, `FlatMap`, генератор `Range` | 2     |
| **М-2.2** | `MapIndexed` и `MapIndexedTo` – преобразование с учётом индекса         | 1     |
| **М-4**   | Перегрузка операторов: `[]` (чтение/запись), побитовые `&`, `\|`, `^`, `~` для `Bit` и `BitSequence` | 2     |
| **П-1**   | Вычисление `min`, `max`, `avg` за один проход (`GetMinMaxAvg`)          | 1     |
| **П-5**   | Получение всех префиксов и постфиксов (`GetPrefixes`, `GetPostfixes`)   | 1     |
| **П-6**   | Скользящее среднее `(aᵢ₋₁ + aᵢ + aᵢ₊₁)/3` (крайние – среднее двух) – `SlidingAverage` | 2     |
| **П-8**   | Поэлементная сумма с «отражением» (`aᵢ + aₙ₋ᵢ₋₁`) – `ZipWithReversed`   | 1     |


---

### 🧱 Структура проекта
```
lab_work_number2/
|-- CMakeLists.txt
|-- README.md
|-- main.cpp – консольный UI, тесты производительности
|-- src/
| |-- ICollection.h
| |-- IEnumerable.h
| |-- IEnumerator.h
| |-- DynamicArray.h
| |-- LinkedList.h
| |-- Sequence.h
| |-- ArraySequence.h
| |-- MutableArraySequence.h
| |-- ImmutableArraySequence.h
| |-- ListSequence.h
| |-- SequenceIterator.h
| |-- Sequence_utils.h – Split, Slice, Range, GetPrefixes, GetPostfixes, SlidingAverage, Unzip
| |-- Bit.h
| |-- BitSequence.h
|
|-- tests/ – модульные тесты (Google Test)
| |-- DynamicArray_tests.cpp
| |-- LinkedList_tests.cpp
| |-- Sequence_tests.cpp
| |-- BitSequence_tests.cpp
| |-- MapReduce_tests.cpp
| |-- Iterator_tests.cpp
```

### 🎮 Возможности консольного UI

Главное меню позволяет:

1. Работать с `ArraySequence<int>` или `ListSequence<int>` (выбор mutable/immutable, ручной или случайный ввод).
2. Выполнять базовые операции: `Append`, `Prepend`, `InsertAt`, `Get`, `GetSubsequence`, `Concat`, `Map`, `Where`, `Reduce`.
3. Переходить в **меню бонусных операций**:
  - `FlatMap` (каждый элемент → подпоследовательность)
  - `Zip` (объединение с другой последовательностью)
  - `Split` (разбиение по разделителю)
  - `Slice` (удаление/замена фрагмента)
  - `Range` (генерация арифметической прогрессии)
  - `Skip` / `Take`
  - `GetPrefixes` / `GetPostfixes`
  - `SlidingAverage`
  - `ZipWithReversed`
  - `Min` / `Max` / `Avg`
4. Работать с `BitSequence` (ввод строки битов, побитовые AND, OR, XOR, NOT).
5. Запускать **тесты производительности** (замеры Append, Get, Map, Reduce, Immutable Append для разных N).  

### 🧪 Тестирование

Модульные тесты охватывают:

 - все конструкторы и методы DynamicArray, LinkedList
 - поведение mutable/immutable последовательностей 
 - битовые операции, граничные случаи
 - map‑reduce цепочки, утилиты (Split, Slice, Range, SlidingAverage и др.)
 - итераторы (IEnumerator) для всех коллекций
 - Тесты производительности встроены в UI (пункт 4 главного меню).

## 🚀 Сборка и запуск

### Требования

- **CMake** 3.10 или выше (в `CMakeLists.txt` указано `4.1`, но сборка успешно проходит и с версией 3.10+)
- **Компилятор** с поддержкой C++20 (g++ 10+, clang 12+, AppleClang 14+)
- **Git** (для автоматической загрузки Google Test)

### Инструкция

```bash
# Клонируйте репозиторий (или перейдите в папку с исходниками)
cd laboratory_work_number2

# Создайте папку для сборки
mkdir build && cd build

# Сгенерируйте Makefile
cmake ..

# Выполните сборку
cmake --build .

# Запустите основную программу
./lab_work_number2
