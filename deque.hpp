#ifndef _DEQUE_HPP_
#define _DEQUE_HPP_

#include <functional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <regex>
#include <vector>

namespace myDataStructure {
    template<typename T>
    class deque {
        const std::regex functionNames[9] = {
            std::regex("isEmpty"),
            std::regex("isFull"),
            std::regex("pop_front"),
            std::regex("pop_back"),
            std::regex("push_front(\\s*)(\\d*)"),
            std::regex("push_back(\\s*)(\\d*)"),
            std::regex("print"),
            std::regex("front"),
            std::regex("back")
        };

        T* array;
        int maxSize;
        int size;
        int head;
        int tail;

        std::vector<std::string> functionQueue;

        typedef std::function<bool(myDataStructure::deque<T>&)> funcType1;
        typedef std::function<bool(myDataStructure::deque<T>&, const T&)> funcType2;
        typedef std::function<T(myDataStructure::deque<T>&)> funcType3;

        funcType1 getFunction1(const std::string& name);
        funcType2 getFunction2(const std::string& name);
        funcType3 getFunction3(const std::string& name);

        bool checkFuncion(const std::string& in);
        void readFromFile(void);

    public:
        deque() = default;
        deque(const int& _maxSize);
        deque(const int& _quantity, const T& _value);

        void doActions(void);
        bool print(void);
        bool isEmpty(void);
        bool isFull(void);
        bool push_front(const T& value);
        bool push_back(const T& value);
        bool pop_back(void);
        bool pop_front(void);

        T front(void);
        T back(void);
    };
}

template<typename T>
void myDataStructure::deque<T>::doActions(void) {
    this->readFromFile();

    for (int i = 0; i < functionQueue.size(); i++) {
        if (functionQueue[i] == "pop_back"  || 
            functionQueue[i] == "pop_front" ||
            functionQueue[i] == "isFull"    ||
            functionQueue[i] == "isEmpty"     ) {
                funcType1 func = this->getFunction1(functionQueue[i]);
                func(*this);
        } else if (functionQueue[i].substr(0, 9) == "push_back" ||
                   functionQueue[i].substr(0, 10) == "push_front") {
                funcType2 func;
                std::string arg;
                if (functionQueue[i].substr(0, 9) == "push_back") {
                    func = this->getFunction2(functionQueue[i].substr(0, 9));
                    arg = functionQueue[i].substr(10);
                } else {
                    func = this->getFunction2(functionQueue[i].substr(0, 10));
                    arg = functionQueue[i].substr(11);
                }
                func(*this, stoi(arg));
        } else {
            funcType3 func = this->getFunction3(functionQueue[i]);
            func(*this);
        }
    }
}

template<typename T>
void myDataStructure::deque<T>::readFromFile(void) {
    std::ifstream in("input.txt", std::ios::in);
    while (!in.eof()) {
        std::string s;
        getline(in, s);

        if (0 == s.compare("END")) {
            break;
        } else {
            if (true == this->checkFuncion(s)) {
                functionQueue.push_back(s);
            } else {
                std::cout << "WRONG" << std::endl;
                return;
            }
        }
    }
}

template<typename T>
typename myDataStructure::deque<T>::funcType1 
myDataStructure::deque<T>::getFunction1(const std::string& name) {
    if (name == "pop_back") {
        return &deque<T>::pop_back;
    } else if (name == "pop_front") {
        return &deque<T>::pop_front;
    } else if (name == "isFull") {
        return &deque<T>::isFull;
    } else { return &deque<T>::isEmpty; }
}

template<typename T>
typename myDataStructure::deque<T>::funcType2
myDataStructure::deque<T>::getFunction2(const std::string& name) {
    return (name == "push_front" ? &deque<T>::push_front : &deque<T>::push_back);
}

template<typename T>
typename myDataStructure::deque<T>::funcType3
myDataStructure::deque<T>::getFunction3(const std::string& name) {
    return (name == "front" ? &deque<T>::front : &deque<T>::back);
}

template<typename T>
myDataStructure::deque<T>::deque(const int& _maxSize) {
    array = new T[_maxSize];
    maxSize = _maxSize;
    head = tail = 0;
    size = 0;
}

template<typename T>
myDataStructure::deque<T>::deque(const int& _quantity, const T& _item) {
    array = new T[_quantity];
    size = maxSize = _quantity;
    head = 0;
    tail = _quantity - 1;

    for (int i = 0; i < _quantity; i++) { array[i] = _item; }
}

template<typename T>
bool myDataStructure::deque<T>::print(void) {
    for (int i = 0, j = head; i < size; i++, j = (j + 1) % maxSize) {
        if (i != size - 1) { std::cout << array[j] << " "; }
        else { std::cout << array[j] << std::endl; }
    }
    return true;
}

template<typename T>
bool myDataStructure::deque<T>::checkFuncion(const std::string& in) {
    for (int i = 0; i < 9; i++) {
        if (std::regex_match(in, functionNames[i])) {
            return true;
        }
    }
    return false;
}

template<typename T>
bool myDataStructure::deque<T>::isFull(void) {
    std::cout << (size == maxSize ? "YES" : "NO") << std::endl;
    return (size == maxSize);
}

template<typename T>
bool myDataStructure::deque<T>::isEmpty(void) {
    std::cout << (size == 0 ? "YES" : "NO") << std::endl;
    return size == 0;
}

template<typename T>
bool myDataStructure::deque<T>::push_back(const T& value) {
    //First check the deque's size.
    if (size == maxSize) {
        std::cout << "FULL." << std::endl;
        return false;
    } else if (size == 0 && head == tail) {
        array[tail] = value;
        size ++;
    } else {
        tail = (tail + 1) % maxSize;
        array[tail] = value;
        size ++;
    }
    this->print();
    return true;
}

template<typename T>
bool myDataStructure::deque<T>::push_front(const T& value) {
    //First check the deque's size.
    if (size == maxSize) {
        std::cout << "FULL." << std::endl;
        return false;
    
    //Accidentally the deque is empty and uninitialized.
    } else if (size == 0 && head == tail) {
        array[head] = value;
        size ++;
    } else {
        head = (head - 1 + maxSize) % maxSize;
        array[head] = value;
        size ++;
    }
    this->print();
    return true;
}

template<typename T>
bool myDataStructure::deque<T>::pop_back(void) {
    if (0 == size) {
        std::cout << "EMPTY." << std::endl;
        return false;
    } else {
        tail = (tail - 1 + maxSize) % maxSize;
        size --;
        this->print();
        return true;
    }
}

template<typename T>
bool myDataStructure::deque<T>::pop_front(void) {
    if (0 == size) {
        std::cout << "EMPTY." << std::endl;
        return false;
    } else {
        head = (head + 1) % maxSize;
        size --;
        this->print();
        return true;
    }
}

template<typename T>
T myDataStructure::deque<T>::front(void) {
    if (0 == size) {
        std::cout << "EMPTY." << std::endl;
        return std::move(-1);
    }
    std::cout << array[head] << std::endl;
    return array[head];
}

template<typename T>
T myDataStructure::deque<T>::back(void) {
    if (0 == size) {
        std::cout << "EMPTY." << std::endl;
        return std::move(-1);
    }
    std::cout << array[tail] << std::endl;
    return array[tail];
}

#endif