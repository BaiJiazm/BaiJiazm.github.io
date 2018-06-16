---
title: BAT经典面试题
date: 2018/3/8 19:33:08 
tags:
- Review
---

##### 简述C++虚函数作用及底层实现原理

*   [C/C++杂记：虚函数的实现的基本原理](https://www.cnblogs.com/malecrab/p/5572730.html)
*   [C++虚函数实现多态原理](https://www.cnblogs.com/jiayayao/p/6279483.html)

##### 内联函数、构造函数、静态成员函数可以是虚函数吗？

*   都不可以。内联函数需要在编译阶段展开，而虚函数是运行时动态绑定的，编译时无法展开； 构造函数在进行调用时还不存在父类和子类的概念，父类只会调用父类的构造函数，子类调用子类的，因此不存在动态绑定的概念；静态成员函数是以类为单位的函数，与具体对象无关，虚函数是与对象动态绑定的，因此是两个不冲突的概念；

##### 构造函数中可以调用虚函数吗？

*   可以，但是没有动态绑定的效果，父类构造函数中调用的仍然是父类版本的函数，子类中调用的仍然是子类版本的函数

##### 简述C++中虚继承的作用及底层实现原理?

*   虚继承用于解决多继承条件下的菱形继承问题，底层实现原理与编译器相关，一般通过虚基类指针实现，即各对象中只保存一份父类的对象，多继承时通过虚基类指针引用该公共对象，从而避免菱形继承中的二义性问题。
*   [C++ 多继承和虚继承的内存布局](http://www.oschina.net/translate/cpp-virtual-inheritance)

##### C++空类占用一字节

*   [C++中空类占一字节原因详解](http://blog.csdn.net/xiong452980729/article/details/71077144)

##### 结构体大小

*   [C语言的字节对齐](https://baijiazm.github.io/2018/03/05/Review/C%E8%AF%AD%E8%A8%80%E7%9A%84%E5%AD%97%E8%8A%82%E5%AF%B9%E9%BD%90/)

注意：
```C++
struct A {
	char a;
	char b;
	char c;
};
```
A的大小是3,结构体中每个成员的起始地址是`min(成员类型大小,pack)`的整数倍，整个结构体大小是最大成员大小的整数倍。

##### Struct和Class的区别

*   [Struct和Class的区别](http://blog.csdn.net/yuliu0552/article/details/6717915)

##### C++中成员函数能够同时用static和const进行修饰？

*   否，因为static表⽰示该函数为静态成员函数，为类所有；而const是用于修饰成员函数的，两者相矛盾。`[Error] static member function 'static void Test::fun()' cannot have cv-qualifier`

##### C++中包含哪几种强制类型转换？他们有什么区别和联系？

[参考答案](https://www.nowcoder.com/ta/nine-chapter/review?page=19)
[C++ 的强制类型转换](http://blog.jobbole.com/107033/)

##### 简述Linux内存分配--伙伴系统 原理

参考答案：伙伴系统，其思想是：把内存块分成不同的组(1,2,4,8,16,32....)；分配内存时找到能够满足条件 的最小的块；如果找不到，就找大的块，然后一分为2，分配一块，留一块；回收时：如果有相邻的同样大小的块，则合并

##### 使用mmap读写文件为什么比普通读写函数要快？

*   [mmap为什么比read/write快](http://blog.csdn.net/kai8wei/article/details/77980201)

##### 实现一个Memcpy函数

* [实现memcpy函数](https://my.oschina.net/renhc/blog/36345)（代码较好，基本正确）      
* [实现一个Memcpy函数](https://www.nowcoder.com/questionTerminal/9602083ec8d749999d86adf8a725b4f7)（评论中有一个按照int 字节拷贝，效率提升，值得借鉴。）

##### 100亿个整数，内存足够，如何找到中位数？内存不足，如何找到中位数？

* [参考答案](https://www.nowcoder.com/ta/nine-chapter/review?page=5)

##### 设计并实现一个LRU Cache

*   [设计并实现一个LRU Cache](https://songlee24.github.io/2015/05/10/design-LRU-Cache/)

##### 请简述智能指针原理，并实现一个简单的智能指针

* [参考答案](https://www.nowcoder.com/ta/nine-chapter/review?page=6)
* [C++ 引用计数技术及智能指针的简单实现](http://www.cnblogs.com/QG-whz/p/4777312.html)
* [C++智能指针简单剖析](http://www.cnblogs.com/lanxuezaipiao/p/4132096.html)

```C++
class SmartPointer;

template<typename T>
class Counter {
		friend class SmartPointer<T>;
	private:
		T* ptr;
		int cnt;
	public:
		Counter() {
			ptr = NULL;
			cnt = 0;
		}
		Counter(T* p) {
			ptr = p;
			cnt = 1;
		}
		~Counter() {
			delete ptr;
		}
};

template <typename T>
class SmartPointer {
	private:
		Counter* ptr_counter;
	public:
		SmartPointer(T* p) {
			ptr_counter = new Counter(p);
		}
		SmartPointer(const SmartPointer &sp) {
			ptr_counter = sp.ptr_counter;
			++ptr_count->cnt;
		}
		SmartPointer& operator=(const SmartPointer &sp) {
			++sp.ptr_counter->cnt;
			--ptr_counter->cnt;
			if (ptr_counter->cnt == 0) {
				delete ptr_counter;
			}
			ptr_counter = sp.ptr_counter;
		}
		~SmartPointer() {
			--ptr_counter->cnt;
			if (ptr_counter->cnt == 0) {
				delete ptr_counter;
			}
		}
};

```