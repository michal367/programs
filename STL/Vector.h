#ifndef VECTOR_H
#define VECTOR_H

#include <memory>
#include <iterator>
#include <stdexcept>
#include <limits>

template<class T, class Alloc = std::allocator<T> >
class Vector
{
    std::size_t size;
    std::size_t reserved;
    T* vec;
public:

    class iterator :public std::iterator<std::random_access_iterator_tag, T>
    {
    protected:
        T* p;
    public:
        iterator() :p(NULL) {}
        iterator(T* ptr) :p(ptr) {}
        iterator(const iterator& other) :p(other.p) {}
        T& operator*() {return *p;}
        T* operator->() {return p;}
        iterator& operator++() {++p; return *this;}
        iterator operator++(int) {iterator clone(*this); ++p; return clone;}
        iterator& operator--() {--p; return *this;}
        iterator operator--(int) {iterator clone(*this); --p; return clone;}
        iterator& operator+=(int n) {return iterator(p+n);}
        iterator& operator-=(int n) {return iterator(p-n);}

        iterator& operator+(int n) {return iterator(p+n);}
        iterator& operator-(int n) {return iterator(p-n);}

        T& operator[](std::size_t n) {return *(p+n);}

        bool operator<(const iterator& other) {return p<other.p;}
        bool operator>(const iterator& other) {return p>other.p;}
        bool operator<=(const iterator& other) {return p<=other.p;}
        bool operator>=(const iterator& other) {return p>=other.p;}
        bool operator==(const iterator& other) {return p==other.p;}
        bool operator!=(const iterator& other) {return p!=other.p;}
    };

    class reverse_iterator :public iterator
    {
    public:
        reverse_iterator() :iterator() {}
        reverse_iterator(T* ptr) :iterator(ptr) {}
        reverse_iterator(const reverse_iterator& other) :iterator(other.p) {}
        reverse_iterator& operator++() {--iterator::p; return *this;}
        reverse_iterator operator++(int) {iterator clone(*this); --iterator::p; return clone;}
        reverse_iterator& operator--() {++iterator::p; return *this;}
        reverse_iterator operator--(int) {iterator clone(*this); ++iterator::p; return clone;}
        reverse_iterator& operator+=(int n) {return iterator(iterator::p-n);}
        reverse_iterator& operator-=(int n) {return iterator(iterator::p+n);}

        reverse_iterator& operator+(int n) {return iterator(iterator::p-n);}
        reverse_iterator& operator-(int n) {return iterator(iterator::p+n);}

        T& operator[](std::size_t n) {return *(iterator::p+n);}

        bool operator<(const iterator& other) {return iterator::p>other.p;}
        bool operator>(const iterator& other) {return iterator::p<other.p;}
        bool operator<=(const iterator& other) {return iterator::p>=other.p;}
        bool operator>=(const iterator& other) {return iterator::p<=other.p;}
    };


    iterator begin() {return iterator(vec);}
    iterator end() {return iterator(vec+size);}
    reverse_iterator rbegin() {return reverse_iterator(vec+size-1);}
    reverse_iterator rend() {return reverse_iterator(vec-1);}

    Vector();
    explicit Vector(std::size_t n);
    explicit Vector(std::size_t n, const T& val);
    Vector(const Vector<T>& other);
    Vector<T>& operator=(const Vector<T>& other);
    ~Vector();

    void reserve(std::size_t n);
    void resize(std::size_t n);

    T& at(std::size_t n);
    T& operator[](std::size_t n);
    T& front();
    T& back();

    void push_back(const T& val);
    void pop_back();

    void insert(std::size_t n, const T& val);

    std::size_t get_size();
    std::size_t get_reserved();
    std::size_t max_size();
    bool empty();

    void erase(std::size_t n);
    void swap(Vector<T>& other);
    void clear();

    template<class U> friend bool operator==(const Vector<U>& a, const Vector<U>& b);
    template<class U> friend bool operator!=(const Vector<U>& a, const Vector<U>& b);
    template<class U> friend bool operator<(const Vector<U>& a, const Vector<U>& b);
    template<class U> friend bool operator>(const Vector<U>& a, const Vector<U>& b);
    template<class U> friend bool operator<=(const Vector<U>& a, const Vector<U>& b);
    template<class U> friend bool operator>=(const Vector<U>& a, const Vector<U>& b);
};

template<class T, class Alloc>
Vector<T,Alloc>::Vector() :size(0), reserved(0), vec(NULL) {}

template<class T, class Alloc>
Vector<T,Alloc>::Vector(std::size_t n) :size(n), reserved(n)
{
    Alloc al;
    vec = al.allocate(n);
    for(T* p=vec; p != vec+n; ++p)
        al.construct(p,T());
}

template<class T, class Alloc>
Vector<T,Alloc>::Vector(std::size_t n, const T& val) :size(n), reserved(n)
{
    Alloc al;
    vec = al.allocate(n);
    for(T* p=vec; p != vec+n; ++p)
        al.construct(p, val);
}

template<class T, class Alloc>
Vector<T,Alloc>::Vector(const Vector<T>& other)
{
    Alloc al;
    vec = al.allocate(other.reserved);
    for(int i=0; i<other.size; i++)
        vec[i] = other.vec[i];

    size = other.size;
    reserved = other.reserved;
}

template<class T, class Alloc>
Vector<T>& Vector<T,Alloc>::operator=(const Vector<T>& other)
{
    if(this != &other)
    {
        Alloc al;
        vec = al.allocate(other.reserved);
        for(int i=0; i<other.size; i++)
            vec[i] = other.vec[i];

        size = other.size;
        reserved = other.reserved;
    }
    return *this;
}

template<class T, class Alloc>
Vector<T,Alloc>::~Vector()
{
    Alloc al;
    for(T* p=vec; p < vec+size; ++p)
        al.destroy(p);
    al.deallocate(vec, reserved);
}

template<class T, class Alloc>
void Vector<T,Alloc>::reserve(std::size_t n)
{
    if(n > max_size())
        throw std::length_error("Vector: length_error (n > max_size())");
    if(n > reserved)
    {
        Alloc al;

        T* temp_vec = al.allocate(n);
        for(int i=0; i<size; i++)
            *(temp_vec+i) = *(vec+i);

        for(T* p=vec; p<vec+size; ++p)
            al.destroy(p);
        al.deallocate(vec, reserved);

        vec = temp_vec;
        reserved = n;
    }
}
template<class T, class Alloc>
void Vector<T,Alloc>::resize(std::size_t n)
{
    if(n > max_size())
        throw std::length_error("Vector: length_error (n > max_size())");
    if(n != size)
    {
        if(n > reserved)
            reserve(n);

        Alloc al;
        if(n > size)
            for(T* p=vec+size; p != vec+n; ++p)
                al.construct(p,T());
        else if(n < size)
            for(T* p=vec+n; p != vec+size; ++p)
                al.destroy(p);

        size = n;
    }
}

template<class T, class Alloc>
T& Vector<T,Alloc>::at(std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("Vector: out_of_range (n >= size)");
    return *(vec+n);
}
template<class T, class Alloc>
T& Vector<T,Alloc>::operator[](std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("Vector: out_of_range (n >= size)");
    return *(vec+n);
}
template<class T, class Alloc>
T& Vector<T,Alloc>::front()
{
    if(size == 0)
        throw std::out_of_range("Vector: out_of_range (size == 0)");
    return *vec;
}
template<class T, class Alloc>
T& Vector<T,Alloc>::back()
{
    if(size == 0)
        throw std::out_of_range("Vector: out_of_range (size == 0)");
    return *(vec+size-1);
}

template<class T, class Alloc>
void Vector<T,Alloc>::push_back(const T& val)
{
    if(reserved == 0)
        reserve(1);
    if(size+1 > reserved)
        reserve(2*reserved);

    Alloc al;
    al.construct(vec+size, val);

    ++size;
}
template<class T, class Alloc>
void Vector<T,Alloc>::pop_back()
{
    if(size > 0)
    {
        Alloc al;
        al.destroy(vec+size-1);
        --size;
    }
}

template<class T, class Alloc>
void Vector<T,Alloc>::insert(std::size_t n, const T& val)
{
    if(n >= size)
        throw std::out_of_range("Vector: out_of_range (n >= size)");
    if(n < size)
    {
        if(size+1 > reserved)
            reserve(2*reserved);
        for(T*p = vec+size-1; p != vec+n; --p)
            *p = *(p-1);
        Alloc al;
        al.destroy(vec+n);
        al.construct(vec+n, val);

        ++size;
    }
}

template<class T, class Alloc>
std::size_t Vector<T,Alloc>::get_size() {return size;}
template<class T, class Alloc>
std::size_t Vector<T,Alloc>::get_reserved() {return reserved;}
template<class T, class Alloc>
std::size_t Vector<T,Alloc>::max_size() {return std::numeric_limits<size_t>::max() / sizeof(T);}
template<class T, class Alloc>
bool Vector<T,Alloc>::empty() {return !size;}

template<class T, class Alloc>
void Vector<T,Alloc>::erase(std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("Vector: out_of_range (n >= size)");
    if(n < size)
    {
        for(T* p=vec+n; p != vec+size-1; ++p)
            *p = *(p+1);
        Alloc al;
        al.destroy(vec+size-1);

        --size;
    }
}
template<class T, class Alloc>
void Vector<T,Alloc>::swap(Vector<T>& other)
{
    T* tmp = vec;
    vec = other.vec;
    other.vec = tmp;

    std::size_t tmp2 = size;
    size = other.size;
    other.size = tmp2;

    tmp2 = reserved;
    reserved = other.reserved;
    other.reserved = tmp2;
}
template<class T, class Alloc>
void Vector<T,Alloc>::clear()
{
    Alloc al;
    for(T* p=vec; p != vec+size; ++p)
        al.destroy(p);

    size = 0;
}

template<class U>
bool operator==(const Vector<U>& a, const Vector<U>& b)
{
    if(a.size == b.size)
    {
        for(U* p1=a.vec, *p2=b.vec; p1 != (a.vec+a.size); ++p1, ++p2)
            if(*p1 != *p2)
                return 0;
        return 1;
    }
    else
        return 0;
}
template<class U>
bool operator!=(const Vector<U>& a, const Vector<U>& b)
{
    return !(operator==(a,b));
}

template<class U>
bool operator<(const Vector<U>& a, const Vector<U>& b)
{
    for(U* p1=a.vec, *p2=b.vec; p1 != (a.vec+a.size) && p2 != (b.vec+b.size); ++p1, ++p2)
    {
        if(*p1 < *p2) return 1;
        else return 0;
    }
    return 0;
}
template<class U>
bool operator>(const Vector<U>& a, const Vector<U>& b)
{
    for(U* p1=a.vec, *p2=b.vec; p1 != (a.vec+a.size) && p2 != (b.vec+b.size); ++p1, ++p2)
    {
        if(*p1 > *p2) return 1;
        else return 0;
    }
    return 0;
}
template<class U>
bool operator<=(const Vector<U>& a, const Vector<U>& b)
{
    return !(operator>(a,b));
}
template<class U>
bool operator>=(const Vector<U>& a, const Vector<U>& b)
{
    return !(operator<(a,b));
}


#endif // VECTOR_H
