#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <string>
#include <random>

template <typename T>
struct Node {
    int apartmentNumber; 
    T tenantName;       
    Node* next;

    Node(int number, const T& name) : apartmentNumber(number), tenantName(name), next(nullptr) {}
};

template <typename T>
class ApartmentList {
private:
    Node<T>* head; 
    Node<T>* tail; 
    size_t size;   

public:
    // Конструктор по умолчанию
    ApartmentList() : head(nullptr), tail(nullptr), size(0) {}

    // Конструктор копирования
    ApartmentList(const ApartmentList& other) : head(nullptr), tail(nullptr), size(0) {
        Node<T>* current = other.head;
        while (current) {
            push_tail(current->apartmentNumber, current->tenantName);
            current = current->next;
        }
    }

    
    ApartmentList(size_t count, double min_value, double max_value, int v) : head(nullptr), tail(nullptr), size(0) {
        std::mt19937 gen(v); 
        std::uniform_real_distribution<double> dis(min_value, max_value); 

        for (size_t i = 0; i < count; ++i) {
            int apartmentNumber = static_cast<int>(i + 1); 
            T tenantName = "Tenant " + std::to_string(dis(gen));
            push_tail(apartmentNumber, tenantName); 
        }
    }

    // Деструктор
    ~ApartmentList() {
        clear();
    }

    // Операция присваивания
    ApartmentList& operator=(const ApartmentList& other) {
        if (this != &other) {
            clear(); 
            Node<T>* current = other.head;
            while (current) {
                push_tail(current->apartmentNumber, current->tenantName);
                current = current->next;
            }
        }
        return *this;
    }

    // Метод для добавления жильца в конец списка
    void push_tail(int apartmentNumber, const T& tenantName) {
        Node<T>* newNode = new Node<T>(apartmentNumber, tenantName);
        if (!head) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    // Перегруженный метод для добавления другого списка в конец
    void push_tail(const ApartmentList& other) {
        Node<T>* current = other.head;
        while (current) {
            push_tail(current->apartmentNumber, current->tenantName);
            current = current->next;
        }
    }

    // Метод для добавления жильца в начало списка
    void push_head(int apartmentNumber, const T& tenantName) {
        Node<T>* newNode = new Node<T>(apartmentNumber, tenantName);
        newNode->next = head;
        head = newNode;
        if (!tail) { 
            tail = newNode;
        }
        size++;
    }

    // Перегруженный метод для добавления другого списка в начало
    void push_head(const ApartmentList& other) {
        Node<T>* current = other.head;
        while (current) {
            push_head(current->apartmentNumber, current->tenantName);
            current = current->next;
        }
    }

    // Метод для удаления первого жильца списка
    void pop_head() {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
        if (!head) { 
            tail = nullptr;
        }
    }

    // Метод для удаления последнего жильца списка
    void pop_tail() {
        if (!head) {
            throw std::runtime_error("List is empty");
        }
        if (head == tail) { 
            delete head;
            head = nullptr;
            tail = nullptr;
        }
        else {
            Node<T>* current = head;
            while (current->next != tail) {
                current = current->next;
            }
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        size--;
    }

    // Метод для удаления всех элементов с указанным именем жильца
    void delete_node(const T& tenantName) {
        Node<T>* current = head;
        Node<T>* previous = nullptr;
        while (current) {
            if (current->tenantName == tenantName) {
                if (current == head) {
                    pop_head();
                    current = head; 
                }
                else {
                    previous->next = current->next;
                    if (current == tail) {
                        tail = previous; 
                    }
                    delete current;
                    current = previous->next; 
                    size--;
                }
            }
            else {
                previous = current;
                current = current->next;
            }
        }
    }

    // Метод для добавления жильца в квартиру по номеру
    void add(int apartmentNumber, const T& tenantName) {
        Node<T>* current = head;
        while (current != nullptr) {
            if (current->apartmentNumber == apartmentNumber) {
                current->tenantName = tenantName; // Обновляем имя арендатора
                return;
            }
            current = current->next;
        }
        // Если квартира не найдена, выбрасываем исключение
        throw std::runtime_error("Apartment not found");
    }

    // Операция доступа по индексу - чтение
    const T& operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->tenantName;
    }

    // Операция доступа по индексу - запись
    T operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = head;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->tenantName;
    }

    // Получение размера списка
    size_t get_size() const {
        return size;
    }

    // Очистка списка
    void clear() {
        while (head) {
            pop_head();
        }
    }

    // Перегрузка оператора << для удобного вывода
    friend std::ostream& operator<<(std::ostream& os, const ApartmentList<T>& list) {
        Node<T>* current = list.head;
        while (current) {
            os << "Apartment " << current->apartmentNumber << ": " << current->tenantName << " -> ";
            current = current->next;
        }
        os << "nullptr";
        return os;
    }    
};

int main() {
    ApartmentList<std::string> apartments(5, 101, 202,42);
    ApartmentList<std::string> apar(5, 303, 404,42);
    std::cout << apartments << std::endl;
    std::cout << apar << std::endl;
    
    apartments.push_tail(6, "NewTenant");
    apartments.add(1, "staryy");
    std::cout << apartments << std::endl;

    apartments.push_head(0, "FirstTenant");
    std::cout << apartments << std::endl;

    apartments.pop_head();
    std::cout << apartments << std::endl;

    apartments.pop_tail();
    std::cout << apartments << std::endl;
    
    std::cout << apartments << std::endl;
    apartments.delete_node("Tenant1");
    std::cout << apartments << std::endl;

    std::cout << "Current size: " << apartments.get_size() << std::endl;

    return 0;
}
