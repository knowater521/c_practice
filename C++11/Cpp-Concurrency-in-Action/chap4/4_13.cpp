#include <iostream>
#include <algorithm>
#include <future>
#include <list>

// 快速排序 --- “期望”并行版
// g++ -Wall -std=c++11 4_13.cpp -o 4_13 -lpthread

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
    if(input.empty()) {
        return input;
    }
     std::list<T> result;
    // 将源list的内容部分或全部元素删除，拼插入到目的list。
    // 讲input中的从intput.begin()开始的元素插入到result.begin()开始的位置
    result.splice(result.begin(),input,input.begin());

    // 第一个数为“中间”值
    T const& pivot = *result.begin();
    // 根据 t < pivot将队列元素分成两组（true or false），返回值是第二个分组的启动迭代器
    auto divide_point =   std::partition(
                                input.begin(),
                                input.end(),
                                 [&](T const& t){return t < pivot;}
                             );

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(),input,input.begin(),divide_point);

    // 排序放在其他线程
    std::future<std::list<T> > new_lower(std::async(&parallel_quick_sort<T>,std::move(lower_part)));

    auto new_higher(parallel_quick_sort(std::move(input))); // 2
    result.splice(result.end(),new_higher);
    result.splice(result.begin(),new_lower.get());
    return result;

}


int main() {
    int myints[] = {1,123,232,23,2,4,5,16,2,77,29};
    std::list<int> lst1 (myints, myints + sizeof(myints) / sizeof(int) );
    std::cout << "The contents of lst1 are: ";
    for (std::list<int>::iterator it = lst1.begin(); it != lst1.end(); it++) {
         std::cout << *it << ' ';
    }
    std::cout << '\n';

    std::list<int> lst2 = parallel_quick_sort(lst1);
    std::cout << "The contents of lst2 are: ";
    for (std::list<int>::iterator it = lst2.begin(); it != lst2.end(); it++) {
         std::cout << *it << ' ';
    }
    std::cout << '\n';
}