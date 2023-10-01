#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <algorithm>
 
template <typename Type>
class SingleLinkedList {
    // Узел списка
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    template <typename ValueType>
    class BasicIterator {
        // Класс списка объявляется дружественным, чтобы из методов списка
        // был доступ к приватной области итератора
        friend class SingleLinkedList;
 
        // Конвертирующий конструктор итератора из указателя на узел списка
        explicit BasicIterator(Node* node)
            :node_(node)
        {
        }
 
    public:
        // Объявленные ниже типы сообщают стандартной библиотеке о свойствах этого итератора
 
        // Категория итератора — forward iterator
        // (итератор, который поддерживает операции инкремента и многократное разыменование)
        using iterator_category = std::forward_iterator_tag;
 
        // Тип элементов, по которым перемещается итератор
        using value_type = Type;
 
        // Тип, используемый для хранения смещения между итераторами
        using difference_type = std::ptrdiff_t;
 
        // Тип указателя на итерируемое значение
        using pointer = ValueType*;
 
        // Тип ссылки на итерируемое значение
        using reference = ValueType&;
 
        BasicIterator() = default;
 
        // Конвертирующий конструктор/конструктор копирования
        // При ValueType, совпадающем с Type, играет роль копирующего конструктора
        // При ValueType, совпадающем с const Type, играет роль конвертирующего конструктора
        BasicIterator(const BasicIterator<Type>& other) noexcept
            : node_(other.node_)
        {
        }
 
        // Чтобы компилятор не выдавал предупреждение об отсутствии оператора = при наличии
        // пользовательского конструктора копирования, явно объявим оператор = и
        // попросим компилятор сгенерировать его за нас
        BasicIterator& operator=(const BasicIterator& rhs) = default;
 
        // Оператор сравнения итераторов (в роли второго аргумента выступает константный итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
            return rhs.node_ == node_;
        }
 
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
            return rhs.node_ != node_;
        }
 
        // Оператор сравнения итераторов (в роли второго аргумента итератор)
        // Два итератора равны, если они ссылаются на один и тот же элемент списка либо на end()
        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return rhs.node_ == node_;
        }
 
        // Оператор проверки итераторов на неравенство
        // Противоположен !=
        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
            return rhs.node_ != node_;
        }
 
        // Оператор прединкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает ссылку на самого себя
        // Инкремент итератора, не указывающего на существующий элемент списка, приводит к неопределённому поведению
        BasicIterator& operator++() noexcept {
            this->node_ = node_->next_node;
            return *this;
        }
 
        // Оператор постинкремента. После его вызова итератор указывает на следующий элемент списка
        // Возвращает прежнее значение итератора
        // Инкремент итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        BasicIterator operator++(int) noexcept {
            auto old_value(*this); // Сохраняем прежнее значение объекта для последующего возврата
            ++(*this); // используем логику префиксной формы инкремента
            return old_value;
        }
 
        // Операция разыменования. Возвращает ссылку на текущий элемент
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] reference operator*() const noexcept {
            return node_->value;
        }
 
        // Операция доступа к члену класса. Возвращает указатель на текущий элемент списка
        // Вызов этого оператора у итератора, не указывающего на существующий элемент списка,
        // приводит к неопределённому поведению
        [[nodiscard]] pointer operator->() const noexcept {
            return &node_->value;
        }
 
    private:
        Node* node_ = nullptr;
    };
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
 
    // Итератор, допускающий изменение элементов списка
    using Iterator = BasicIterator<Type>;
 
    // Константный итератор, предоставляющий доступ для чтения к элементам списка
    using ConstIterator = BasicIterator<const Type>;
 
    // Возвращает итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    [[nodiscard]] Iterator begin() noexcept {
        return (size_ == 0) ? this->end() : BasicIterator<Type>(head_.next_node);
    }
 
    // Возвращает итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] Iterator end() noexcept {
        auto it = head_.next_node;
        while (it != nullptr) {
            ++it;
        }
        return BasicIterator<Type>(it);
    }
 
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен end()
    // Результат вызова эквивалентен вызову метода cbegin()
    [[nodiscard]] ConstIterator begin() const noexcept {
        return (size_ == 0) ? end() : cbegin();
    }
 
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    // Результат вызова эквивалентен вызову метода cend()
    [[nodiscard]] ConstIterator end() const noexcept {
        return cend();
    }
 
    // Возвращает константный итератор, ссылающийся на первый элемент
    // Если список пустой, возвращённый итератор будет равен cend()
    [[nodiscard]] ConstIterator cbegin() const noexcept {
        return size_ == 0 ? this->cend() : BasicIterator<Type>(head_.next_node);
    }
 
    // Возвращает константный итератор, указывающий на позицию, следующую за последним элементом односвязного списка
    // Разыменовывать этот итератор нельзя — попытка разыменования приведёт к неопределённому поведению
    [[nodiscard]] ConstIterator cend() const noexcept {
        auto it = head_.next_node;
        while (it != nullptr) {
            ++it;
        }
        return BasicIterator<Type>(it);
    }
 
    SingleLinkedList() = default;
    
    SingleLinkedList(std::initializer_list<Type> values) : size_(0) {
        this->Assign(values);
    }
    
    SingleLinkedList(const SingleLinkedList& other) {
        SingleLinkedList tmp;
 
        tmp.Assign(other);
    
        swap(tmp);
    }
 
    void PushFront(const Type& value) {
        head_.next_node = new Node(value, head_.next_node);
        ++size_;
    }
 
    void Clear() noexcept {
        while (head_.next_node != nullptr) {
            auto ptr_copy = head_.next_node;
            head_.next_node = ptr_copy->next_node;
            delete ptr_copy;
        }
        size_ = 0;
    }
 
    // Возвращает количество элементов в списке за время O(1)
    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    }
 
    // Сообщает, пустой ли список за время O(1)
    [[nodiscard]] bool IsEmpty() const noexcept {
        return size_ != 0 ? false : true;
    }
    ~SingleLinkedList() {
        Clear();
    }
    
    void swap(SingleLinkedList<Type>& rhs) {
        size_t tmp_size = rhs.size_;
        rhs.size_ = size_;
        this->size_ = tmp_size;
 
        Node tmp_node;
        tmp_node.next_node = rhs.head_.next_node;
        rhs.head_.next_node = head_.next_node;
        head_.next_node = tmp_node.next_node;
    }
    
    template <typename Container>
    void Assign(const Container& values) {
        this->size_ = 0;
        auto proxy = this->head_.next_node;
        for (auto iter = values.begin(); iter != values.end(); ++iter) {
            proxy->next_node = new Node(*iter, nullptr);
            this->size_++;
            proxy = proxy->next_node;
        }
    }
    
    /*bool operator==(const SingleLinkedList<Type>& rhs) const noexcept {
        if(this->size_ != rhs.GetSize()) {
            return false;
        }
        for (auto liter = this->begin(), riter = rhs.begin(); liter != this.end(); ++liter, ++riter) {
            if (*liter != *riter) {
                return false;
            }
        }
        return true;
    }*/
    
    SingleLinkedList& operator=(const SingleLinkedList<Type>& rhs) {
        if (this != &rhs) {
            SingleLinkedList proxy(rhs);
            this->swap(proxy);
        }
        return *this;
    }
    
    bool operator==(const SingleLinkedList<Type>& rhs) const noexcept {
        return std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }
    
    bool operator!=(const SingleLinkedList<Type>& rhs) const noexcept {
        return !(*this == rhs);
    }
    
    bool operator<(const SingleLinkedList<Type>& rhs) const noexcept {
        return std::lexicographical_compare(
        this->begin(), this->end(), rhs.begin(), rhs.end());
    }
    
    bool operator>(const SingleLinkedList<Type>& rhs) const noexcept {
        return rhs < *this;
    }
    
    bool operator<=(const SingleLinkedList<Type>& rhs) const noexcept {
        if (*this < rhs || *this == rhs) {
            return true;
        }
        return false;
    }
    
    bool operator>=(const SingleLinkedList<Type>& rhs) const noexcept {
        if (*this > rhs || *this == rhs) {
            return true;
        }
        return false;
    }
    
 
private:
    // Фиктивный узел, используется для вставки "перед первым элементом"
    Node head_;
    size_t size_ = 0;
};

template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
    lhs.swap(rhs);
}

void Test2() {
    // Итерирование по пустому списку
    {
        SingleLinkedList<int> list;
        // Константная ссылка для доступа к константным версиям begin()/end()
        const auto& const_list = list;

        // Итераторы begin и end у пустого диапазона равны друг другу
        assert(list.begin() == list.end());
        assert(const_list.begin() == const_list.end());
        assert(list.cbegin() == list.cend());
        assert(list.cbegin() == const_list.begin());
        assert(list.cend() == const_list.end());
    }

    // Итерирование по непустому списку
    {
        SingleLinkedList<int> list;
        const auto& const_list = list;

        list.PushFront(1);
        assert(list.GetSize() == 1u);
        assert(!list.IsEmpty());

        assert(const_list.begin() != const_list.end());
        assert(const_list.cbegin() != const_list.cend());
        assert(list.begin() != list.end());

        assert(const_list.begin() == const_list.cbegin());

        assert(*list.cbegin() == 1);
        *list.begin() = -1;
        assert(*list.cbegin() == -1);

        const auto old_begin = list.cbegin();
        list.PushFront(2);
        assert(list.GetSize() == 2);

        const auto new_begin = list.cbegin();
        assert(new_begin != old_begin);
        // Проверка прединкремента
        {
            auto new_begin_copy(new_begin);
            assert((++(new_begin_copy)) == old_begin);
        }
        // Проверка постинкремента
        {
            auto new_begin_copy(new_begin);
            assert(((new_begin_copy)++) == new_begin);
            assert(new_begin_copy == old_begin);
        }
        // Итератор, указывающий на позицию после последнего элемента, равен итератору end()
        {
            auto old_begin_copy(old_begin);
            assert((++old_begin_copy) == list.end());
        }
    }
    // Преобразование итераторов
    {
        SingleLinkedList<int> list;
        list.PushFront(1);
        // Конструирование ConstIterator из Iterator
        SingleLinkedList<int>::ConstIterator const_it(list.begin());
        assert(const_it == list.cbegin());
        assert(*const_it == *list.cbegin());

        SingleLinkedList<int>::ConstIterator const_it1;
        // Присваивание ConstIterator'у значения Iterator
        const_it1 = list.begin();
        assert(const_it1 == const_it);
    }
    // Проверка оператора ->
    {
        using namespace std;
        SingleLinkedList<std::string> string_list;

        string_list.PushFront("one"s);
        assert(string_list.cbegin()->length() == 3u);
        string_list.begin()->push_back('!');
        assert(*string_list.begin() == "one!"s);
    }
}

int main() {
    Test2();
}
