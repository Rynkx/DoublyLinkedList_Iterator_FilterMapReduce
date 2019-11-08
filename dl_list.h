#pragma once

//Iterator needs to be friend to the class to access the node.
//Class needs to be a friend of the iterator for some reason.
//We put typename before accessing the struct inside the class for some reason

template<typename T>
class dl_iter;

template <typename T>
class dl_list {
private:
    struct data_box{
        data_box* prev;
        T data;
        data_box* next;
        data_box(data_box* _p, T _d, data_box * _n): prev(_p), data(_d), next(_n){}
    };
    data_box* first;
    data_box* last;
private:
    void copy(const dl_list<T>& other);
    void wipe();
public:
    dl_list();
    dl_list(const dl_list& other);
    const dl_list& operator=(const dl_list& other);
    ~dl_list();
public:
    friend class dl_iter<T>; //Why?
    dl_iter<T> begin();
    dl_iter<T> end();
    void push_back(const T& new_data);
    bool empty() const;
    unsigned size() const;
};


template<typename T>
class dl_iter{
public:
    friend class dl_list<T>; 
    dl_iter<T>(typename dl_list<T>::data_box* from); 
    T& operator*();
    bool operator==(const dl_iter<T>& other) const;
    bool operator!=(const dl_iter<T>& other) const; 
    dl_iter<T>& operator++();
    dl_iter<T>& operator--(); 
private:
    typename dl_list<T>::data_box* curr; 
};

template<typename T>
dl_list<T>::dl_list(){
    last = first = nullptr;
}

template<typename T>
bool dl_list<T>::empty()const{
    return first == nullptr;
}

template <typename T>
unsigned dl_list<T>::size() const{
    unsigned size = 0;
    for(auto i = first; i!= nullptr; i = i->next)
        size++;
    return size;
}

template<typename T>
void dl_list<T>::push_back(const T& new_data){
    if(empty()) last = first = new data_box(nullptr, new_data, nullptr);
    else last = last->next = new data_box(last, new_data, nullptr);
}

template <typename T>
void dl_list<T>::wipe(){
    if(empty()) return;
    data_box* keep = first;
    while(!empty()){
        keep = keep->next;
        delete first;
        first = keep;
    }
    last = nullptr;
}

template <typename T>
dl_list<T>::~dl_list(){
    wipe();
}
template <typename T>
dl_list<T>::dl_list (const dl_list<T>& other) {
    first = nullptr;
    last = nullptr;
    copy(other);
}

template<typename T>
const dl_list<T>& dl_list<T>::operator=(const dl_list<T>& other){
    if(this == &other) return *this;
    wipe();
    copy(other);
}

template<typename T>
void dl_list<T>::copy(const dl_list<T>& other){
    if(other.empty()) return;
    data_box* i_other = other.first;
    last = first = new data_box(nullptr, i_other->data, nullptr);
    for(i_other = i_other->next; i_other != nullptr; i_other = i_other->next)
        last = last->next = new data_box(last, i_other->data, nullptr);
}

template<typename T>
dl_iter<T>::dl_iter(typename dl_list<T>::data_box* from): curr(from){}

template<typename T>
T& dl_iter<T>::operator*(){
    return curr->data;
}

template<typename T>
dl_iter<T>& dl_iter<T>::operator++(){
    curr = curr->next;
    return (*this);
}

template<typename T>
dl_iter<T>& dl_iter<T>::operator--(){
    curr = curr->prev;
    return (*this);
}

template<typename T>
bool dl_iter<T>::operator==(const dl_iter<T>& other)const{
    return curr == other.curr;
}


template<typename T>
bool dl_iter<T>::operator!=(const dl_iter<T>& other)const{
    return curr != other.curr;
}

template<typename T>
dl_iter<T> dl_list<T>::begin(){
    return dl_iter<T>(first);
}