#pragma once

#include <iterator>
/**
 * InstanceList automatically maintains a linked list of instances of child classes.
 * If you want to add an InstanceList to your class simply derive it from InstanceList

 * class myClass : protected InstanceList<myClass>
 */
template <class T>
class InstanceList
{
 protected:
    /**
     * Constructor
     * Adds the instance to the linked list
     */
    InstanceList()
    {
        InstanceList* tmp = first;
        first             = this;
        first->next       = tmp;
    }

    /**
     * Destructor
     * Removes the instance from the linked list
     * */
    ~InstanceList()
    {
        if (this == first)
        {
            first = this->next;
        }
        else
        {
            for (auto& node : instanceList)
            {
                if (node.next == this)
                {
                    node.next = this->next;
                    break;
                }
            }
        }
    }

    static InstanceList* first;
    InstanceList* next;

    //-----------------------

    struct Iterator
    {
        using iterator_category = std::input_iterator_tag;
        using value_type        = InstanceList<T>;
        using pointer           = value_type*;
        using reference         = value_type&;

        Iterator(pointer ptr) : node(ptr) {}

        T& operator*() const { return *static_cast<T*>(node); }
        T* operator->() { return static_cast<T*>(node); }

        // Prefix increment
        Iterator& operator++()
        {
            node = node->next;
            return *this;
        }

        // Postfix increment
        Iterator operator++(int)
        {
            Iterator tmp = node;
            node         = node->next;
            return tmp;
        }

        pointer node;
        friend bool operator==(const Iterator& a, const Iterator& b) { return a.node == b.node; };
        friend bool operator!=(const Iterator& a, const Iterator& b) { return a.node != b.node; };
    };

    static struct
    {
        Iterator begin() { return Iterator(first); }
        Iterator end() { return Iterator(nullptr); }
    } instanceList;
};

template <class T>
InstanceList<T>* InstanceList<T>::first = nullptr;
