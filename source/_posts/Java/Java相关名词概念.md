---
title: Java移位运算
date: 2018/2/2 19:05:34     
tags:
- Java
- Java基本名词
---

Java里面有很多名词，刚入门的新手肯定有很多名词不知道其意，这里简要总结，详细阐述见其它博客。

**J2EE**
全称Java 2 Enterprise Edition，是Java的一种企业版，用于企业级应用开发。

**J2SE**
全称Java 2 Standard Edition，是Java的标准版，用于标准应用开发，也是Java基础版本。

**J2ME**
全称Java 2 Micro Edition，是Java的微型版，用于手机、PDA等嵌入式开发，针对手机开发，还有专门的J2ME Wireless Toolkit免费套件提供。

总而言之，J2EE,J2SE,J2ME是java针对不同使用范畴提供不同的服务，也就是提供不同类型的类库。针对企业应用的J2EE（Java 2 Enterprise Edition）、针对普通PC应用的J2SE（Java 2 Standard Edition）和针对嵌入式设备及消费类电器的J2ME（Java 2 Micro Edition）三个版本。

**EJB**
EJB是sun的JavaEE服务器端组件模型，设计目标与核心应用是部署分布式应用程序。简单来说就是把已经编写好的程序（即：类）打包放在服务器上执行。凭借java跨平台的优势，用EJB技术部署的分布式系统可以不限于特定的平台。EJB (Enterprise JavaBean)是J2EE(javaEE)的一部分，定义了一个用于开发基于组件的企业多重应用程序的标准。

**SDK**
全称Software Develop Kit，软件开发工具包，用于帮助开发人员提高开发效率。各种不同类型的软件开发，都可以有自己的SDK。Windows有Windows SDK，DirectX 有 DirectX 9 SDK，.NET开发也有Microsoft .NET Framework SDK。JAVA开发当然也有自己的Java SDK。

**JDK**
全称Java development toolkit，相当于是Java的库函数，是编译、运行java程序的工具包，是一切java应用程序的基础,所有java应用程序是构建在这个之上的。它是一组API，也可以说是一些java Class。JDK作为Java开发工具包，主要用于构建在Java平台上运行的应用程序、Applet 和组件等，目前ORACLE官网介绍看，JDK属于SDK的一部分，JDK和SDK一起发行的。

**JRE**
全称Java Runtime Environment，Java运行环境，也就是Java平台。所有的Java程序都要在JRE下才能运行。JDK的工具也是Java程序，也需要JRE才能运行。为了保持JDK的独立性和完整性，在JDK的安装过程中，JRE也是安装的一部分。所以，目前ORACLE官网介绍看，JRE则属于JDK的一部分，如果不想开发，只是想跑跑JAVA程序，官网可以下载到单独的JRE包，但如果要开发，还是需安装上面的JDK。

**JVM**
全称Java Virtual Machine，Java虚拟机，是JRE的一部分。它是一个虚构出来的计算机，是通过在实际的计算机上仿真模拟各种计算机功能来实现的。JVM有自己完 善的硬件架构，如处理器、堆栈、寄存器等，还具有相应的指令系统。Java语言最重要的特点就是跨平台运行。使用JVM就是为了支持与操作系统无关，实现跨平台。JAVA虚拟机JVM是属于JRE的，而现在我们安装JDK时也附带安装了JRE（当然也可以单独安装JRE）。

**IoC**
控制反转（Inversion of Control，英文缩写为IoC）把创建对象的权利交给框架,是框架的重要特征，并非面向对象编程的专用术语。它包括依赖注入（Dependency Injection，简称DI）和依赖查找（Dependency Lookup）。IoC不是什么技术，而是一种设计思想。在Java开发中，Ioc意味着将你设计好的对象交给容器控制，而不是传统的在你的对象内部直接控制。

**AOP**
在软件业，AOP为Aspect Oriented Programming的缩写，意为：面向切面编程，通过预编译方式和运行期动态代理实现程序功能的统一维护的一种技术。AOP是OOP的延续，是软件开发中的一个热点，也是Spring框架中的一个重要内容，是函数式编程的一种衍生范型。利用AOP可以对业务逻辑的各个部分进行隔离，从而使得业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。

**JavaBean**
JavaBean 是一种JAVA语言写成的可重用组件。为写成JavaBean，类必须是具体的和公共的，并且具有无参数的构造器。JavaBean 通过提供符合一致性设计模式的公共方法将内部域暴露成员属性，set和get方法获取。