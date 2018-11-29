//
// Created by Tsinin on 17.11.2018.
//

#include <iostream>
#include <exception>
#include <utility>
#include "Vector.h"
#include <memory>

template <typename Key>
class FibHeap {
public:
    class Pointer;
    struct node {
        node *child, *parent, *next, *prev;
        std::shared_ptr<Pointer> backPtr;
        int degree = 0;
        bool mark = false;
        Key data;
        node(Key newData) {
            data = newData;
            backPtr = nullptr;
            child = parent = next = prev = nullptr;
        };
        node() {
            backPtr = nullptr;
            child = parent = next = prev = nullptr;
        };
    };
    class Pointer {
    public:
        ~Pointer() = default;
    private:
        friend class FibHeap<Key>;
        Pointer(node* new_ptr): element(new_ptr) {}
        node* element;
    };
    bool is_empty() const {
        return (size == 0);
    }
    std::shared_ptr<Pointer> insert(Key x) {
        node *new_node = new node(x);
        std::shared_ptr<Pointer> pointer(new Pointer(new_node));
        new_node->backPtr = pointer;
        if(size == 0) {
            min = min->next = min->prev = new_node;
        } else {
            new_node->next = min;
            new_node->prev = min->prev;
            new_node->next->prev = new_node;
            new_node->prev->next = new_node;
            if(new_node->data < min->data)
                min = new_node;
        }
        size++;
        return pointer;
    }
    Key get_min()  {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }
        return min->data;
    }
    void merge(FibHeap &otherHeap) {
        if(otherHeap.is_empty())
            return;
        else if(this->is_empty()) {
            this->min = otherHeap.min;
            this->size = otherHeap.size;
        } else {
            node* l = this->min->prev, *r = otherHeap.min->next;
            otherHeap.min->next = this->min;
            this->min->prev = otherHeap.min;
            l->next = r;
            r->prev = l;
            if(otherHeap.min->data < this->min->data)
                min = otherHeap.min;
            size += otherHeap.size();
        }
    }
    Key extract_min() {
        try {
            if(this->is_empty())
                throw std::logic_error("Heap is empty.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(1);
        }
        Key ans = min->data;

    }
    void erase(std::shared_ptr<Pointer> &pointer) {
        try {
            if(pointer.use_count() == 1)
                throw std::invalid_argument("Element has been already deleted.");
        }
        catch(std::invalid_argument& error) {
            std::cerr << error.what() << std::endl;
            exit(2);
        }
        pointer->element->smallest = true;
        decreaseKey(pointer, pointer->element->data);
        pointer.reset();
        this->extract_min();
    }
    void change(std::shared_ptr<Pointer> &pointer, Key newKey) {
        try {
            if(pointer.use_count() == 1)
                throw std::invalid_argument("Element does not exist.");
        }
        catch(std::invalid_argument& error) {
            std::cerr << error.what() << std::endl;
            exit(2);
        }
        if(newKey > pointer->element->data) {
            this->erase(pointer);
            pointer = insert(newKey);
        } else {
            decreaseKey(pointer, newKey);
        }
    }
    FibHeap() {};
    ~FibHeap() {};
private:
    int size = 0;
    node* min = nullptr;

    node* merge(node* first, node* second) {
        if(first->data >= second->data) {
            node* temp = first;
            first = second;
            second = temp;
        }
        if(!first->lastChild) {
            first->lastChild = first->firstChild = second;
            second->parent = first;
            second->next = second->prev = nullptr;
            first->degree++;
            return first;
        }
        first->lastChild->next = second;
        second->next = nullptr;
        second->prev = first->lastChild;
        first->lastChild = first->lastChild->next;
        second->parent = first;
        first->degree++;
        return first;
    }
    void swapWithParent(node*& ptr) {
        Key tempForData = ptr->data;
        ptr->data = ptr->parent->data;
        ptr->parent->data = tempForData;

        std::shared_ptr<Pointer> tempForBackPtr = ptr->backPtr;
        ptr->backPtr = ptr->parent->backPtr;
        ptr->parent->backPtr = tempForBackPtr;

        ptr->backPtr->element = ptr->parent;
        ptr->parent->backPtr->element = ptr;
    }
    void decreaseKey(std::shared_ptr<Pointer> &pointer, int newKey) {
        node* currentNode = pointer->element;
        currentNode->data = newKey;
        while(currentNode->parent && (currentNode->smallest || currentNode->data < currentNode->parent->data)) {
            swapWithParent(currentNode);
            currentNode = currentNode->parent;
        }
    }
};
