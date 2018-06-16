---
title: C++ STL equal_range lower_bound upper_bound函数
date: 2018/6/15 11:03:07 
tags: 
- CPlusPlus
- STL
---

//所有容器适用`(O(log(n)))`    已序区间查找算法

`lower_bound()`        //找第一个符合的元素，返回位置迭代器,返回val出现的第一个位置

`upper_bound()`        //找最后一个符合的元素，返回位置迭代器，返回Val出现的最后一个位置的下一个位置

`equal_range()`       //找一对迭代器pair(<>,<>)，等效于lower_bound()和upper_bound().关联式容器有等效的成员函数，性能更佳

```C++
#include<iostream>  
#include<cstdio>  
#include<cstring>  
#include<vector>  
#include<algorithm>  
using namespace std;  
  
/************************************************************************************* 
std::lower_bound                 所有排序容器适用                           algorithm 
-------------------------------------------------------------------------------------- 
template <class ForwardIterator, class T> 
  ForwardIterator lower_bound ( ForwardIterator first, ForwardIterator last, 
                                const T& value ); 
 
template <class ForwardIterator, class T, class Compare> 
  ForwardIterator lower_bound ( ForwardIterator first, ForwardIterator last, 
                                const T& value, Compare comp ); 
 
//eg： 
template <class ForwardIterator, class T> 
  ForwardIterator lower_bound ( ForwardIterator first, ForwardIterator last, const T& value ) 
{ 
  ForwardIterator it; 
  iterator_traits<ForwardIterator>::difference_type count, step; 
  count = distance(first,last); 
  while (count>0) 
  { 
    it = first; step=count/2; advance (it,step); 
    if (*it<value)          // or: if (comp(*it,value)), for the comp version 
      { first=++it; count-=step+1;  } 
    else count=step; 
  } 
  return first; 
} 
*************************************************************************************/  
  
  
/************************************************************************************* 
std::upper_bound                  所有排序容器适用                           algorithm 
-------------------------------------------------------------------------------------- 
template <class ForwardIterator, class T> 
  ForwardIterator upper_bound ( ForwardIterator first, ForwardIterator last, 
                                const T& value ); 
 
template <class ForwardIterator, class T, class Compare> 
  ForwardIterator upper_bound ( ForwardIterator first, ForwardIterator last, 
                                const T& value, Compare comp ); 
 
//eg： 
template <class ForwardIterator, class T> 
  ForwardIterator upper_bound ( ForwardIterator first, ForwardIterator last, const T& value ) 
{ 
  ForwardIterator it; 
  iterator_traits<ForwardIterator>::difference_type count, step; 
  count = distance(first,last); 
  while (count>0) 
  { 
    it = first; step=count/2; advance (it,step); 
    if (!(value<*it))                 // or: if (!comp(value,*it)), for the comp version 
      { first=++it; count-=step+1;  } 
    else count=step; 
  } 
  return first; 
} 
*************************************************************************************/  
  
/************************************************************************************* 
std::equal_range                  所有排序容器适用                           algorithm 
-------------------------------------------------------------------------------------- 
template <class ForwardIterator, class T> 
  pair<ForwardIterator,ForwardIterator> 
    equal_range ( ForwardIterator first, ForwardIterator last, const T& value ); 
 
template <class ForwardIterator, class T, class Compare> 
  pair<ForwardIterator,ForwardIterator> 
    equal_range ( ForwardIterator first, ForwardIterator last, const T& value, 
                  Compare comp ); 
 
//eg： 
template <class ForwardIterator, class T> 
  pair<ForwardIterator,ForwardIterator> 
    equal_range ( ForwardIterator first, ForwardIterator last, const T& value ) 
{ 
  ForwardIterator it = lower_bound (first,last,value); 
  return make_pair ( it, upper_bound(it,last,value) ); 
} 
*************************************************************************************/  
bool mygreater (int i,int j){ return (i>j); }  
  
int main()  
{  
    int myints[] = {10,20,30,30,20,10,10,20};  
    vector<int> v(myints,myints+8);           // 10 20 30 30 20 10 10 20  
    vector<int>::iterator low,up;  
  
    sort (v.begin(), v.end());                // 10 10 10 20 20 20 30 30  
  
    cout<<"10 10 10 20 20 20 30 30\n";  
  
    low=lower_bound (v.begin(), v.end(), 20); //          ^  
    up= upper_bound (v.begin(), v.end(), 20); //                   ^  
  
    cout << "20 lower_bound at position " << int(low- v.begin()) + 1 << endl;  
    cout << "20 upper_bound at position " << int(up - v.begin()) + 1 << endl;  
  
    cout << endl;  
  
    /**-------------------------------------------------------------------------------**/  
  
    pair<vector<int>::iterator,vector<int>::iterator> bounds;  
  
    // using default comparison:  
//  sort (v.begin(), v.end());                              // 10 10 10 20 20 20 30 30  
    bounds=equal_range (v.begin(), v.end(), 20);            //          ^        ^  
    cout<<"10 10 10 20 20 20 30 30\n";  
    cout << "20 bounds at positions " << int(bounds.first - v.begin());  
    cout << " and " << int(bounds.second - v.begin()) << endl;  
  
    // using "mygreater" as comp:  
    sort (v.begin(), v.end(), mygreater);                   // 30 30 20 20 20 10 10 10  
    bounds=equal_range (v.begin(), v.end(), 20, mygreater); //       ^        ^  
  
    cout<<endl<<"30 30 20 20 20 10 10 10"<<endl;  
  
    cout << "20 bounds at positions " << int(bounds.first - v.begin());  
    cout << " and " << int(bounds.second - v.begin()) << endl;  
  
    return 0;  
}  
  
/****** 
Output: 
    10 10 10 20 20 20 30 30 
    20 lower_bound at position 4 
    20 upper_bound at position 7 
 
    10 10 10 20 20 20 30 30 
    bounds at positions 3 and 6 
 
    30 30 20 20 20 10 10 10 
    bounds at positions 2 and 5 
 
*/
```

比较好的代码样例：
* [今日头条2018校园招聘后端开发工程师(第二批)编程题 - 题解](https://blog.csdn.net/flushhip/article/details/79416715)

```C++
#include <bits/stdc++.h>

using namespace std;

bool sort_cmp(const pair<int, int> &A, const pair<int, int> &B)
{
    return A.first == B.first ? A.second < B.second :
        A.first < B.first;
}

struct find_first_cmp {
    bool operator()(const pair<int, int> &P, int k) const
    {
        return P.first < k;
    }

    bool operator()(int k, const pair<int, int> &P) const
    {
        return k < P.first;
    }
};

struct find_second_cmp {
    bool operator()(const pair<int, int> &P, int k) const
    {
        return P.second < k;
    }

    bool operator()(int k, const pair<int, int> &P) const
    {
        return k < P.second;
    }
};

int main()
{
    int n, q;
    while (EOF != scanf("%d", &n)) {
        vector<pair<int, int> > arr;
        for (int i = 0, x; i < n; cin >> x, arr.emplace_back(x, ++i)) {}
        sort(arr.begin(), arr.end(), sort_cmp);

        for (scanf("%d", &q); q--;) {
            int L, R, k;
            scanf("%d%d%d", &L, &R, &k);
            pair<vector<pair<int, int> >::iterator, vector<pair<int, int> >::iterator> sd =
                equal_range(arr.begin(), arr.end(), k, find_first_cmp{});
            printf("%d\n", upper_bound(sd.first, sd.second, R, find_second_cmp{}) -
                lower_bound(sd.first, sd.second, L, find_second_cmp{}));
        }
    }
    return 0;
}
```