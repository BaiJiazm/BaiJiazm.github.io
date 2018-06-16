---
title: 编程语言C++
date: 2018/3/23 0:06:43 
tags:
- C++
- Review
---

## 编程语言C++

### 类的大小

* [深度探索C++对象模型：类的大小](http://glgjing.github.io/blog/2015/03/23/shen-du-tan-suo-c-plus-plus-dui-xiang-mo-xing-:lei-de-da-xiao/)
* [C++ 空类的大小及相关继承类的大小](http://blog.csdn.net/u010921682/article/details/43529305)

```C++
//以下测试环境为GCC 32bit-Release模式

#include<iostream>
using namespace std;

class A {};
class B {};
class C : public A {
		virtual void c_fun()=0;
};
class D : public B, public C {};

int main() {
	cout<<"sizeof(A):"<<sizeof(A)<<endl;
	cout<<"sizeof(B):"<<sizeof(B)<<endl;
	cout<<"sizeof(C):"<<sizeof(C)<<endl;
	cout<<"sizeof(D):"<<sizeof(D)<<endl;
	return  0;
}

/*
 * 结果为： 1	1	4	4
 * 最后一个可能与编译器有关，可能8
 */

#include<iostream>
using namespace std;

class A {
	private:
		int data;
};

class B {
	private:
		int data;
		static int data1;
};
int B::data1=0;

//注：void main()会编译报错
main() {
	cout<<"sizeof(A) = "<< sizeof(A) <<endl;
	cout<<"sizeof(B) = "<< sizeof(B) <<endl;
}

/*
 * 结果为： 4	4
 */

#include<iostream>
using namespace std;

class A {
	public :
		A(int a) {
			a=x;
		}
		void f(int x) {
			cout<<x<<endl;
		}
		~A() {}

	private:
		int x;
		int g;
};
class B {
	public:
	private:
		int data;
		int data2;
		static int xs;
};
int B::xs=0;

int main() {
	A s(10);
	s.f(10);
	cout << "sizeof(a): "<< sizeof(A) << endl;
	cout << "sizeof(b): "<< sizeof(B) << endl;
}

/*
 * 结果为： 8	8
 */

#include<iostream>
using namespace std;

class X {};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// 上述为经典的钻石继承

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * 结果为： 1	4	4	8
 */

#include<iostream>
using namespace std;

class X {
		int a;
};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// 上述为经典的钻石继承

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * 结果为： 4	8	8	12
 */

#include<iostream>
using namespace std;

class X {
		int a;
		virtual void fun()=0;
};
class Y : public virtual X {};
class Z : public virtual X {};
class A : public Y, public Z {};
// 上述为经典的钻石继承

int main() {
	cout << "sizeof(X): " << sizeof(X) << endl;
	cout << "sizeof(Y): " << sizeof(Y) << endl;
	cout << "sizeof(Z): " << sizeof(Z) << endl;
	cout << "sizeof(A): " << sizeof(A) << endl;

	return 0;
}

/*
 * 结果为： 8	12	12	16
 */
```

### C/C++内存对齐
* [C/C++内存对齐](https://songlee24.github.io/2014/09/20/memory-alignment/)
* [C语言的字节对齐](http://blog.csdn.net/yusiguyuan/article/details/44564333)

### C++空类占用一字节
*   [C++中空类占一字节原因详解](http://blog.csdn.net/xiong452980729/article/details/71077144)

### 结构体大小
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

### C++虚函数作用及底层实现原理
*   [C/C++杂记：虚函数的实现的基本原理](https://www.cnblogs.com/malecrab/p/5572730.html)
*   [C++虚函数实现多态原理](https://www.cnblogs.com/jiayayao/p/6279483.html)

### C++中虚继承的作用及底层实现原理
*   虚继承用于解决多继承条件下的菱形继承问题，底层实现原理与编译器相关，一般通过虚基类指针实现，即各对象中只保存一份父类的对象，多继承时通过虚基类指针引用该公共对象，从而避免菱形继承中的二义性问题。
*   [C++ 多继承和虚继承的内存布局](http://www.oschina.net/translate/cpp-virtual-inheritance)

### Struct和Class的区别
*   [Struct和Class的区别](http://blog.csdn.net/yuliu0552/article/details/6717915)

### C++中包含哪几种强制类型转换？他们有什么区别和联系？
[牛客网链接](https://www.nowcoder.com/ta/nine-chapter/review?page=19)
[C++ 的强制类型转换](http://blog.jobbole.com/107033/)

### 请简述智能指针原理，并实现一个简单的智能指针

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



---
## Part2

### 内联函数、构造函数、静态成员函数可以是虚函数吗？
*   都不可以。内联函数需要在编译阶段展开，而虚函数是运行时动态绑定的，编译时无法展开； 构造函数在进行调用时还不存在父类和子类的概念，父类只会调用父类的构造函数，子类调用子类的，因此不存在动态绑定的概念；静态成员函数是以类为单位的函数，与具体对象无关，虚函数是与对象动态绑定的，因此是两个不冲突的概念；

### 构造函数中可以调用虚函数吗？
*   可以，但是没有动态绑定的效果，父类构造函数中调用的仍然是父类版本的函数，子类中调用的仍然是子类版本的函数
*   样例错题5

### 虚函数不能是内联函数
*   虚函数不能是内联函数（编译时展开，必须有实体），不能是静态函数（属于自身类，不属于对象，而虚函数要求有实体），不能是构造函数（尚未建立虚函数表）。

### C++中成员函数能够同时用static和const进行修饰？
*   否，因为static表⽰示该函数为静态成员函数，为类所有；而const是用于修饰成员函数的，两者相矛盾。`[Error] static member function 'static void Test::fun()' cannot have cv-qualifier`

### C++ 内联函数
* [C++ 内联函数](http://www.cnblogs.com/QG-whz/p/4641479.html)

### C/C++参数传递顺序
* [C或C++为什么被设计成是从右向左处理参数?](http://blog.sina.com.cn/s/blog_5ed3e6210100ceiu.html)

### volatile有什么作用？
*   volatile定义变量的值是易变的，每次用到这个变量的值的时候都要去重新读取这个变量的值，而不是读寄存器内的备份。
*   多线程中被几个任务共享的变量需要定义为volatile类型。

### delete和delete[]区别？
*   delete只会调用一次析构函数。
*   delete[]会调用数组中每个元素的析构函数。

### 面向对象三大特性？
*   封装性：数据和代码捆绑在一起，避免外界干扰和不确定性访问。
*   继承性：让某种类型对象获得另一个类型对象的属性和方法。
*   多态性：同一事物表现出不同事物的能力，即向不同对象发送同一消息，不同的对象在接收时会产生不同的行为（重载实现编译时多态，虚函数实现运行时多态）。

### 构造函数能否为虚函数，析构函数呢？
*   析构函数：
    *   析构函数可以为虚函数，并且一般情况下基类析构函数要定义为虚函数。
    *   只有在基类析构函数定义为虚函数时，调用操作符delete销毁指向对象的基类指针时，才能准确调用派生类的析构函数（从该级向上按序调用虚函数），才能准确销毁数据。
    *   析构函数可以是纯虚函数，含有纯虚函数的类是抽象类，此时不能被实例化。但派生类中可以根据自身需求重新改写基类中的纯虚函数。
*   构造函数：
    *   构造函数不能定义为虚函数，不仅如此，构造函数中还不能调用虚函数。因为那样实际执行的是父类对应的函数，因为自己还没有构造好（构造顺序先基类再派生类）。

### C语言0长度数组(可变数组/柔性数组)
*   [C语言0长度数组(可变数组/柔性数组)详解](https://blog.csdn.net/gatieme/article/details/64131322)

### 有哪几种情况只能用intialization list而不能用assignment？
*   当类中含有const成员变量；基类无默认构造函数时，有参的构造函数都需要初始化表；当类中含有reference成员变量。

### 关于const int* 和int const *及int * const
*   [关于C++ const 的全面总结](https://blog.csdn.net/Eric_Jo/article/details/4138548)



---
## Part3

### static作用是什么？在C和C++中有何区别？
*   static可以修饰局部变量（静态局部变量）、全局变量（静态全局变量）和函数，被修饰的变量存储位置在静态区。对于静态局部变量，相对于一般局部变量其生命周期长，直到程序运行结束而非函数调用结束，且只在第一次被调用时定义；对于静态全局变量，相对于全局变量其可见范围被缩小，只能在本文件中可见；修饰函数时作用和修饰全局变量相同，都是为了限定访问域。
*   C++的static除了上述两种用途，还可以修饰类成员（静态成员变量和静态成员函数），静态成员变量和静态成员函数不属于任何一个对象，是所有类实例所共有。
*   static的数据记忆性可以满足函数在不同调用期的通信，也可以满足同一个类的多个实例间的通信。
*   未初始化时，static变量默认值为0。

### malloc和new的区别？
*   malloc和free是标准库函数，支持覆盖；new和delete是运算符，并且支持重载。
*   malloc仅仅分配内存空间，free仅仅回收空间，不具备调用构造函数和析构函数功能，用malloc分配空间存储类的对象存在风险；new和delete除了分配回收功能外，还会调用构造函数和析构函数。
*   malloc和free返回的是void类型指针（必须进行类型转换），new和delete返回的是具体类型指针。

### 指针和引用区别？
*   引用只是别名，不占用具体存储空间，只有声明没有定义；指针是具体变量，需要占用存储空间。
*   引用在声明时必须初始化为另一变量，一旦出现必须为typename refname &varname形式；指针声明和定义可以分开，可以先只声明指针变量而不初始化，等用到时再指向具体变量。
*   引用一旦初始化之后就不可以再改变（变量可以被引用为多次，但引用只能作为一个变量引用）；指针变量可以重新指向别的变量。
*   不存在指向空值的引用，必须有具体实体；但是存在指向空值的指针。

### 构造函数调用顺序，析构函数呢？
*   基类的构造函数：如果有多个基类，先调用纵向上最上层基类构造函数，如果横向继承了多个类，调用顺序为派生表从左到右顺序。
*   成员类对象的构造函数：如果类的变量中包含其他类（类的组合），需要在调用本类构造函数前先调用成员类对象的构造函数，调用顺序遵照在类中被声明的顺序。
*   派生类的构造函数。
*   析构函数与之相反。

### 虚函数和纯虚函数区别？
*   虚函数是为了实现动态编联产生的，目的是通过基类类型的指针指向不同对象时，自动调用相应的、和基类同名的函数（使用同一种调用形式，既能调用派生类又能调用基类的同名函数）。虚函数需要在基类中加上virtual修饰符修饰，因为virtual会被隐式继承，所以子类中相同函数都是虚函数。当一个成员函数被声明为虚函数之后，其派生类中同名函数自动成为虚函数，在派生类中重新定义此函数时要求函数名、返回值类型、参数个数和类型全部与基类函数相同。
*   纯虚函数只是相当于一个接口名，但含有纯虚函数的类不能够实例化。

### 覆盖、重载和隐藏的区别？
*   覆盖是派生类中重新定义的函数，其函数名、参数列表（个数、类型和顺序）、返回值类型和父类完全相同，只有函数体有区别。派生类虽然继承了基类的同名函数，但用派生类对象调用该函数时会根据对象类型调用相应的函数。覆盖只能发生在类的成员函数中。
*   隐藏是指派生类函数屏蔽了与其同名的函数，这里仅要求基类和派生类函数同名即可。其他状态同覆盖。可以说隐藏比覆盖涵盖的范围更宽泛，毕竟参数不加限定。
*   重载是具有相同函数名但参数列表不同（个数、类型或顺序）的两个函数（不关心返回值），当调用函数时根据传递的参数列表来确定具体调用哪个函数。重载可以是同一个类的成员函数也可以是类外函数。

### 在main执行之前执行的代码可能是什么？
*   全局对象的构造函数。

### 构造函数和析构函数调用时机？
*   全局范围中的对象：构造函数在所有函数调用之前执行，在主函数执行完调用析构函数。
*   局部自动对象：建立对象时调用构造函数，函数结束时调用析构函数。
*   动态分配的对象：建立对象时调用构造函数，调用释放时调用析构函数。
*   静态局部变量对象：建立时调用一次构造函数，主函数结束时调用析构函数。



---
### 错题1
```C++
题目：
    在C++中，
    const int i = 0;
    int *j = (int *) &i;
    *j = 1;
    printf("%d, %d", i, *j);
    输出是多少？

答案:
    0
    1

解答:
    考察C++常量折叠。
    1. const变量放在编译器的符号表中，计算时编译器直接从表中取值，省去了访问内存的时间，从而达到了优化。
    2. 结论，const变量通过取地址方式可以修改该地址存储的数据值，但不能修改常量的值。
```

### 错题2
```C++
题目：
    假设在一个32位little endian的机器上运行下面的程序，结果是多少？
    #include <stdio.h>
    int main(){
        long long a = 1, b = 2, c = 3;
        printf("%d %d %d\n", a, b, c);
        return 0;
    }

答案:
    1
    0
    2

解答:
    考察小端法及printf输出控制符。
    1. long long占8字节。
    2. 小端表示，低字节在低位，最低4字节为1，接下来四字节为高位部分的0，再接下来4字节为第二个数低位的2。
    3. printf的控制符相当于分配好待打印容器大小，这里"%d %d %d"就分配了12字节，分别装入三个8字节元素，只装入一半。
    4. 换为cout为1 2 3
```

### 错题3
```C++
题目：
    请选择下列程序的运行结果:
    #include<iostream>
    using namespace std;
    class B0{
    public:
        virtual void display(){
            cout << "B0::display0" << endl;
        }
    };
    class B1:public B0{
    public:
        void display(){
            cout << "B1::display0" << endl;
        }
    };
    class D1: public B1{
    public:
        void display(){
            cout << "D1::display0" << endl;
        }
    };
    void fun(B0 ptr){
        ptr.display();
    }
    int main(){
        B0 b0;
        B1 b1;
        D1 d1;
        fun(b0);
        fun(b1);
        fun(d1);
    }

答案:
    B0::display0
    B0::display0
    B0::display0
解答:
    1. 这里传递的是对象本身而非指针，对象被直接转为基类对象，调用基类的函数。
    2. 如果要实现虚函数动态绑定需要将B0 ptr改为B0* ptr，ptr->display()。
    3. 对象的形参传递需要先使用拷贝构造函数（默认）生成B0类型的临时变量，只拷贝基类部分数据（只有指向基类虚函数表的虚函数指针，赋值兼容规则）。
```

### 错题4
```C++
题目：
    下面代码输出什么：
    #include<stdio.h>
    int main( ){
        unsigned int a = 6;
        int b = -20;
        (a + b > 6) ? printf(">6") : printf("<=6");
        return 0;
    }

答案:
    >6

解答:
    考察强制类型转换。
    1. 必须先明确：int与unsigned相加，int -> unsigned int。
    2. int b = -20，首位位"1"，用无符号型表示是非常大的正整数。
```

### 错题5
```C++
题目：
    class Base{
    public:
        Base(){
            Init();
        }
        virtual void Init(){
            printf("Base Init\n");
        }
        void func(){
            printf("Base func\n");
        }
    };
    class Derived: public Base{
    public:
        virtual void Init(){
            printf("Derived Init\n");
        }
        void func(){
            printf("Derived func\n");
        }
    };
    int main(){
        Derived d;
        ((Base *)&d)->func();
        return 0;
    }

答案：
    Base Init
    Base func

解答：
    考察虚函数。
    1. 类Derived继承自Base，先调用基类构造函数Base()，再调用基类的init()，输出Base Init。
        构造子类对象，基类中不会调用子类的虚函数：
            基类构造函数 -> 子类构造函数
            子类还没有构造，还没有初始化，属于未初始化对象
            基类不会去调用子类虚函数（哪怕子类中确实声明为虚函数）
    2. 虽然 ((Base *)&d)->func()是虚函数调用的样子，但func()跟本没有被定义为虚函数，基类指针访问基类的func()。
```


---
## STL部分

### STL中的迭代器失效问题
*   [STL迭代器及迭代器失效问题](https://blog.csdn.net/tian_110/article/details/43190627)