#pragma once
#include <iostream>
#include <algorithm>
#include <memory>

namespace LTC
{
    template <typename _Tp>
    class vector
    {
        using allocator_type = std::allocator<_Tp>;
        using iterator = _Tp*;
        using const_iterator = const _Tp*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reference = _Tp&;
        using const_reference = const _Tp&;
        using right_reference = _Tp&&;
        size_t _size = 0;
        size_t _capacity = 0;
        // std::unique_ptr<_Tp, Deleter> base;
        _Tp *base;
        allocator_type _allocator;
        std::allocator_traits<allocator_type> alloc_traits;

    public:
        vector()
            : base(nullptr) {}
        vector(size_t size);
        vector(size_t size, const_reference _val);
        vector(const std::initializer_list<_Tp> &list);

        vector(const vector &vec);
        vector(vector &&vec);

        void swap(vector<_Tp> &vec);
        
        ~vector();

        vector& operator=(const vector &vec);

        vector& operator=(vector &&vec);

        void assign(const std::initializer_list<_Tp> &list);

        void assign(size_t n, const_reference val);

        template <typename _InputIterator>
        void assign(_InputIterator _first, _InputIterator _last);

        // 访问部分
        reference operator[](size_t pos) // 虽然函数不修改值，但没有标记为 const，因为要与下面的函数重载区分开
        {return *(begin() + pos);}

        const_reference operator[](size_t pos) const
        {return *(cbegin() + pos);}

        size_t size(void) const
        {return _size;}

        size_t capacity(void) const
        {return _capacity;}

        bool empty(void) const
        {return _size == 0;}

        reference at(size_t pos);
        
        const_reference at(size_t pos) const;

        reference front(void)
        {return *begin();}

        const_reference front(void) const
        {return *cbegin();}

        reference back(void)
        {return *rbegin();}

        const_reference back(void) const
        {return *crbegin();}

        // 迭代器部分
        iterator begin(void)
        {return base;}

        const_iterator begin(void) const
        {return base;}

        iterator end(void)
        {return base + _size;}

        const_iterator end(void) const
        {return base + _size;}

        const_iterator cbegin(void) const
        {return base;}

        const_iterator cend(void) const
        {return base + _size;}

        reverse_iterator rbegin(void)
        {return reverse_iterator(end());}

        const_reverse_iterator rbegin(void) const
        {return const_reverse_iterator(cend());}

        reverse_iterator rend(void)
        {return reverse_iterator(begin());}

        const_reverse_iterator rend(void) const
        {return const_reverse_iterator(cbegin());}

        const_reverse_iterator crbegin(void) const
        {return const_reverse_iterator(cend());}

        const_reverse_iterator crend(void) const
        {return const_reverse_iterator(cbegin());}

        void reserve(size_t capacity);

        void resize(size_t size);

        void resize(size_t size, const_reference val);

        void shrink_to_fit(void);

        void clear(void);

        iterator erase(const_iterator _pos);

        iterator erase(const_iterator _first, const_iterator _last);

        // 添加元素
        void push_back(const_reference val);

        void push_back(right_reference val);

        template <typename ...Args>
        void emplace_back(Args && ...args);

        template <typename ...Args>
        iterator emplace(const_iterator _pos, Args && ...args);

        iterator insert(const_iterator _pos, const std::initializer_list<_Tp> &list);

        iterator insert(const_iterator _pos, right_reference val);

        iterator insert(const_iterator _pos, const_reference val);

        iterator insert(const_iterator _pos, size_t n, const_reference val);

        template <typename _InputIterator>
        iterator insert(const_iterator _pos, _InputIterator _first, _InputIterator _last);

    
    private:
        void reallocate(size_t size);

        void reallocate(size_t size, const_reference val);

        /**
         * @brief 辅助在插入时扩容
         * 
         * @param temp 
         * @param n 待插入元素个数
         */
        void insertReallocte(vector &temp, size_t n);

        // 辅助插入原有元素
        void insertHelper(vector &temp, const_iterator _pos, size_t n);

    };
    
}

template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const LTC::vector<_Tp> &arr);

template <typename _Tp>
LTC::vector<_Tp>::vector(size_t size)
    :_size(size), _capacity(size)
{
    base = alloc_traits.allocate(_allocator, size);
    for (size_t i = 0; i < _capacity; ++i)
        alloc_traits.construct(_allocator, begin() + i);
}

template <typename _Tp>
LTC::vector<_Tp>::vector(size_t size, const_reference _val)
    :_size(size), _capacity(size)
{
    base = alloc_traits.allocate(_allocator, size);
    for (size_t i = 0; i < _capacity; ++i)
        alloc_traits.construct(_allocator, begin() + i, _val);
}

template <typename _Tp>
LTC::vector<_Tp>::vector(const std::initializer_list<_Tp> &list)
    : _size(list.size()), _capacity(list.size())
{
    base = alloc_traits.allocate(_allocator, size());
    for (size_t i = 0; i < _capacity; ++i)
        alloc_traits.construct(_allocator, begin() + i, *(list.begin() + i));
}

template <typename _Tp>
LTC::vector<_Tp>::vector(const vector &vec)
    : _allocator(vec._allocator), alloc_traits(vec.alloc_traits)
{
    reserve(vec.capacity());
    _size = vec.size();
    std::copy(vec.begin(), vec.end(), begin());
}

template <typename _Tp>
LTC::vector<_Tp>::vector(vector &&vec)
    : _size(vec.size()), _capacity(vec.capacity()), 
    _allocator(vec._allocator), alloc_traits(vec.alloc_traits)
{
    // 将移动后的对象置于初始状态
    vec._size = 0;
    vec._capacity = 0;
    base = std::move(vec.base);
}

template <typename _Tp>
void LTC::vector<_Tp>::swap(vector<_Tp> &vec)
{
    std::swap(_size, vec._size);
    std::swap(_capacity, vec._capacity);
    std::swap(base, vec.base);
}

template <typename _Tp>
void LTC::vector<_Tp>::assign(const std::initializer_list<_Tp> &list)
{
    assign(list.begin(), list.end());
}

template <typename _Tp>
void LTC::vector<_Tp>::assign(size_t n, const_reference val)
{
    if(capacity() < n)
    {
        for(auto it = begin(); it != end(); ++it)
            alloc_traits.destroy(_allocator, it);
        alloc_traits.deallocate(_allocator, begin(), capacity());
        base = alloc_traits.allocate(_allocator, n);
        _size = n;
        _capacity = n;
       for(auto it = begin(); it != end(); ++it)
            alloc_traits.construct(_allocator, it, val);
    }   
    else
    {
        auto it = begin();
        for(; it != begin() + n; ++it)
            *it = val;
        for(; it != end(); ++it)
            alloc_traits.destroy(_allocator, it);
        _size = n;
    }
}

template <typename _Tp>
template <typename _InputIterator>
void LTC::vector<_Tp>::assign(_InputIterator _first, _InputIterator _last)
{
    size_t n = _last - _first;
    auto src = _first;
    if(capacity() < n)
    {
        for(auto it = begin(); it != end(); ++it)
            alloc_traits.destroy(_allocator, it);
        alloc_traits.deallocate(_allocator, begin(), capacity());
        base = alloc_traits.allocate(_allocator, n);
        _size = n;
        _capacity = n;
        for(auto it = begin(); it != end(); ++it, ++src)
            alloc_traits.construct(_allocator, it, *src);
    }   
    else
    {
        auto it = begin();
        for(; it != begin() + n; ++it, ++src)
            *it = *src;
        for(; it != end(); ++it)
            alloc_traits.destroy(_allocator, it);
        _size = n;
    }
}

template <typename _Tp>
LTC::vector<_Tp>& LTC::vector<_Tp>::operator=(const vector &vec)
{
    vector temp;
    temp.reserve(vec.capacity());
    temp._size = vec.size();
    std::copy(vec.begin(), vec.end(), temp.begin());
    swap(temp);
    return *this;
}

template <typename _Tp>
LTC::vector<_Tp>& LTC::vector<_Tp>::operator=(vector &&vec)
{
    _size = vec.size();
    _capacity = vec.capacity();
    base = std::move(vec.base);
    vec._size = 0;
    vec._capacity = 0;
    return *this;
}

template <typename _Tp>
LTC::vector<_Tp>::~vector()
{   
    if(capacity() == 0)
        return;
    for(size_t i = 0; i < size(); ++i)
    {
        alloc_traits.destroy(_allocator, base + i);
    }
    alloc_traits.deallocate(_allocator, base, capacity());
}

template<typename _Tp>
inline
typename
LTC::vector<_Tp>::reference LTC::vector<_Tp>::at(size_t pos)
{
    if(pos >= _size)
        throw std::out_of_range("vector");
    return *(begin() + pos);
}

template<typename _Tp>
inline
typename
LTC::vector<_Tp>::const_reference LTC::vector<_Tp>::at(size_t pos) const
{
    if(pos >= _size)
        throw std::out_of_range("vector");
    return *(cbegin() + pos);
}

template<typename _Tp>
void LTC::vector<_Tp>::reserve(size_t capacity)
{
    if(this->capacity() < capacity)
    {
        reallocate(capacity);
    }
}

template<typename _Tp>
void LTC::vector<_Tp>::resize(size_t size)
{
    if(size <= _size)
    {
        _size = size;
        return;
    }

    vector temp;
    if(capacity() * 2 >= size)
        temp.reserve(capacity() * 2);
    else temp.reserve(size);
    temp._size = size;
    for(size_t i = this->size(); i < temp.size(); ++i)
        temp.alloc_traits.construct(temp._allocator, temp.begin() + i);
    for(auto src = begin(), dst = temp.begin(); src != end() && dst != temp.end(); ++src, ++dst)
        temp.alloc_traits.construct(temp._allocator, dst, std::move(*src));
    swap(temp);
}

template<typename _Tp>
void LTC::vector<_Tp>::resize(size_t size, const_reference val)
{
    if(size <= _size)
    {
        _size = size;
        return;
    }

    vector temp;
    if(capacity() * 2 >= size)
        temp.reserve(capacity() * 2);
    else temp.reserve(size);
    temp._size = size;
    for(size_t i = this->size(); i < temp.size(); ++i)
        temp.alloc_traits.construct(temp._allocator, temp.begin() + i, val);
    for(auto src = begin(), dst = temp.begin(); src != end() && dst != temp.end(); ++src, ++dst)
        temp.alloc_traits.construct(temp._allocator, dst, std::move(*src));
    swap(temp);
}

template <typename _Tp>
void LTC::vector<_Tp>::shrink_to_fit(void)
{
    if(capacity() > size())
        reallocate(size());
}

template <typename _Tp>
void LTC::vector<_Tp>::clear(void)
{
    vector temp;
    temp.reserve(size());
    swap(temp);
}

template <typename _Tp>
typename LTC::vector<_Tp>::iterator
LTC::vector<_Tp>::erase(const_iterator _pos)
{
    return erase(_pos, _pos + 1);
}

template <typename _Tp>
typename LTC::vector<_Tp>::iterator
LTC::vector<_Tp>::erase(const_iterator _first, const_iterator _last)
{
    size_t offset = _first - begin();
    iterator dst = begin() + offset, src = begin() + (_last - begin());
    for(; src != end(); ++dst, ++src)
        *dst = std::move(*src);
    for(; dst != end(); ++dst)
        alloc_traits.destroy(_allocator, dst);
    _size -= (_last - _first);
    return begin() + offset;
}

template <typename _Tp>
void LTC::vector<_Tp>::push_back(const_reference val)
{
    if(capacity() == 0)
    {
        vector temp(1, val);
        swap(temp);
        return ;
    }
    alloc_traits.construct(_allocator, begin() + size(), val);
    if(size() == capacity())
    {
        reallocate(capacity() == 0 ? 1 : capacity() * 2);
    }
    ++_size;
}

template <typename _Tp>
void LTC::vector<_Tp>::push_back(right_reference val)
{
    if(capacity() == 0)
    {
        vector temp(1, std::move(val));
        swap(temp);
        return ;
    }
    alloc_traits.construct(_allocator, begin() + size(), std::move(val));
    if(size() == capacity())
    {
        reallocate(capacity() == 0 ? 1 : capacity() * 2);
    }
    ++_size;
}

template <typename _Tp>
template <typename ...Args>
void LTC::vector<_Tp>::emplace_back(Args &&...args)
{
    if(size() == capacity())
    {
        reallocate(capacity() == 0 ? 1 : capacity() * 2);
    }
    alloc_traits.construct(_allocator, begin() + size(), std::forward<Args>(args)...);
    ++_size;
}

template <typename _Tp>
template <typename ...Args>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::emplace(const_iterator _pos, Args && ...args)
{
    size_t pos = _pos - begin();
    vector temp;

    insertReallocte(temp, 1);
    
    auto dst = temp.begin() + pos;
    temp.alloc_traits.construct(temp._allocator, dst, std::forward<Args>(args)...);

    insertHelper(temp, _pos, 1);

    swap(temp);
    return temp.begin() + pos;
}

template <typename _Tp>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::insert(const_iterator _pos, const std::initializer_list<_Tp> &list)
{
    return insert(_pos, list.begin(), list.end());
}

// 下面的 insert 代码有很多重复的地方，需改正
// 5.23 已略微修改
template <typename _Tp>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::insert(const_iterator _pos, right_reference val)
{
    size_t offset = _pos - begin();
    vector temp;

    insertReallocte(temp, 1);
    
    auto dst = temp.begin() + offset;
    temp.alloc_traits.construct(temp._allocator, dst, std::move(val));

    insertHelper(temp, _pos, 1);

    swap(temp);
    return begin() + offset;
}

template <typename _Tp>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::insert(const_iterator _pos, const_reference val)
{
    return insert(_pos, size_t(1), val);
}

template <typename _Tp>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::insert (const_iterator _pos, size_t n, const_reference val)
{
    size_t offset = _pos - begin();
    vector temp;

    insertReallocte(temp, n);
    
    auto dst = temp.begin() + offset;
    for(size_t i = 0; i < n; ++i)
    {
        temp.alloc_traits.construct(temp._allocator, dst + i, val);
    }

    insertHelper(temp, _pos, n);

    swap(temp);
    return begin() + offset;
}

template <typename _Tp>
template <typename _InputIterator>
typename LTC::vector<_Tp>::iterator 
LTC::vector<_Tp>::insert(const_iterator _pos, _InputIterator _first, _InputIterator _last)
{
    size_t n = _last - _first;
    size_t offset = _pos - begin();
    vector temp;

    insertReallocte(temp, n);
    
    auto dst = temp.begin() + offset;
    for(auto src = _first; src != _last; ++dst, ++src)
    {
        temp.alloc_traits.construct(temp._allocator, dst, *src);
    }

    insertHelper(temp, _pos, n);

    swap(temp);
    return begin() + offset;
}

template <typename _Tp>
void LTC::vector<_Tp>::reallocate(size_t capacity)
{
    vector temp;
    temp.base = temp.alloc_traits.allocate(_allocator, capacity);
    temp._capacity = capacity;
    temp._size = std::min(this->size(), capacity);

    for(auto src = begin(), dst = temp.begin(); src != end() && dst != temp.end(); ++src, ++dst)
        temp.alloc_traits.construct(temp._allocator, dst, std::move(*src));

    swap(temp);
}

template <typename _Tp>
void LTC::vector<_Tp>::reallocate(size_t capacity, const_reference val)
{
    reallocate(capacity);
    for(size_t i = size(); i < this->capacity(); ++i)
    {
        *(begin() + i) = val;
    }
}

template <typename _Tp>
void LTC::vector<_Tp>::insertReallocte(vector &temp, size_t n)
{
    if(capacity() < size() + n)
    {
        if(capacity() * 2 >= size() + n)
            temp.reserve(capacity() * 2);
        else temp.reserve(size() + n);
    }
    else temp.reserve(capacity());
    temp._size = size() + n;
}

template <typename _Tp>
void LTC::vector<_Tp>::insertHelper(vector &temp, const_iterator _pos, size_t n)
{
    size_t pos = _pos - begin();
    auto src = this->begin();
    for(auto dst = temp.begin(); src != _pos; ++dst, ++src)
    {
        temp.alloc_traits.construct(temp._allocator, dst, std::move(*src));
    }

    for(auto dst = temp.begin() + pos + n; src != end(); ++dst, ++src)
    {
        temp.alloc_traits.construct(temp._allocator, dst, std::move(*src));
    }
}

template <typename _Tp>
std::ostream &operator<<(std::ostream &os, const LTC::vector<_Tp> &arr)
{
    for (const auto &elem : arr)
        std::cout << elem << " ";
    return os;
}