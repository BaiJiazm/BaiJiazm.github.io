---
title: final关键字用法1
date: 2018/1/30 23:52:32   
tags:
- Java
---

1.修饰数据

在java中，用final关键字修饰的变量，只能进行一次赋值操作，并且在生存期内不可以改变它的值。在针对基本类型和引用类型时，final关键字的效果存在细微差别。例子：

    class Value {
        int v;
        public Value(int v) {
            this.v = v;
        }
    }
    
    public class FinalTest {
        
        final int f1 = 1;
        final int f2;
        public FinalTest() {
            f2 = 2;
        }
    
        public static void main(String[] args) {
            final int value1 = 1;
            // value1 = 4;
            final double value2;
            value2 = 2.0;
            final Value value3 = new Value(1);
            value3.v = 4;
        }
    }

给value1赋初始值之后，我们无法再对value1的值进行修改，final关键字起到了常量的作用;从value2我们可以看到，final修饰的变量可以不在声明时赋值，即可以先声明，后赋值;value3时一个引用变量，final修饰引用变量时，只是限定了引用变量的引用不可改变，即不能将value3再次引用另一个Value对象，但是引用的对象的值是可以改变的。

引用变量的值实际上是它所引用的对象的地址，该地址的值是不可改变的，实际引用的对象实际上是不受final关键字的影响的，所以它的值是可以改变的。因为final修饰的数据的值是不可改变的，所以我们必须确保在使用前就已经对成员变量赋值了。因此对于final修饰的成员变量，我们有且只有两个地方可以给它赋值，一个是声明该成员时赋值，另一个是在构造方法中赋值。

2.修饰方法参数

编写方法时，可以在参数前面添加final关键字，它表示在整个方法中，不能改变参数的值：

    public class FinalTest {
    
        /* ... */
    
        public void finalFunc(final int i, final Value value) {
            // i = 5; 不能改变i的值
            // v = new Value(); 不能改变v的值
            value.v = 5; // 可以改变引用对象的值
        }
    }

3.修饰方法

第三种方式，即用final关键字修饰方法，它表示该方法不能被覆盖。关于private和final关键字还有一点联系，这就是类中所有的private方法都隐式地指定为是final的，由于无法在类外使用private方法，所以也就无法覆盖它。

4.修饰类

用final修饰的类是无法被继承的。

总结

final关键字是我们经常使用的关键字之一，它的主要用法有以下四种：
* 用来修饰数据，包括成员变量和局部变量，该变量只能被赋值一次且它的值无法被改变。对于成员变量来讲，我们必须在声明时或者构造方法中对它赋值；
* 用来修饰方法参数，表示在变量的生存期中它的值不能被改变；
* 修饰方法，表示该方法无法被重写；
* 修饰类，表示该类无法被继承。