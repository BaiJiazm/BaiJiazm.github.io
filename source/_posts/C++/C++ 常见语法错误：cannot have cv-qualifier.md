---
title: C++ 常见语法错误：cannot have cv-qualifier
date: 2018/2/22 19:22:39 
tags:
- CPlusPlus
---

在C++中CV指const和volatile

1. 非成员函数不能有CV限定，友元函数不是类的成员函数声明友元函数不能用const限定。
    friend voin fun(classname &ref)const;
    //编译器会给错误error: non-member function ‘xxxxxxxxx’ cannot have cv-qualifier

2. 静态成员函数不能有CV限定

#### 在C++中，非成员函数不能含有CV限定，即const和volatile限定

```C++
#include <iostream>
using namespace std;

double getArea() const {
	return 0.0;
}

int main(int arg,char *argv[]) {
	cout << getArea() << endl;
	return 0;
}
```

编译产生错误:
```C++
test.cpp	[Error] non-member function 'double getArea()'
cannot have cv-qualifier//不能有CV限定，非成员函数不能有cv限定符。
```

#### 在C++中，静态成员函数不能有CV限定，即const和volatile限定

```C++
#include <iostream>
using namespace std;

class CStatic {
	private:
		static int static_value;

	public:
		static int get_static_value() const;
		//当不是static方法时，可以用const进行限定。
};

int CStatic::get_static_value() const {
	return static_value;
}

int CStatic::static_value = 1;

int main(int argc,char *argv[]) {
	cout << CStatic::get_static_value()<<endl;
	return 0;
}
```

编译出现的错误:
```C++
[Error] static member function 'static int CStatic::get_static_value()' 
cannot have cv-qualifier
```
#### 友元函数同普通函数一样

```C++
#include<iostream>
#include<string>
using namespace std;

class Animal {
	public:
		friend void showRes(Animal &ref)const;
		//这行编译器会给错误non-member function ‘void showRes(Animal&)’
		//cannot have cv-qualifier

	private:
		int age;
		string name;
};

void showRes(Animal &ref) {
	ref.age=15;
	ref.name="panda";
	cout << ref.age << ref.name << endl;
}
```

编译出现的错误:
```C++
[Error] non-member function 'void showRes(Animal&)' cannot have cv-qualifier
```