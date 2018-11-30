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
        Pointer* backPtr;
        int degree = 0;
        bool mark = false;
        Key data;
        explicit node(Key newData) {
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
        explicit Pointer(node* new_ptr): element(new_ptr) {}
        node* element;
    };
    bool is_empty() const {
        return (size == 0);
    }
    Pointer* insert(Key x) {
        node *new_node = new node(x);
        Pointer* pointer = new Pointer(new_node);
        new_node->backPtr = pointer;
        if(size == 0) {
            min->prev = min->next = min = new_node;
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
    Key get_min() const {
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
            union_lists(this->min, otherHeap.min);
            if(otherHeap.min->data < this->min->data)
                min = otherHeap.min;
            size += otherHeap.size;
        }
        otherHeap.min = nullptr;
        otherHeap.size = 0;
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
        if(min->degree > 0)
            union_lists(min, min->child);
        node *l = min->prev, *r = min->next;
        l->next = r;
        r->prev = l;
        delete min->backPtr;
        delete min;
        min = r;
        size--;
        consolidate();
        return ans;
    }
    void decrease(Pointer* pointer, Key newKey) {
        try {
            if(newKey > pointer->element->data)
                throw std::invalid_argument("Element is greater than previous one.");
        }
        catch(std::logic_error& error) {
            std::cerr << error.what() << std::endl;
            exit(2);
        }
        node* element = pointer->element;
        if(element->parent == nullptr) {
            element->data = newKey;
            if(newKey < min->data)
                min = element;
            return;
        } else if(newKey > element->parent->data) {
            pointer->element->data = newKey;
            return;
        }
        node* parent = element->parent;
        cut(element);
        cascading_cut(parent);
    }
    FibHeap() = default;
    ~FibHeap() {
        if(size == 0)
            return;
        int count = 1;
        node* temp = min->next;
        while(temp != min) {
            temp = temp->next;
            count++;
        }
        for(int i = 0; i < count; ++i) {
            temp = min->next;
            destruct(min, 0);
            min = temp;
        }
    };
private:
    const int max_degree = 100;
    int size = 0;
    node* min = nullptr;
    void destruct(node* x, int a) {
        if(x == nullptr)
            return;
        node* child = x->child;
        for(int i = 0; i < x->degree; ++i) {
            node *temp = child->next;
            destruct(child, a + 1);
            child = temp;
        }
        delete x->backPtr;
        delete x;
    }
    void cut(node* x) {
        x->next->prev = x->prev;
        x->prev->next = x->next;
        x->parent->degree--;
        if(x->parent->child == x) {
            if(x->next == x)
                x->parent->child = nullptr;
            else
                x->parent->child = x->next;
        }
        x->next = x->prev = x;
        x->parent = nullptr;
        union_lists(min, x);
    }
    void cascading_cut(node* x) {
        while(x->mark == true) {
            node* parent= x->parent;
            cut(x);
            x = parent;
        }
        x->mark = true;
    }
    void consolidate() {
        if(size == 0)
            return;
        Vector<node*> nodes(max_degree, nullptr);
        node* current = min;
        while(current != nullptr) {
            node* toTheNext = current->next;
            if(current == current->next)
                toTheNext = nullptr;
            else {
                current->next->prev = current->prev;
                current->prev->next = current->next;
            }
            current->next = current->prev = current->parent = nullptr;
            if(nodes[current->degree] == nullptr)
                nodes[current->degree] = current;
            else while(nodes[current->degree] != nullptr) {
                node* second = nodes[current->degree];
                if(current->data > second->data) {
                    node* temp = current;
                    current = second;
                    second = temp;
                }
                if(current->child == nullptr) {
                    current->child = second;
                    second->next = second->prev = second;
                } else {
                    second->prev = current->child;
                    second->next = current->child->next;
                    second->prev->next = second;
                    second->next->prev = second;
                }
                nodes[current->degree] = nullptr;
                current->degree++;
                second->parent = current;
            }
            nodes[current->degree] = current;
            current = toTheNext;
        }
        min = nullptr;
        for(int i = 0; i < max_degree; i++) {
            if(nodes[i] != nullptr) {
                if(min == nullptr) {
                    min = nodes[i];
                    min->next = min->prev = min;
                } else {
                    nodes[i]->next = min->next;
                    nodes[i]->prev = min;
                    nodes[i]->next->prev = nodes[i];
                    nodes[i]->prev->next = nodes[i];
                }
                if(min->data > min->next->data && min != min->next)
                    min = min->next;
            }
        }
    }
    void union_lists(node* first, node* second) {
        node *l = first->prev, *r = second->next;
        second->next = first;
        first->prev = second;
        l->next = r;
        r->prev = l;
    }
};
