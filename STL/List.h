#ifndef LIST_H
#define LIST_H

#include <iterator>
#include <stdexcept>
#include <limits>

template<class T>
class List
{
    class Node
    {
    public:
        T value;
        Node* ptr_next;
        Node* ptr_prev;
    };
    Node* ptr_first;
    Node* ptr_last;
    std::size_t size;
public:
    class iterator :public std::iterator<std::bidirectional_iterator_tag, T>
    {
    protected:
        Node* p;
    public:
        iterator() :p(NULL) {}
        iterator(Node* ptr) :p(ptr) {}
        iterator(const iterator& other) :p(other.p) {}
        iterator& operator=(const iterator& other) {p = other.p; return *this;}
        T& operator*() {return p->value;}
        T* operator->() {return &(p->value);}

        iterator operator++() {p = p->ptr_next; return *this;}
        iterator operator++(int) {iterator clone(*this); p = p->ptr_next; return clone;}
        iterator operator--() {p = p->ptr_prev; return *this;}
        iterator operator--(int) {iterator clone(*this); p = p->ptr_prev; return clone;}

        bool operator==(const iterator& other) {return (p == other.p);}
        bool operator!=(const iterator& other) {return (p != other.p);}
    };

    class reverse_iterator :public iterator
    {
    public:
        reverse_iterator() :iterator(NULL) {}
        reverse_iterator(Node* ptr) :iterator(ptr) {}
        reverse_iterator(const iterator& other) :iterator(other.p) {}
        reverse_iterator& operator=(const iterator& other) {iterator::p = other.p; return *this;}

        reverse_iterator operator++() {iterator::p = iterator::p->ptr_prev; return *this;}
        reverse_iterator operator++(int) {reverse_iterator clone(*this); iterator::p = iterator::p->ptr_prev; return clone;}
        reverse_iterator operator--() {iterator::p = iterator::p->ptr_next; return *this;}
        reverse_iterator operator--(int) {reverse_iterator clone(*this); iterator::p = iterator::p->ptr_next; return clone;}
    };

    iterator begin() {return iterator(ptr_first);}
    iterator end() {return iterator(NULL);}
    reverse_iterator rbegin() {return reverse_iterator(ptr_last);}
    reverse_iterator rend() {return reverse_iterator(NULL);}

    List();
    List(std::size_t n);
    List(std::size_t n, const T& val);
    List(const List<T>& other);
    List<T>& operator=(const List<T>& other);
    ~List();

    std::size_t get_size();
    std::size_t max_size();
    bool empty();

    T& front();
    T& back();

    void push_front(const T& val);
    void push_back(const T& val);
    void pop_front();
    void pop_back();

    void insert(std::size_t n, const T& val);
    void erase(std::size_t n);
    void clear();
};

template<class T>
List<T>::List() :size(0), ptr_first(NULL), ptr_last(NULL) {}
template<class T>
List<T>::List(std::size_t n)
{
    List<T>::Node* p1 = new List<T>::Node;
    p1->value = T();
    p1->ptr_next = NULL;
    p1->ptr_prev = NULL;
    ptr_first = p1;
    ptr_last = p1;

    for(int i=0; i<n-1; ++i)
    {
        Node* p2 = new Node;
        p2->value = T();
        p2->ptr_prev = p1;
        p1->ptr_next = p2;
        p1 = p2;
        if(i == n-2)
        {
            p2->ptr_next = NULL;
            ptr_last = p2;
        }
    }
    size = n;
}
template<class T>
List<T>::List(std::size_t n, const T& val)
{
    Node* p1 = new Node;
    p1->value = val;
    p1->ptr_next = NULL;
    p1->ptr_prev = NULL;
    ptr_first = p1;
    ptr_last = p1;

    for(int i=0; i<n-1; ++i)
    {
        Node* p2 = new Node;
        p2->value = val;
        p2->ptr_prev = p1;
        p1->ptr_next = p2;
        p1 = p2;
        if(i == n-2)
        {
            p2->ptr_next = NULL;
            ptr_last = p2;
        }
    }
    size = n;
}
template<class T>
List<T>::List(const List<T>& other)
{
    Node* p_iter = other.ptr_first;

    Node* p1 = new Node;
    p1->value = p_iter->value;
    p1->ptr_next = NULL;
    p1->ptr_prev = NULL;
    ptr_first = p1;
    ptr_last = p1;

    for(int i=0; i<other.size-1; ++i)
    {
        p_iter = p_iter->ptr_next;
        Node* p2 = new Node;
        p2->value = p_iter->value;
        p2->ptr_prev = p1;
        p1->ptr_next = p2;
        p1 = p2;
        if(i == other.size-2)
        {
            p2->ptr_next = NULL;
            ptr_last = p2;
        }
    }
    size = other.size;
}
template<class T>
List<T>& List<T>::operator=(const List<T>& other)
{
    if(this != &other)
    {
        Node* p_iter = other.ptr_first;

        Node* p1 = new Node;
        p1->value = p_iter->value;
        p1->ptr_next = NULL;
        p1->ptr_prev = NULL;
        ptr_first = p1;
        ptr_last = p1;

        for(int i=0; i<other.size-1; ++i)
        {
            p_iter = p_iter->ptr_next;
            Node* p2 = new Node;
            p2->value = p_iter->value;
            p2->ptr_prev = p1;
            p1->ptr_next = p2;
            p1 = p2;
            if(i == other.size-2)
            {
                p2->ptr_next = NULL;
                ptr_last = p2;
            }
        }
        size = other.size;
    }
    return *this;
}
template<class T>
List<T>::~List()
{
    if(size >= 1)
    {
        Node* p_iter = ptr_last;
        for(int i=0; i<size-1; ++i)
        {
            p_iter = p_iter->ptr_prev;
            delete p_iter->ptr_next;
        }
        delete p_iter;
        ptr_first = NULL;
        ptr_last = NULL;
        size = 0;
    }
}

template<class T>
std::size_t List<T>::get_size()
{
    return size;
}
template<class T>
std::size_t List<T>::max_size()
{
    return (std::numeric_limits<std::size_t>::max() / sizeof(T));
}
template<class T>
bool List<T>::empty()
{
    return !size;
}

template<class T>
T& List<T>::front()
{
    return ptr_first->value;
}
template<class T>
T& List<T>::back()
{
    return ptr_last->value;
}

template<class T>
void List<T>::push_front(const T& val)
{
    if(size == 0)
    {
        Node* p1 = new Node;
        p1->value = val;
        p1->ptr_prev = NULL;
        p1->ptr_next = NULL;

        ptr_first = p1;
        ptr_last = p1;
    }
    else
    {
        Node* p_new = new Node;
        p_new->value = val;
        p_new->ptr_next = ptr_first;
        p_new->ptr_prev = NULL;

        ptr_first->ptr_prev = p_new;

        ptr_first = p_new;
    }
    ++size;
}
template<class T>
void List<T>::push_back(const T& val)
{
    if(size == 0)
    {
        Node* p1 = new Node;
        p1->value = val;
        p1->ptr_prev = NULL;
        p1->ptr_next = NULL;

        ptr_first = p1;
        ptr_last = p1;
    }
    else
    {
        Node* p_new = new Node;
        p_new->value = val;
        p_new->ptr_next = NULL;
        p_new->ptr_prev = ptr_last;

        ptr_last->ptr_next = p_new;

        ptr_last = p_new;
    }
    ++size;
}
template<class T>
void List<T>::pop_front()
{
    if(size == 1)
    {
        delete ptr_first;
        ptr_first = NULL;
        ptr_last = NULL;
        size = 0;
    }
    else if(size > 1)
    {
        Node* p_temp = ptr_first->ptr_next;
        delete ptr_first;
        p_temp->ptr_prev = NULL;
        ptr_first = p_temp;
        --size;
    }
}
template<class T>
void List<T>::pop_back()
{
    if(size == 1)
    {
        delete ptr_first;
        ptr_first = NULL;
        ptr_last = NULL;
        size = 0;
    }
    else if(size > 1)
    {
        Node* p_temp = ptr_last->ptr_prev;
        delete ptr_last;
        p_temp->ptr_next = NULL;
        ptr_last = p_temp;

        --size;
    }
}

template<class T>
void List<T>::insert(std::size_t n, const T& val)
{
    if(n >= size)
        throw std::out_of_range("List: out_of_range (n >= size)");
    else
    {
        if(n == 0)
            push_front(val);
        else if(n == size)
            push_back(val);
        else
        {
            Node* p_iter = ptr_first;
            for(int i=0; i<n-1; ++i)
                p_iter = p_iter->ptr_next;
            Node* p_iter2 = p_iter->ptr_next;

            Node* p_new = new Node;
            p_new->value = val;
            p_new->ptr_next = p_iter2;
            p_new->ptr_prev = p_iter;

            p_iter->ptr_next = p_new;
            p_iter2->ptr_prev = p_new;
            ++size;
        }
    }
}
template<class T>
void List<T>::erase(std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("List: out_of_range (n >= size)");
    else
    {
        if(n == 0)
            pop_front();
        else if(n == size-1)
            pop_back();
        else
        {
            Node* p_iter = ptr_first;
            for(int i=0; i<n; ++i)
                p_iter = p_iter->ptr_next;
            Node* p_iter2 = p_iter->ptr_next;
            Node* p_iter1 = p_iter->ptr_prev;

            delete p_iter;

            p_iter1->ptr_next = p_iter2;
            p_iter2->ptr_prev = p_iter1;
            --size;
        }
    }
}
template<class T>
void List<T>::clear()
{
    if(size >= 1)
    {
        Node* p_iter = ptr_last;
        for(int i=0; i<size-1; ++i)
        {
            p_iter = p_iter->ptr_prev;
            delete p_iter->ptr_next;
        }
        delete p_iter;
        ptr_first = NULL;
        ptr_last = NULL;

        size = 0;
    }
}



#endif // LIST_H
