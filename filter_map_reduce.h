#pragma once

template<typename Iterator, typename T>
struct IteratorBounds{
    Iterator begin;
    Iterator end;
    IteratorBounds(Iterator _b, Iterator _e): begin(_b), end(_e){}
    IteratorBounds(const IteratorBounds& other):
        begin(other.begin), end(other.end){}
};

template <typename Iterator, typename T>
class MapIterator{
public:
    Iterator begin;
    T (*map) (const T& element);
public:
    T operator*(){
        return map(*begin);
    }
    MapIterator(const Iterator& _b,
     T (*_m)(const T& element)): begin(_b), map(_m){}
    MapIterator& operator++ (){
        ++begin;
        return *this;
    }
    bool operator == (const MapIterator& other){
        return begin == other.begin;
    }
    bool operator != (const MapIterator& other){
        return begin != other.begin;
    }
};

template <typename Iterator, typename T>
class FilterIterator{
public: // ??
    Iterator begin;
    Iterator end;
    bool (*filter) (const T& element);
public:
    T& operator*(){
        return *begin;
    }
    FilterIterator(const Iterator& _b, const Iterator& _e,
    bool (*_f)(const T& element)): begin(_b), end(_e), filter(_f){
        while (begin != end && !filter(*begin))
            ++begin;
    }
    FilterIterator& operator++ (){
        ++begin;
        while (begin != end && !filter(*begin))
            ++begin;
        return *this;
    }
    bool operator == (const FilterIterator& other){
        return begin == other.begin;
    }
    bool operator != (const FilterIterator& other){
        return begin != other.begin;
    }
};

template <typename Iterator, typename T>
IteratorBounds<FilterIterator<Iterator, T>, T> filter
(IteratorBounds<Iterator, T> bounds, bool (*filter)(const T&)){
    FilterIterator<Iterator, T> f_begin (bounds.begin, bounds.end, filter);
    FilterIterator<Iterator, T> f_end (bounds.end, bounds.end, filter);
    return IteratorBounds<FilterIterator<Iterator, T>, T> (f_begin, f_end);
}


template <typename Iterator, typename T>
IteratorBounds<MapIterator<Iterator, T>, T> map
(IteratorBounds<Iterator, T> bounds, T (*mapfn)(const T&)){
    MapIterator<Iterator, T> m_begin(bounds.begin, mapfn);
    MapIterator<Iterator, T> m_end(bounds.end, mapfn);
    return IteratorBounds<MapIterator<Iterator, T>, T> (m_begin, m_end);
}

template<typename Iterator, typename T>
T reduce(IteratorBounds<Iterator, T> bounds,
         T (*op)(const T& first, const T& second),
         T null_value){
    T result = null_value;
    for(auto i = bounds.begin; i != bounds.end; ++i)
        result = op(result, *i);
    return result;
}

bool is_odd(const int& x){
    return x%2;
}
int times2(const int& x){
    return 2*x;
}
int same(const int& x){
    return x;
}
int plus(const int& x, const int& y){
    return x+y;
}
int times(const int& x, const int& y){
    return x*y;
}