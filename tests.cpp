#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "dl_list.h"
#include "filter_map_reduce.h"

TEST_CASE("constr, empty, size"){
    dl_list<int> stuff;
    CHECK(stuff.empty());
    CHECK(stuff.size() == 0);
}

TEST_CASE("iterator"){
    dl_list<int> stuff;
    stuff.push_back(2);
    stuff.push_back(3);
    auto i = stuff.begin();
    CHECK(*i == 2);
    CHECK(*(++i) == 3);
}
TEST_CASE("filtermapreduce"){
    dl_list<int> stuff;
    stuff.push_back(2);
    stuff.push_back(3);
    stuff.push_back(4);
    stuff.push_back(4);
    stuff.push_back(5);
    stuff.push_back(8);
    stuff.push_back(10);
    stuff.push_back(3);
    CHECK(reduce(map(filter(IteratorBounds<dl_iter<int>>(stuff.begin(), nullptr),
                            *[](const int& x)->bool {return x%2;}), //filter by is_odd
                        *[](const int& x)->int{return x*2;}), //map to *2
                *[](const int& x, const int& y) -> int {return x+y;}, 0) //reduce right by sum
                ==22);
           //a lambda is not a function pointer, use * before it.
}

TEST_CASE("merge"){
    dl_list<int> x;
    x.push_back(2);
    x.push_back(4);
    dl_list<int> y;
    y.push_back(3);
    y.push_back(5);
    auto merged = 
        merge<dl_iter<int>, int>(IteratorBounds<dl_iter<int>>(x.begin(), nullptr), IteratorBounds<dl_iter<int>>(y.begin(), nullptr));

    CHECK (reduce(map(merged, *[](const int& x)->int{return x%2;}), *[](const int& x, const int&y)->int{return x+y;}, 0) ==2);
}