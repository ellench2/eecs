// Project UID c1f28c309e55405daf00c565d57ff9ad

#include "List.h"
#include "unit_test_framework.h"

using namespace std;

// Add your test cases here

TEST(test_stub) {
    // Add test code here
    List<int> my_list;
    my_list.push_front(42);
    my_list.push_back(42);
    assert(my_list.size()==2);
    my_list.pop_back();
    my_list.pop_front();
    assert(my_list.size()==0);
    my_list.push_front(42);
    my_list.push_front(42);
    assert(my_list.size()==2);
    my_list.pop_back();
    my_list.pop_back();
    assert(my_list.size()==0);
    my_list.push_front(42);
    assert(my_list.front()==my_list.back());
    my_list.push_back(1);
    assert(my_list.front()!=my_list.back());
    my_list.pop_front();
    assert(my_list.back()==my_list.front());
    List<int>::Iterator iter = my_list.begin();
    my_list.insert(iter, 12);
    assert(my_list.size()==2);
    my_list.pop_front();
    assert(my_list.size()==1);
    List<int>::Iterator iter_1 = my_list.begin();
    my_list.insert(iter_1, 21);
    assert(my_list.size()==2);
    my_list.erase(iter_1);
    assert(my_list.size()==1);
    assert(!my_list.empty());
    List<int> my_list_1;
    my_list_1 = my_list;
    assert(my_list_1.size() == my_list.size());
    my_list_1.pop_back();
    my_list_1.push_back(1);
    my_list_1.push_back(2);
    my_list_1.push_back(3);
    my_list_1.push_back(4);
    List<int>::Iterator iter_2 = my_list_1.begin();
    assert(*iter_2==1);
    ++iter_2;
    assert(*iter_2==2);
    ++iter_2;
    assert(*iter_2==3);
    my_list_1.erase(iter_2);
    List<int>::Iterator iter_3 = my_list_1.begin();
    assert(*iter_3==1);
    ++++iter_3;
    assert(*iter_3==4);
    List<int>::Iterator iter_4 = my_list_1.begin();
    assert(*----++++iter_4 == 1);
    my_list_1.push_front(0);
    assert(*--iter_4==0);
    my_list_1.push_front(-1);
    assert(*--iter_4==-1);
    assert(*++++++iter_4==2);
    List<int>::Iterator iter_5 = my_list_1.begin();
    my_list_1.erase(iter_5);
    assert(*----iter_4==0);
    my_list_1.clear();
}

TEST_MAIN()
