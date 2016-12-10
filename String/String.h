#ifndef STRING_H
#define STRING_H

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <limits>

class String
{
    char* str;
    std::size_t size;
    std::size_t reserved;
public:
    typedef char char_type;
    typedef const char_type& const_reference;
    static const std::size_t npos = -1;

    String();
    String(const char* s);
    String(const char* s, std::size_t n);
    String(const String& other);
    String& operator=(const String& other);
    String& operator=(const char* s);
    String& operator=(const char c);
    ~String();

    void reserve(std::size_t n);
    void resize(std::size_t n);
    void clear();

    std::size_t get_size();
    std::size_t get_reserved();
    static std::size_t max_size();

    char& at(std::size_t n);
    char& operator[](std::size_t n);

    void append(const String& s);
    void append(const char* s);
    void append(const char c);
    void append(char c, std::size_t n);
    void push_back(const char val);
    void operator+=(const String& s);
    void operator+=(const char* s);
    void operator+=(const char c);

    void insert(std::size_t n, const String& s);
    void insert(std::size_t n, const char* s);
    void insert(std::size_t n, char c);
    void erase(std::size_t n, std::size_t len = npos);

    const char* c_str();

    std::size_t find(std::size_t pos, const String& s);
    std::size_t find(std::size_t pos, const char* s);
    std::size_t find(std::size_t pos, char c);

    String substr(std::size_t pos, std::size_t len = npos) const;

    friend String operator+(const String& a, const String& b);

    friend std::ostream& operator<<(std::ostream& os, const String& s);
    friend std::istream& operator>>(std::istream& is, String& s);
    friend std::istream& getline(std::istream& is, String& s);
};

String::String()
    : str(NULL), size(0), reserved(0) {}
String::String(const char* s)
    : size(strlen(s)), reserved(strlen(s)+1)
{
    str = reinterpret_cast<char*>(::operator new((strlen(s)+1) * sizeof(char)));
    const char* p2 = s;
    for(char* p=str; p != str+size; ++p, ++p2)
        new(p) char_type(*p2);
    new(str+size) char_type('\0');
}
String::String(const char* s, std::size_t n)
    : size(n), reserved(n+1)
{
    str = reinterpret_cast<char*>(::operator new((n+1) * sizeof(char)));
    int i=0;
    const char* p2 = s;
    for(char* p=str; p != str+size && i<n; ++p, ++p2, ++i)
        new(p) char_type(*p2);
    new(str+size) char_type('\0');
}
String::String(const String& other)
    : size(other.size), reserved(other.reserved)
{
    str = reinterpret_cast<char*>(::operator new((other.reserved) * sizeof(char)));
    for(char* p=str, *p2 = other.str; p != str+size; ++p, ++p2)
        new(p) char_type(*p2);
    new(str+size) char_type('\0');
}
String& String::operator=(const String& other)
{
    if(this != &other)
    {
        if(size > 0)
            for(char* p=str; p != str+size+1; ++p)
                p->~char_type();
        if(reserved > 0)
            ::operator delete(str);

        size = other.size;

        str = reinterpret_cast<char*>(::operator new((other.reserved) * sizeof(char)));
        reserved = other.reserved;
        for(char* p=str, *p2 = other.str; p != str+size; ++p, ++p2)
            new(p) char_type(*p2);
        new(str+size) char_type('\0');
    }
    return *this;
}
String& String::operator=(const char* s)
{
    if(size > 0)
        for(char* p=str; p != str+size+1; ++p)
            p->~char_type();
    if(reserved > 0)
        ::operator delete(str);

    size = strlen(s);

    str = reinterpret_cast<char*>(::operator new((strlen(s)+1) * sizeof(char)));
    reserved = strlen(s)+1;
    const char* p2 = s;
    for(char* p=str; p != str+size; ++p, ++p2)
        new(p) char_type(*p2);
    new(str+size) char_type('\0');

    return *this;
}
String& String::operator=(const char c)
{
    if(size > 0)
        for(char* p=str; p != str+size+1; ++p)
            p->~char_type();
    if(reserved > 0)
        ::operator delete(str);

    size = 1;

    str = reinterpret_cast<char*>(::operator new(2 * sizeof(char)));
    reserved = 2;

    new(str) char(c);
    new(str+1) char('\0');

    return *this;
}
String::~String()
{
    if(size > 0)
        for(char* p=str; p != str+size+1; ++p)
            p->~char_type();
    if(reserved > 0)
        ::operator delete(str);
    str = NULL;
    size = 0;
    reserved = 0;
}


void String::reserve(std::size_t n)
{
    if(n > npos)
        throw std::length_error("String: length_error (n > max_size)");
    if(n > reserved)
    {
        char* str_temp = reinterpret_cast<char*>(::operator new(n * sizeof(char)));

        if(size > 0)
        {
            for(char* p = str_temp, *p2 = str; p2 != str+size+1; ++p, ++p2)
            {
                new(p) char(*p2);
                p2->~char_type();
            }
        }
        if(reserved > 0)
            ::operator delete(str);

        str = str_temp;
        reserved = n;
    }
}
void String::resize(std::size_t n)
{
    if(n > npos)
        throw std::length_error("String: length_error (n > max_size)");
    if(n > size)
    {
        if(n+1 > reserved)
            reserve(n+1);
        (str+size)->~char_type();
        for(char* p=str+size; p != str+n; ++p)
            new(p) char_type();
        new(str+n) char('\0');
        size = n;
    }
    else if(n < size)
    {
        for(char* p = str+n; p != str+size+1; ++p)
            p->~char_type();
        new(str+n) char('\0');
        size = n;
    }
}
void String::clear()
{
    if(size > 0)
    {
        for(char* p=str; p != str+size+1; ++p)
            p->~char_type();
        *(str) = '\0';
        size = 0;
    }
}

std::size_t String::get_size() {return size;}
std::size_t String::get_reserved() {return reserved;}
std::size_t String::max_size() {return npos;}

char& String::at(std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("String: out_of_range (n >= size)");
    return *(str+n);
}
char& String::operator[](std::size_t n)
{
    if(n >= size)
        throw std::out_of_range("String: out_of_range (n >= size)");
    return *(str+n);
}


void String::append(const String& s)
{
    if(size + s.size > npos)
        throw std::length_error("String: length_error (size + s.size > max_size)");
    if(reserved == 0)
    {
        reserve(s.size+1);
        for(char* p = str, *p2=s.str; p2 != s.str+s.size+1; ++p, ++p2)
            new(p) char(*p2);
        size = s.size;
    }
    else
    {
        if(size + s.size + 1 > reserved)
        reserve(size+s.size+1);

        (str+size)->~char_type();
        for(char* p = str+size, *p2 = s.str; p2 != s.str+s.size+1; ++p, ++p2)
            new(p) char(*p2);

        size += s.size;
    }

}
void String::append(const char* s)
{
    if(size + strlen(s) > npos)
        throw std::length_error("String: length_error (size + strlen(s) > max_size)");
    if(reserved == 0)
    {
        reserve(strlen(s)+1);
        const char* p2 = s;
        for(char* p = str+size; p2 != s+strlen(s); ++p, ++p2)
            new(p) char(*p2);
        new(str+strlen(s)) char('\0');
        size = strlen(s);
    }
    else
    {
        if(size + strlen(s) + 1 > reserved)
            reserve(size+strlen(s)+1);

        (str+size)->~char_type();
        const char* p2 = s;
        for(char* p = str+size; p2 != s+strlen(s)+1; ++p, ++p2)
            new(p) char(*p2);

        size += strlen(s);
    }
}
void String::append(const char c)
{
    if(size + 1 > npos)
        throw std::length_error("String: length_error (size + 1 > max_size)");
    if(reserved == 0)
    {
        reserve(2);
        new(str) char(c);
        new(str+1) char('\0');
        ++size;
    }
    else
    {
        if(size + 2 > reserved)
        reserve(size+1);

        (str+size)->~char_type();
        new(str+size) char(c);
        new(str+size+1) char('\0');

        ++size;
    }
}
void String::append(const char c, std::size_t n)
{
    if(size + n > npos)
        throw std::length_error("String: length_error (size + 1 > max_size)");
    if(reserved == 0)
    {
        reserve(n+1);
        for(char* p = str; p != str+n; ++p)
            new(p) char(c);
        new(str+n) char('\0');
        size = n;
    }
    else
    {
        if(size + n + 1 > reserved)
        reserve(size+n+1);

        (str+size)->~char_type();
        for(char*p = str+size; p != str+size+n; ++p)
            new(p) char(c);
        new(str+size+n) char('\0');

        size += n;
    }
}
void String::push_back(const char val)
{
    if(size + 1 > npos)
        throw std::length_error("String: length_error (size + 1 > max_size)");
    if(reserved == 0)
    {
        reserve(2);
        new(str) char(val);
        new(str+1) char('\0');
        ++size;
    }
    else
    {
        if(size + 2 > reserved)
            reserve(2*reserved);

        (str+size)->~char_type();
        new(str+size) char(val);
        new(str+size+1) char('\0');
        ++size;
    }
}
void String::operator+=(const String& s)
{
    append(s);
}
void String::operator+=(const char* s)
{
    append(s);
}
void String::operator+=(const char c)
{
    append(c);
}


void String::insert(std::size_t n, const String& s)
{
    if(n+s.size > npos)
        throw std::length_error("String: length_error (n + s.size > max_size)");
    if(n > size)
        throw std::out_of_range("String: out_of_range (n > size)");
    if(n == size || reserved == 0)
        append(s);
    else
    {
        if(size+s.size+1 > reserved)
            reserve(size+s.size+1);

        for(char* p = str+size; p != str+n-1; --p)
            *(p+s.size) = *p;
        for(char* p = str+n, *p2 = s.str; p2 != s.str+s.size; ++p, ++p2)
            *p = *p2;
        size += s.size;
    }
}
void String::insert(std::size_t n, const char* s)
{
    if(n+strlen(s) > npos)
        throw std::length_error("String: length_error (n + strlen(s) > max_size)");
    if(n > size)
        throw std::out_of_range("String: out_of_range (n > size)");
    if(n == size || reserved == 0)
        append(s);
    else
    {
        if(size+strlen(s)+1 > reserved)
            reserve(size+strlen(s)+1);

        for(char* p = str+size; p != str+n-1; --p)
            *(p+strlen(s)) = *p;
        const char* p2 = s;
        for(char* p = str+n; p2 != s+strlen(s); ++p, ++p2)
            *p = *p2;
        size += strlen(s);
    }
}
void String::insert(std::size_t n, char c)
{
    if(n+1 > npos)
        throw std::length_error("String: length_error (n + strlen(s) > max_size)");
    if(n > size)
        throw std::out_of_range("String: out_of_range (n > size)");
    if(n == size || reserved == 0)
        append(c);
    else
    {
        if(size+2 > reserved)
            reserve(size+2);

        for(char* p = str+size; p != str+n-1; --p)
            *(p+1) = *p;
        *(str+n) = c;
        ++size;
    }
}

void String::erase(std::size_t n, std::size_t len)
{
    if(n >= size)
        throw std::out_of_range("String: out_of_range (n >= size)");
    for(char* p=str+n; p != str+n+len && p != str+size+1; ++p)
        p->~char_type();
    if(n+len < size)
    {
        for(char* p = str+n+len; p != str+size+1; ++p)
            *(p-len) = *p;
        size -= len;
    }
    else
    {
        new(str+n) char('\0');
        size = n;
    }
}


const char* String::c_str()
{
    return str;
}


std::size_t String::find(std::size_t pos, const String& s)
{
    if(pos+s.size-1 > size)
        throw std::out_of_range("String: out_of_range (pos + s.size - 1 > size)");
    for(char* p=str+pos; p != str+size-s.size+1; ++p)
    {
        int i=0;
        for(char* p2 = s.str; p2 != s.str+s.size && *(p+i) == *p2; ++p2, ++i)
        {
            if(p2 == s.str+s.size-1)
                return p-(str+pos);
        }
    }
    return npos;
}
std::size_t String::find(std::size_t pos, const char* s)
{
    if(pos+strlen(s)-1 > size)
        throw std::out_of_range("String: out_of_range (pos + strlen(s) - 1 > size)");
    for(char* p=str+pos; p != str+size-strlen(s)+1; ++p)
    {
        int i=0;
        const char* p2 = s;
        for(; p2 != s+strlen(s) && *(p+i) == *p2; ++p2, ++i)
        {
            if(p2 == s+strlen(s)-1)
                return p-(str+pos);
        }
    }
    return npos;
}
std::size_t String::find(std::size_t pos, const char c)
{
    if(pos > size)
        throw std::out_of_range("String: out_of_range (pos > size)");
    for(char* p=str+pos; p != str+size; ++p)
    {
        if(*p == c)
            return p-(str+pos);
    }
    return npos;
}

String String::substr(std::size_t pos, std::size_t len) const
{
    if(pos >= size)
        throw std::out_of_range("String: out_of_range (pos >= size)");
    String result;
    for(char* p=str+pos; p != str+size && p != str+pos+len; ++p)
        result.push_back(*p);
    return result;
}

String operator+(const String& a, const String& b)
{
    String result(a);
    result.append(b);
    return result;
}

std::ostream& operator<<(std::ostream& os,const String& s)
{
    std::cout << s.str;
}
std::istream& operator>>(std::istream& is, String& s)
{
    std::istream::sentry cerberos(is);
    if (cerberos) {
        s.clear();
        std::istreambuf_iterator<char> it(is), end;
        if (it != end) {
            std::ctype<char> const& ctype(std::use_facet<std::ctype<char> >(is.getloc()));
            std::back_insert_iterator<String> to(s);
            std::streamsize n(0), width(is.width()? is.width(): String::max_size());
            for (; it != end && n != width && !ctype.is(std::ctype_base::space, *it); ++it, ++to) {
                *to = *it;
            }
        }
    }
    else {
        is.setstate(std::ios_base::failbit);
    }
    return is;
}
std::istream& getline(std::istream& is, String& s)
{
    char ch;
    s.clear();
    while (is.get(ch) && ch != '\n')
        s.push_back(ch);
    return is;
}

#endif
