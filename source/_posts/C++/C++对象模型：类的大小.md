---
title: C++对象模型：类的大小
date: 2018/3/5 9:23:49      
tags:
- C
- CPlusPlus
---

#### 参考以下博文：

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