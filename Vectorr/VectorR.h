#pragma once
#define DEBUG
#include<iostream>
#include<utility>

template <typename T>
class VectorR;

template <typename T>
std::ostream &operator<<(std::ostream &out, const VectorR<T> &obj)
{
    out << "VectorR [" << obj.size() << "] elements values {";

    for(size_t i{0}; i < obj.size(); ++i) {

        if(i < obj.size() - 1)
            out << obj[i] << ", ";
        else {
            out << obj[i];
        }
    }
    out << '}';

#ifdef DEBUG
    out << "\nVector size " << obj.size() << ", ";
    out << "Vector capacity " << obj.capacity() << std::endl;
#endif

    return out;
}

//оператор >>
template<typename T>
std::istream &operator>>(std::istream &input, VectorR<T> &obj)
{
    std::cout << "Input values for elements\n";
    for(int i = 0; i < obj.size(); ++i) {
        std::cout << "?element [" << i << "]->";
        if(input.fail()) {//обработка ввода неверного значения
            input.clear();
            input.ignore();
            std::cout << "Not valid value. Once again\n";
            i -= 2;
        } else {
            input >> obj[i];
        }

    }

    return input;
}

template <typename T>
class VectorR
{
    private:
    T *m_array;
    size_t m_size;
    size_t m_capacity;
    
    //оператор <<
    // один из вариантов для работы <<, мы должны вынести определение оператора<< до шаблона
    friend std::ostream & operator<< <T>(std::ostream &out, const VectorR &obj);
    
    //оператор >>
    friend std::istream &operator>> <T>(std::istream &input, VectorR &obj);
       
    //прячу сеттеры чтоб пользователь не имел доступа к данным методам
    // т.к. в таком случае он не сможет во время работы нарушить конструкцию выделяемого объекта
    ///сеттеры
    void setArray(const size_t &size);
    void setSize(const size_t &size);
    void setCapacity(const size_t &capacity);

    //Обнуление значений
    void setToNull();
    void setAllElements(const T &value);

    //очистка указателя
    void clearArrayPtr();
    
    //проверка индекса
    bool inRange(const size_t &index) const;

    //перераспределение памяти для массива
    void reallocateArr(const size_t &size);
    
    ///Сортировка(QSort)
    int partitionAsc(int start, int end, bool asc);
    void qSortt(const int start, const int end, bool asc);

    public:
    ///конструкторы
    //удалил ктор по умолчанию т.к. не пришло еще в голову как инициализировать первый элемент шаблона
    VectorR() = delete;
    VectorR(const T &value);
    VectorR(const size_t &size, const T &value);
     
    //Конструктор копирования
    VectorR(const VectorR<T> &other);

    //Конструктор перемещения
    VectorR(VectorR<T> &&other);

    ~VectorR();

    ///Операторы
    //оператор присваивания
    const VectorR<T> &operator=(const VectorR<T> &right);

    //оператор перемещения
    VectorR<T> &operator=(VectorR<T> &&right);

    //оператор []
    T &operator[](const size_t &index);
    const T &operator[](const size_t &index) const;

    //оператор ==
    bool operator==(const VectorR<T> &right) const;
    bool operator!=(const VectorR<T> &right) const;

    T getElementAt(const size_t &index);

    ///фун-ии работы с массивом
    
    //вставить элемент в позицию
    void insertAt(const size_t &index, const T &element);

    //добавление в начало массива
    void pushFront(const T &element);

    //добавление в конец массива
    void pushBack(const T &element);
    
    //удалить элемент в позиции
    void removeAt(const size_t &index);

    //удаление первого элемента
    void popFront();

    //удаление последнего элемента
    void popBack();

    //реверс массива
    void reverse();

    //поиск элемента по значению
    int indexByValue(const T &value);

    //удаление элементов по значению
    void removeByValue(const T &value);

    //подгонка capacity к size
    void trimToSize();

    ///Геттеры
    const size_t size() const;
    const size_t capacity() const;

    ///сортировка Вектора
    void sortAsc();
    void sortDesc();
};

//template <typename T>
//VectorR<T>::VectorR() {
//    setArray(1);
//}

template <typename T>
VectorR<T>::VectorR(const T &value)
    :m_array{nullptr}
{
    setArray(1);
    setAllElements(value);
}

template <typename T>
VectorR<T>::VectorR(const size_t &size, const T &value)
    :m_array{nullptr}
{
    if(size > 0) {
        setArray(size);
        setAllElements(value);
    }
    else
        throw std::out_of_range("Invalid size!");
}

template <typename T>
VectorR<T>::VectorR(const VectorR<T> &other)
    :m_size{other.m_size},
     m_capacity{other.m_capacity}
{
    m_array = new T[m_capacity];

    for(size_t i{0}; i < other.m_size; ++i) {
        m_array[i] = other.m_array[i];
    }
}

template <typename T>
VectorR<T>::VectorR(VectorR<T> &&other)
    :m_size{other.m_size},
    m_capacity{other.m_capacity} 
{
    m_array = other.m_array;
    other.setToNull();
}

template <typename T>
VectorR<T>::~VectorR() {
    clearArrayPtr();

#ifdef DEBUG
    std::cout << "VectorR in rules deleted!\n";
#endif
}

template <typename T>
const VectorR<T> &VectorR<T>::operator=(const VectorR<T> &right) {
    if(this == &right)
        return *this;

    clearArrayPtr();

    m_array = new T[m_capacity];
    for(size_t i{0}; i < right.m_size; ++i) {
        m_array[i] = right.m_array[i];
    }

    return *this;
}

template <typename T>
VectorR<T> &VectorR<T>::operator=(VectorR<T> &&right) {
    if(this == &right)
        return *this;

    clearArrayPtr();

    m_array = right.m_array;
    right.setToNull();
    
    return *this;
}

template <typename T>
T &VectorR<T>::operator[](const size_t &index) {
    if(!inRange(index))
        throw std::out_of_range("Invalid index of element");

    return m_array[index];
}

template <typename T>
const T &VectorR<T>::operator[](const size_t &index) const {
    if(!this->inRange(index))
        throw std::out_of_range("Invalid index of element");
    
    return m_array[index];
}

template<typename T>
inline bool VectorR<T>::operator==(const VectorR<T> &right) const
{
    bool equal{false};

    if(this->m_size == right.m_size) {
        equal = true;
        
        for(size_t i{0}; i < m_size; ++i) {
            if(this->m_array[i] != right.m_array[i]) {
                equal = false;
                break;
            }
        }
    }

    return equal;
}

template<typename T>
inline bool VectorR<T>::operator!=(const VectorR<T> &right) const
{
    return !(*this==right);
}

template<typename T>
inline T VectorR<T>::getElementAt(const size_t& index)
{
    if(!this->inRange(index))
        throw std::out_of_range("Invalid index of element");

    return m_array[index];
}

template <typename T>
void VectorR<T>::setArray(const size_t &size) {
    
    clearArrayPtr();

    setSize(size);
    setCapacity(size * 3 / 2);
    m_array = new T[m_capacity];
}

template <typename T>
void VectorR<T>::setSize(const size_t &size) {
    
        m_size = size;
}

template <typename T>
void VectorR<T>::setCapacity(const size_t &capacity) {
    
    m_capacity = capacity;
}

template<typename T>
void VectorR<T>::setToNull() {
    
    m_array = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template <typename T>
void VectorR<T>::setAllElements(const T &value)
{
    for(size_t i{0}; i < m_size; ++i)
        m_array[i] = value;
}

template<typename T>
void VectorR<T>::clearArrayPtr() {
    if(m_array != nullptr) {
        delete[]m_array;
        m_array = nullptr;
    }
}

template <typename T>
const size_t VectorR<T>::size() const {
    return m_size;
}

template <typename T>
const size_t VectorR<T>::capacity() const {
    return m_capacity;
}

template <typename T>
bool VectorR<T>::inRange(const size_t &index) const {
    if(index >= 0 && index < m_size) {
        return true;
    }

    return false;
}
template <typename T>
void VectorR<T>::reallocateArr(const size_t &size)
{
    VectorR<T> tmp{*this};
    
    delete[] this->m_array;
    
    setSize(size);
    setCapacity(size * 3 / 2);
    
    this->m_array = new T[m_capacity];
    
    for(size_t i{0}; i < m_size; ++i) {
        m_array[i] = tmp[i];
    }
}

template <typename T>
void VectorR<T>::insertAt(const size_t &index, const T& element)
{
    
    
    //проверяю index
    if(index > m_size)
        throw(std::out_of_range("Invalid index!"));

    ++m_size;
    
    //если нет свободного места перевыделяю память под новый массив
    // и переношу данные из старого
    if(m_size >= m_capacity) {
        reallocateArr(m_size);
    }
    
    //алгоритм внесения элемента в массив
    
        
        T buff{element};
        T prev;

        for(size_t i{index}; i < m_size; ++i) {
            prev = m_array[i];
            m_array[i] = buff;
            buff = prev;
        }
        
    
    
}

template<typename T>
inline void VectorR<T>::pushFront(const T &element)
{
    insertAt(0, element);
}

template<typename T>
inline void VectorR<T>::pushBack(const T &element)
{
    if(m_size == m_capacity) {
        reallocateArr(m_size + 1);
    }
    m_array[m_size] = element;
    ++m_size;
}

template <typename T>
void VectorR<T>::removeAt(const size_t &index)
{
    if(m_size == 0) {
        std::cout << "Nothing to remove! Vector is empty!\n";
        return;
    }

    ////проверяю index
    //if(index >= m_size)
    //    throw(std::out_of_range("Invalid index!"));
        
    
    //алгоритм удаления элемента из массива
    for(size_t i{index}; i < m_size; ++i) {
        m_array[i] = m_array[i + 1];
    }

    --m_size;
}

template<typename T>
inline void VectorR<T>::popFront()
{
    removeAt(0);
}

template<typename T>
inline void VectorR<T>::popBack()
{
    removeAt(m_size - 1);
}

template <typename T>
int VectorR<T>::partitionAsc(int start, int end, bool asc)
{
    int pivot = m_array[end];

    int pIndex = start;

    if(asc) {
        for(int i{start}; i < end; ++i) {
            if(m_array[i] <= pivot) {
                std::swap(m_array[i], m_array[pIndex]);
                ++pIndex;
            }
        }
    }
    else {
        for(int i{start}; i < end; ++i) {
            if(m_array[i] >= pivot) {
                std::swap(m_array[i], m_array[pIndex]);
                ++pIndex;
            }
        }
    }
    std::swap(m_array[pIndex], m_array[end]);

    return pIndex;
}

template <typename T>
void VectorR<T>::qSortt(const int start, const int end, bool asc)
{
    if(start >= end) {
        return;
    }

    //точка опора // сортировка от передаваемого индекса до точки опора
    int pivot = partitionAsc(start, end, asc);

    //сортирую массив до точки опора
    qSortt(start, pivot - 1, asc);
    
    //сортирую массив после точки опора
    qSortt(pivot + 1, end, asc);
}

template <typename T>
void VectorR<T>::sortAsc()
{
    qSortt(0, m_size-1, true);
}

template <typename T>
void VectorR<T>::sortDesc()
{
    qSortt(0, m_size - 1, false);
}

template <typename T>
void VectorR<T>::reverse()
{
    T tmp;

    for(size_t i{0}; i < (m_size / 2); ++i) {
        tmp = m_array[i];
        m_array[i] = m_array[m_size - 1 - i];
        m_array[m_size - 1 - i] = tmp;
    }
}

template<typename T>
inline int VectorR<T>::indexByValue(const T &value)
{
    for(size_t i{0}; i < m_size; ++i) {
        if(m_array[i] == value)
            return i;
    }

    //std::cout << "No element with value \'" << value << "\' !\n";

    return -1;
}

template<typename T>
inline void VectorR<T>::removeByValue(const T &value)
{
    int counterDeleted{0};

    while(true)
    {
        int index = indexByValue(value);

        if(index != -1) {
            removeAt(index);
            ++counterDeleted;
        } else {
            break;
        }

    }

    if(counterDeleted)
        std::cout << counterDeleted << " elements in Vector deleted\n";
    else
        std::cout << "No elements with such value in Vector\n";
}

template<typename T>
inline void VectorR<T>::trimToSize()
{
    VectorR<T> tmp{*this};

    delete[] this->m_array;
        
    setCapacity(m_size);

    this->m_array = new T[m_capacity];

    for(size_t i{0}; i < m_size; ++i) {
        m_array[i] = tmp[i];
    }
}
