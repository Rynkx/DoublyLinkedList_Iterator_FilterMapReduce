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
    std::clog << reduce(map(filter(IteratorBounds<dl_iter<int>, int>(stuff.begin(), nullptr),
           is_odd), same), *[](const int& x, const int& y) -> int {return x+y;}, 0); 
           //whether i make the arg *, no modifier or &, i need the * here, for some reason,
           //maybe because a lambda is not a function pointer
}
//(int (*)(const int&, const int&))