//
// Created by Tsinin on 09.11.2018.
//

template <typename T>
class Vector {
public:
    class Iterator {
    public:
        Iterator(Iterator* iterator) {
            this->ptr = iterator->ptr;
            this->index = iterator->index;
        }
        Iterator(int new_index, Vector<T>* new_ptr): index(new_index), ptr(new_ptr) {};
        Iterator operator++()         // Pre-increment Operator
        {
            index++;
            return *this;
        }
        Iterator operator++(int)         // Post-increment Operator
        {
            const Iterator old = *this;
            ++(*this);
            return old;
        }
        T& operator*() {
            T& data = (*ptr)[index];
            return data;
        }
        bool operator==(const Iterator& iterator) {
            return (this->ptr == iterator.ptr) && (this->index == iterator.index);
        }
        bool operator!=(const Iterator& iterator) {
            return (*this == iterator);
        }
    private:
        Vector<T>* ptr;
        int index;
    };
    int max_sz() const{
        return max_size;
    }
    int size() const{
        return vector_size;
    }
    T& operator[](int i) {
        T& element = data[i];
        return element;
    }
    Iterator begin() {
        return Iterator(0, this);
    }
    Iterator end() {
        return Iterator(vector_size, this);
    }
    void push_back(T new_x) {
        if(vector_size == max_size) {
            max_size = max_size * 2 + 1;
            T *new_data = new T[max_size];
            for(int i = 0; i < vector_size; i++)
                new_data[i] = data[i];
            T* temporary_for_swap = data;
            data = new_data;
            new_data = temporary_for_swap;
            delete[] new_data;
        }
        data[vector_size] = new_x;
        vector_size++;
    }
    T pop_back() {
        vector_size--;
        T lastElement = data[vector_size - 1];
        if(4 * vector_size <= max_size) {
            max_size /= 2;
            T *new_data = new T[max_size];
            for(int i = 0; i < vector_size; i++)
                new_data[i] = data[i];
            T* temporary_for_swap = data;
            data = new_data;
            new_data = temporary_for_swap;
            delete[] new_data;
        }
        return lastElement;
    }
    Vector() {
        vector_size = 0;
        max_size = 1;
        data = new T[max_size];
    }
    ~Vector() {
        delete[] data;
    }
private:
    int vector_size, max_size;
    T *data;
};
