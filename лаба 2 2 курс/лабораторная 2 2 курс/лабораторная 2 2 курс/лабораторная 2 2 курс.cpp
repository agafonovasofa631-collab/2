#include <iostream>
#include <chrono>
#include <vector>

using namespace std;


// Реализация собственного класс Vector

template <typename T>
class Vector {
private:
    T* data;    // Указатель на кусок памяти в куче, где лежат элементы
    int sz;     // Сколько элементов сейчас реально лежит в массиве
    int cap;    // Сколько всего памяти мы выделили (capacity всегда >= size)

public:
    // По умолчанию массив пустой, ничего не выделено
    Vector() : data(nullptr), sz(0), cap(0) {}

    // В деструкторе чистим память
    ~Vector() {
        delete[] data;
    }

    // Метод reserve: заранее выделяет память под newCap элементов
    void reserve(int newCap) {
        if (newCap <= cap) return;

        // Выделяем новую память
        T* newData = new T[newCap];

        // Перекидываем (копируем) старые элементы в новый массив
        for (int i = 0; i < sz; ++i) {
            newData[i] = data[i];
        }

        delete[] data; // Старую память удаляем
        data = newData; // Переключаем указатель на новую память
        cap = newCap;   // Обновляем информацию
    }

    // Метод push_back: добавляет элемент в конец
    void push_back(const T& value) {
        // Если места больше нет 
        if (sz >= cap) {
            // Выделяем в 2 раза больше памяти (или 1, если массив был пуст).
            int newCap = (cap == 0) ? 1 : cap * 2;
            reserve(newCap);
        }
        // Записываем новый элемент в конец и увеличиваем размер
        data[sz] = value;
        sz++;
    }

    // Метод pop_back: удаляет последний элемент
    void pop_back() {
        if (sz > 0) {
            sz--; 
        }
    }

    // Оператор [] для доступа по индексу 
    T& operator[](int index) {
        return data[index];
    }

    // Константная версия оператора []
    const T& operator[](int index) const {
        return data[index];
    }

    // Геттеры для размера и емкости
    int size() const { return sz; }
    int capacity() const { return cap; }
};


// Тестируем и замеряем время
int main() {
    setlocale(LC_ALL, "ru"); 
    const int N = 100000;    

    // Замер своего Vector 
    Vector<int> myVec;
    auto start1 = chrono::high_resolution_clock::now(); // Засекаем время

    for (int i = 0; i < N; ++i) {
        myVec.push_back(i);
    }

    auto end1 = chrono::high_resolution_clock::now();   // Останавливаем
    // Считаем разницу в миллисекундах
    auto duration1 = chrono::duration_cast<chrono::milliseconds>(end1 - start1).count();

    // Замеряем стандартный std::vector 
    vector<int> stdVec;
    auto start2 = chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        stdVec.push_back(i);
    }

    auto end2 = chrono::high_resolution_clock::now();
    auto duration2 = chrono::duration_cast<chrono::milliseconds>(end2 - start2).count();

    
    cout << "Сравнение производительности (N = " << N << ") \n";
    cout << "Мой Vector<T> : " << duration1 << " мс\n";
    cout << "std::vector   : " << duration2 << " мс\n";

    // Проверка, что основные методы работают правильно 
    cout << "\nПроверка функционала \n";
    Vector<int> testVec;
    testVec.push_back(10);
    testVec.push_back(20);
    testVec.push_back(30);

    // Выводим элементы через цикл и оператор []
    cout << "Элементы testVec: ";
    for (int i = 0; i < testVec.size(); ++i) {
        cout << testVec[i] << " ";
    }
    cout << "\n";
    cout << "Размер: " << testVec.size() << ", Емкость: " << testVec.capacity() << "\n";

    // Проверяем pop_back
    testVec.pop_back();
    cout << "После pop_back() размер: " << testVec.size() << "\n";

    return 0;
}