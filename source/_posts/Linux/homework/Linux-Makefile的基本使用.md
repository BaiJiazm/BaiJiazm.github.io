---
title: Linux-Makefile的基本使用
date: 2017/10/8 20:59:27 
tag: Linux
---

---

### Makefile之我见

1.  **Makefile文件的作用**

    Linux内核中 Makefile 的作用是根据配置的情况，构造出需要编译的源文件列表，然后分别编译，并把目标代码链接到一起，最终形成 linux 内核二进制文件。  

    一个工程中的源文件不计其数，按类型、功能、模块分别放在若干个目录中，makefile定义了一系列的规则来指定，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为makefile就像一个Shell脚本一样，其中也可以执行操作系统的命令。makefile带来的好处就是——“自动化编译”，一旦写好，只需要一个make命令，整个工程完全自动编译，极大的提高了软件开发的效率。

    makefile文件保存了编译器和连接器的参数选项,还表述了所有源文件之间的关系(源代码文件需要的特定的包含文件,可执行文件要求包含的目标文件模块及库等).创建程序(make程序)首先读取makefile文件,然后再激活编译器,汇编器,资源编译器和连接器以便产生最后的输出,最后输出并生成的通常是可执行文件.创建程序利用内置的推理规则来激活编译器,以便通过对特定CPP文件的编译来产生特定的OBJ文件.

2.  **作业中用到的基本语法**

    **Makefile基本格式：**
        
        target ... : prerequisites ...
        command
	    ...
        ...

    target也就是一个目标文件，可以是Object File，也可以是执行文件。还可以是一个标签（Label）。
    prerequisites就是，要生成那个target所需要的文件或是目标。
    command也就是make需要执行的命令。（任意的Shell命令）
    这是一个文件的依赖关系，target这一个或多个的目标文件依赖于prerequisites中的文件，其生成规则定义在command中。说白一点就是说，prerequisites中如果有一个以上的文件比target文件要新的话，command所定义的命令就会被执行。这就是Makefile的规则。也就是Makefile中最核心的内容。

    **关于依赖关系**  
    make会一层一层的去找文件的依赖关系，最终编译出第一个目标文件。

    **关于重新编译**  
    只要任何prerequisite 比 target新，那么这个目标文件就会被下面的命令重新生成。每一个命令都会被传递到shell中，并在自己的子shell里面执行。

    **关于错误**  
    如果在寻找过程中出现错误，如文件找不到，则make会直接退出并报错。对于所定义的命令错误或者编译不成功，make是不会理会的，它只负责文件的依赖性。

    **变量的基本使用**  
    在Makefile中的定义的变量，就像是C/C++语言中的宏一样，他代表了一个文本字串，在Makefile中执行的时候其会自动原模原样地展开在所使用的地方。其与C/C++所不同的是，你可以在Makefile中改变其值。在Makefile中，变量可以使用在“目标”，“依赖目标”，“命令”或是Makefile的其它部分中。
    变量在声明时需要给予初值，而在使用时，需要给在变量名前加上“$”符号，但最好用小括号“（）”或是大括号“{}”把变量给包括起来。

    **伪目标**  
    伪目标是这样一个目标：它不代表一个真正的文件名，在执行make时可以指定这个目标来执行其所在规则定义的命令，有时我们也可以将一个伪目标称为标签。
    将一个目标声明为伪目标需要将它作为特殊目标.PHONY的依赖。如下：
        
        .PHONY : clean 
        clean: 
        rm *.o temp 

    这样目标clean就是一个伪目标，无论当前目录下是否存在clean这个文件。我们输入make clean之后。rm命令都会被执行。而且，当一个目标被声明为伪目标后，make在执行此规则时不会试图去查找隐含规则来创建这个目标。这样也提高了make的执行效率，同时我们也不用担心由于目标和文件名重名而使我们的期望失败。 

    **Makefile自动推导**  
    GNU的make很强大，它可以自动推导文件以及文件依赖关系后面的命令，于是我们就没必要去在每一个[.o]文件后都写上类似的命令，因为，我们的make会自动识别，并自己推导命令。只要make看到一个[.o]文件，它就会自动的把[.c]文件加在依赖关系中，如果make找到一个whatever.o，那么whatever.c，就会是whatever.o的依赖文件。并且 cc -c whatever.c 也会被推导出来。

    **嵌套执行make**  
    在Makefile中使用“make”作为一个命令来执行本身或者其它makefile文件。递归调用在一个村在有多级子目录的项目中非常有用。例如，当前目录下存在一个“subdir”子目录，这个子目录中有描述这个目录编译规则的makefile文件，在执行make时需要从上层目录（当前目录）开始并完成它所有子目录的编译。那么在当前目录下可以使用这样一个规则来实现对它的子目录的编译：
        
        subsystem:
   		   $(MAKE) -C subdir

    规则中“$(MAKE)”是对变量“MAKE”的引用，在make的递归调用中，需要了解变量“CURDIR”，此变量代表了make当前的工作路径。如果使用“-C”选项进入一个子目录后，此变量将被重新赋值。总之，如果在Makefile中没有对此变量进行显式的赋值操作，那么它代表make的当前工作目录。我们也可以在Makefile为这个变量赋一个新的值。此时这变量将不再代表make的工作目录。

    **Shell函数**  
    shell函数也不像其它的函数。它的参数应该就是操作系统Shell的命令。它和反引号“`”是相同的功能。这就是说，shell函数把执行操作系统命令后的输出作为函数返回。于是，我们可以用操作系统命令以及字符串处理命令awk，sed等等命令来生成一个变量，如： 

        contents := $(shell cat foo) 
        files := $(shell echo *.c) 
    
    注意，这个函数会新生成一个Shell程序来执行命令，所以你要注意其运行性能，如果你的Makefile中有一些比较复杂的规则，并大量使用了这个函数，那么对于你的系统性能是有害的。特别是Makefile的隐晦的规则可能会让你的shell函数执行的次数比你想像的多得多。

    **for循环**  
    在makefile中使用for语句，其行结束要使用“；\”do的前后用空格分开用\结尾；for语句内循环要用2个Tab。形如：
               
        @(for i in $(files); do \
        echo "$$i ";\
        done

---

### 系统概述Makefile

1.  **Make的概念**

    Make这个词，英语的意思是"制作"。Make命令直接用了这个意思，就是要做出某个文件。比如，要做出文件a.txt，就可以执行下面的命令。
        
        $ make a.txt
    
    但是，如果你真的输入这条命令，它并不会起作用。因为Make命令本身并不知道，如何做出a.txt，需要有人告诉它，如何调用其他命令完成这个目标。
    比如，假设文件 a.txt 依赖于 b.txt 和 c.txt ，是后面两个文件连接（cat命令）的产物。那么，make 需要知道下面的规则。
        
        a.txt: b.txt c.txt
        cat b.txt c.txt > a.txt
    
    也就是说，make a.txt 这条命令的背后，实际上分成两步：第一步，确认 b.txt 和 c.txt 必须已经存在，第二步使用 cat 命令 将这个两个文件合并，输出为新文件。
    像这样的规则，都写在一个叫做Makefile的文件中，Make命令依赖这个文件进行构建。Makefile文件也可以写为makefile， 或者用命令行参数指定为其他文件名。
    
        $ make -f rules.txt
        # 或者
        $ make --file=rules.txt
    
    上面代码指定make命令依据rules.txt文件中的规则，进行构建。
    总之，make只是一个根据指定的Shell命令进行构建的工具。它的规则很简单，你规定要构建哪个文件、它依赖哪些源文件，当那些文件有变动时，如何重新构建它。

2.  **Makefile文件的格式**

    2.1 **概述**  
    Makefile文件由一系列规则（rules）构成。每条规则的形式如下。
        
        <target> : <prerequisites> 
        [tab]  <commands>

    上面第一行冒号前面的部分，叫做"目标"（target），冒号后面的部分叫做"前置条件"（prerequisites）；第二行必须由一个tab键起首，后面跟着"命令"（commands）。
    "目标"是必需的，不可省略；"前置条件"和"命令"都是可选的，但是两者之中必须至少存在一个。
    每条规则就明确两件事：构建目标的前置条件是什么，以及如何构建。下面就详细讲解，每条规则的这三个组成部分。
    
    2.2 **目标（target）**  
    一个目标（target）就构成一条规则。目标通常是文件名，指明Make命令所要构建的对象，比如上文的 a.txt 。目标可以是一个文件名，也可以是多个文件名，之间用空格分隔。
    除了文件名，目标还可以是某个操作的名字，这称为"伪目标"（phony target）。
        clean:
          rm *.o

    上面代码的目标是clean，它不是文件名，而是一个操作的名字，属于"伪目标 "，作用是删除对象文件。
        
        $ make  clean

    但是，如果当前目录中，正好有一个文件叫做clean，那么这个命令不会执行。因为Make发现clean文件已经存在，就认为没有必要重新构建了，就不会执行指定的rm命令。
    为了避免这种情况，可以明确声明clean是"伪目标"，写法如下。

        .PHONY: clean
        clean:
        rm *.o temp

    声明clean是"伪目标"之后，make就不会去检查是否存在一个叫做clean的文件，而是每次运行都执行对应的命令。像.PHONY这样的内置目标名还有不少，可以查看手册。
    如果Make命令运行时没有指定目标，默认会执行Makefile文件的第一个目标。
        
        $ make
        
    上面代码执行Makefile文件的第一个目标。

    2.3 **前置条件（prerequisites）**  
    前置条件通常是一组文件名，之间用空格分隔。它指定了"目标"是否重新构建的判断标准：只要有一个前置文件不存在，或者有过更新（前置文件的last-modification时间戳比目标的时间戳新），"目标"就需要重新构建。

        result.txt: source.txt
        cp source.txt result.txt

    上面代码中，构建 result.txt 的前置条件是 source.txt 。如果当前目录中，source.txt 已经存在，那么make result.txt可以正常运行，否则必须再写一条规则，来生成 source.txt 。

        source.txt:
        echo "this is the source" > source.txt

    上面代码中，source.txt后面没有前置条件，就意味着它跟其他文件都无关，只要这个文件还不存在，每次调用make source.txt，它都会生成。
        $ make result.txt
        $ make result.txt

    上面命令连续执行两次make result.txt。第一次执行会先新建 source.txt，然后再新建 result.txt。第二次执行，Make发现 source.txt 没有变动（时间戳晚于 result.txt），就不会执行任何操作，result.txt 也不会重新生成。
    如果需要生成多个文件，往往采用下面的写法。
        
        source: file1 file2 file3

    上面代码中，source 是一个伪目标，只有三个前置文件，没有任何对应的命令。

        $ make source

    执行make source命令后，就会一次性生成 file1，file2，file3 三个文件。这比下面的写法要方便很多。

        $ make file1
        $ make file2
        $ make file3

    2.4 **命令（commands）**  
    命令（commands）表示如何更新目标文件，由一行或多行的Shell命令组成。它是构建"目标"的具体指令，它的运行结果通常就是生成目标文件。
    每行命令之前必须有一个tab键。如果想用其他键，可以用内置变量.RECIPEPREFIX声明。
    
        .RECIPEPREFIX = >
        all:
        > echo Hello, world

    上面代码用.RECIPEPREFIX指定，大于号（>）替代tab键。所以，每一行命令的起首变成了大于号，而不是tab键。
    需要注意的是，每行命令在一个单独的shell中执行。这些Shell之间没有继承关系。

        var-lost:
        export foo=bar
        echo "foo=[$$foo]"

    上面代码执行后（make var-lost），取不到foo的值。因为两行命令在两个不同的进程执行。一个解决办法是将两行命令写在一行，中间用分号分隔。

        var-kept:
        export foo=bar; echo "foo=[$$foo]"

    另一个解决办法是在换行符前加反斜杠转义。

        var-kept:
        export foo=bar; \
        echo "foo=[$$foo]"

    最后一个方法是加上.ONESHELL:命令。

        .ONESHELL:
        var-kept:
        export foo=bar; 
        echo "foo=[$$foo]"

3.  **Makefile文件的语法**

    3.1 **注释**  
    井号（#）在Makefile中表示注释。

        # 这是注释
        result.txt: source.txt
        # 这是注释
        cp source.txt result.txt # 这也是注释

    3.2 **回声（echoing）**  
    正常情况下，make会打印每条命令，然后再执行，这就叫做回声（echoing）。
    
        test:
        # 这是测试

    执行上面的规则，会得到下面的结果。

        $ make test
        # 这是测试

    在命令的前面加上@，就可以关闭回声。

        test:
        @# 这是测试

    现在再执行make test，就不会有任何输出。
    由于在构建过程中，需要了解当前在执行哪条命令，所以通常只在注释和纯显示的echo命令前面加上@。

        test:
        @# 这是测试
        @echo TODO

    3.3 **通配符**  
    通配符（wildcard）用来指定一组符合条件的文件名。Makefile 的通配符与 Bash 一致，主要有星号（*）、问号（？）和 [...] 。比如， *.o 表示所有后缀名为o的文件。

        clean:
            rm -f *.o

    3.4 **模式匹配**  
    Make命令允许对文件名，进行类似正则运算的匹配，主要用到的匹配符是%。比如，假定当前目录下有 f1.c 和 f2.c 两个源码文件，需要将它们编译为对应的对象文件。
        
        %.o: %.c

    等同于下面的写法。

        f1.o: f1.c
        f2.o: f2.c

    使用匹配符%，可以将大量同类型的文件，只用一条规则就完成构建。

    3.5 **变量和赋值符**  
    Makefile 允许使用等号自定义变量。

        txt = Hello World
        test:
            @echo $(txt)

    上面代码中，变量 txt 等于 Hello World。调用时，变量需要放在 $( ) 之中。
    调用Shell变量，需要在美元符号前，再加一个美元符号，这是因为Make命令会对美元符号转义。

        test:
            @echo $$HOME

    有时，变量的值可能指向另一个变量。

        v1 = $(v2)

    上面代码中，变量 v1 的值是另一个变量 v2。这时会产生一个问题，v1 的值到底在定义时扩展（静态扩展），还是在运行时扩展（动态扩展）？如果 v2 的值是动态的，这两种扩展方式的结果可能会差异很大。
    为了解决类似问题，Makefile一共提供了四个赋值运算符 （=、:=、？=、+=），它们的区别请看StackOverflow。

        VARIABLE = value
        # 在执行时扩展，允许递归扩展。
        
        VARIABLE := value
        # 在定义时扩展。
        
        VARIABLE ?= value
        # 只有在该变量为空时才设置值。
        
        VARIABLE += value
        # 将值追加到变量的尾端。

    3.6 **内置变量（Implicit Variables）**  
    Make命令提供一系列内置变量，比如，$(CC) 指向当前使用的编译器，$(MAKE) 指向当前使用的Make工具。这主要是为了跨平台的兼容性，详细的内置变量清单见手册。

        output:
        $(CC) -o output input.c

    3.7 **自动变量（Automatic Variables）**  
    Make命令还提供一些自动变量，它们的值与当前规则有关。主要有以下几个。

        （1）$@
        $@指代当前目标，就是Make命令当前构建的那个目标。比如，make foo的 $@ 就指代foo。
        a.txt b.txt: 
            touch $@
        等同于下面的写法。
        a.txt:
            touch a.txt
        b.txt:
            touch b.txt
        （2）$<
        $< 指代第一个前置条件。比如，规则为 t: p1 p2，那么$< 就指代p1。
        a.txt: b.txt c.txt
            cp $< $@ 
        等同于下面的写法。

4.  **Makefile 的实例**  

        （1）执行多个目标
        .PHONY: cleanall cleanobj cleandiff
        
        cleanall : cleanobj cleandiff
                rm program
        
        cleanobj :
                rm *.o
        
        cleandiff :
                rm *.diff

    上面代码可以调用不同目标，删除不同后缀名的文件，也可以调用一个目标（cleanall），删除所有指定类型的文件。

        （2）编译C语言项目
        edit : main.o kbd.o command.o display.o 
            cc -o edit main.o kbd.o command.o display.o
        
        main.o : main.c defs.h
            cc -c main.c
        kbd.o : kbd.c defs.h command.h
            cc -c kbd.c
        command.o : command.c defs.h command.h
            cc -c command.c
        display.o : display.c defs.h
            cc -c display.c
        
        clean :
             rm edit main.o kbd.o command.o display.o
        
        .PHONY: edit clean
