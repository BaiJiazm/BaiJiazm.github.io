---
title: Linux守护进程1
date: 2017/10/18 19:44:25  
tag: Linux
---

---

#### Linux守护进程1——程序前后台切换

---

*   **写test1-1.c/test1-2.c 两个程序，都是死循环操作，每5 秒打印一句话，写配套的makefile 文件，make 后可生成test1-1和test1-2 两个可执行文件**

        //test1-1.c
        #include<stdio.h>
        #include<unistd.h>
        
        #define SLEEP_TIME 5
        
        int main(){
                while(1){
                        printf("1453381\n");
                        sleep(SLEEP_TIME);
                }
                return 0;
        }

        //test1-2.c
        
        #include<stdio.h>
        #include<unistd.h>
        
        #define SLEEP_TIME 5
        
        int main(){
                while(1){
                        printf("曾鸣\n");
                        sleep(SLEEP_TIME);
                }
                return 0;
        }

    makefile ：

        cc ?= gcc
        
        srcNames :=test1-1 test1-2
        
        all :
        	@for src in $(srcNames) ; \
        	do \
        		$(cc) -o $$src "$$src".c ; \
        	done
        
        clean :
        	rm -f *.o $(srcNames)
        
        .PHONY : all clean

*   **用SecureCRT登录系统（简称控制台），然后./test1-1 运行test1-1 程序，控制台进入死循环，不再出现#或$提示符**

*   **如何将test1-1 放入后台运行？**

    程序运行时在终端按下 [Ctrl] + z，使工作转入后台，但此时工作停止，需要使用 bg 来使之运行。

*   **如何查看在后台的test1-1 程序的信息？**  

    *   jobs命令  
        功能：查看当前终端后台运行的任务  
        `jobs -l`选项可显示当前终端所有任务的PID，jobs的状态可以是running，stopped，Terminated。+ 号表示当前任务，- 号表示后一个任务。

    *   ps命令  
        功能：ps命令用于报告当前系统的进程状态  
        `ps -ef | grep "test1-1"`   
        -a：显示所有终端机下执行的程序  
        -f：显示UID,PPIP,C与STIME栏位。

    测试截图

    ![][1]

*   **如何使test1-1 重新切换到前台来？**

    fg 命令

*   **再次将test1-1 放入后台，./test1-2 运行test1-2 程序，然后将test1-2 也放入后台如何控制将test1-1/test1-2 中的某一个切换到前台来？**

    fg + 工作序号。如 fg 2 会让 test1-2 回到前台。  
    同理，如果后台有多个工作停止，则使用 bg + 工作序号可以使指定工作开始后台运行。

    `[1]- [2]+`，加号代表当前工作为`[2]`，减号代表当前工作的下一个工作为`[1]`。它们表明了工作执行的顺序，如果你使用不加序号的 fg 命令，则默认把带加号的工作提到前台。

    测试截图

    ![][2]

*   **test1-1/test1-2 的父进程标识是谁？如何查看？**

        ps -ef | grep test1-1   #查看test1-1进程信息
        pstree pid  #查看pid进程树
        ps -ef | grep pid #筛选出包含与pid有关的进程，其中第二列为pid，第三列为ppid

    测试截图

    ![][3]  
    ![][4]

*   **用SecureCRT 再登录一个控制台，用什么命令可以看到test1-1/test1-2 的信息？此时父进
程标识是谁？如何查看？**

    同上，一样的命令可以查看，原进程pid及ppid不变，此时的bash进程pid不同。

*   **当test1-1/test1-2 在后台运行时，如果CTRL+D 退出该控制台登录，在另一个控制台再查看test1-1/test1-2 的信息，此时父进程标识是谁？**

    父进程PID 是 1。

    测试截图

    ![][5]

*   **能否使test1-1/test1-2 在终端退出登录后继续运行（不是再次运行）？此时打印信息能否继续出现在新登录的终端上？**

    1.  setsid命令，用setsid来启动程序，使启动的进程在新的session中，并且终端关闭时，进程不退出。
    
            setsid ./test1-1 &
            setsid ./test1-2 &

    2.  nohup命令，被nohup启动的程序会忽略SIGHUP信号。

            nohup ./test1-1 &
            nohup ./test1-2 &

        nohup命令可以将程序以忽略挂起信号的方式运行起来，被运行的程序的输出信息将不会显示到终端。 无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 nohup.out 文件中。如果当前目录的 nohup.out 文件不可写，输出重定向到$HOME/nohup.out文件中。

    3.  对于jobs中的后台程序，通过disown命令
    
            disown -h %id   #id表示jobs中的id

    打印信息不会继续出现在新的终端上。  
    会不会出现在新的终端上取决于新的 shell 的标准输出/错误输出的文件描述符是否与 test1-1 test1-2 进程相同。在原来终端未退出时，默认情况下（没有使用重定向）它们继承了父进程（即原来 shell）的文件描述符。在原来 shell 结束后，它们继承新的父进程的文件描述符，而新的文件描述符和原来文件描述符是不同的，所以不会。

    我们可以用重定向将输出重定向为指定设备。

        tty #获取某个终端的控制台
        setsid ./test1-1 >/dev/pts/1    #/dev/pts/1 是上面的结果

*   **如何将一个正常程序直接放入后台运行？（不要通过按键切换）**

    执行程序时，在命令后加上 &，比如 `./test1-1 &`


[1]: Linux守护进程1/1.jpg
[2]: Linux守护进程1/2.jpg
[3]: Linux守护进程1/3.jpg
[4]: Linux守护进程1/4.jpg
[5]: Linux守护进程1/5.jpg
