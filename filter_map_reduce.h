#pragma once

//Iterators NEED arg and copy constr, maybe op=
//NEED == and !+
//NEED ++, maybe --
//and thats about it
//the filter iterator needs to know when to stop
//which means the filter iterators we pass to the map iterator are (begin, end) and (end, end)
//if we make begin() and end() we can do (auto i : stuff)
//the merge function cant induce the type it's supposed to be underneath all the iterators, not good.
//the underlying iterator carries with itself the information needed, but it's very deep and we can't really use it.
//that's why we pass the underlying type, too. it can't be induced, because args dont need the type, just the iterators.

template<typename Iterator>
struct IteratorBounds{
    Iterator begin;
    Iterator end;
    IteratorBounds(Iterator _b, Iterator _e): begin(_b), end(_e){}
    IteratorBounds(const IteratorBounds& other):
        begin(other.begin), end(other.end){}
    Iterator begin(){return begin;}
    Iterator end(){return end;}
};

template <typename Iterator, typename T>
class MapIterator{
private:
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
private: 
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
        return begin == other.begin && end  == other.end;
    }
    bool operator != (const FilterIterator& other){
        return begin != other.begin || end != other.end;
    }
};

template<typename Iterator, typename T>
class MergeIterator{
    private:
    IteratorBounds<Iterator> first;
    IteratorBounds<Iterator> second;
    bool on_first = true;
    public:
    MergeIterator(const IteratorBounds<Iterator>& _f, const IteratorBounds<Iterator>& _s): first(_f), second(_s){}
    MergeIterator(const MergeIterator<Iterator, T>& other): first(other.first), second(other.second){}
    bool operator == (const MergeIterator<Iterator, T>& other) const{
        return first.begin == other.first.begin &&
               first.end == other.first.end &&
               second.begin == other.second.begin &&
               second.end == other.second.end;
    }
    bool operator != (const MergeIterator<Iterator, T>& other) const{
        return first.begin != other.first.begin ||
               first.end != other.first.end ||
               second.begin != other.second.begin ||
               second.end != other.second.end;
    }
    MergeIterator<Iterator, T>& operator ++(){
        if(!on_first){
            ++first.begin;
            ++second.begin;
        }
        on_first = !on_first;
        return *this;
    }
    T& operator*(){
        if(on_first) return *first.begin;
        else return *second.begin;
    }
};


template <typename Iterator, typename T>
IteratorBounds<FilterIterator<Iterator, T>> filter
(IteratorBounds<Iterator> bounds, bool (*filter)(const T&)){
    FilterIterator<Iterator, T> f_begin (bounds.begin, bounds.end, filter);
    FilterIterator<Iterator, T> f_end (bounds.end, bounds.end, filter);
    return IteratorBounds<FilterIterator<Iterator, T>> (f_begin, f_end);
}


template <typename Iterator, typename T>
IteratorBounds<MapIterator<Iterator, T>> map
(IteratorBounds<Iterator> bounds, T (*mapfn)(const T&)){
    MapIterator<Iterator, T> m_begin(bounds.begin, mapfn);
    MapIterator<Iterator, T> m_end(bounds.end, mapfn);
    return IteratorBounds<MapIterator<Iterator, T>> (m_begin, m_end);
}

template<typename Iterator, typename T>
IteratorBounds<MergeIterator<Iterator,T>> merge (IteratorBounds<Iterator> first, IteratorBounds<Iterator> second){
    MergeIterator<Iterator, T> m_begin (first, second);
    MergeIterator<Iterator, T> m_end (IteratorBounds<Iterator> (first.end, first.end), IteratorBounds<Iterator>(second.end, second.end));
    return IteratorBounds<MergeIterator<Iterator, T>> (m_begin, m_end);
}

template<typename Iterator, typename T>
T reduce(IteratorBounds<Iterator> bounds,
         T (*op)(const T& first, const T& second),
         T null_value){
    T result = null_value;
    for(auto i = bounds.begin; i != bounds.end; ++i)
        result = op(result, *i);
    return result;
}
