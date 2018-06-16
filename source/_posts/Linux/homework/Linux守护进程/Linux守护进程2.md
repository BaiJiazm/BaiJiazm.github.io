---
title: Linux守护进程2
date: 2017/10/18 19:44:25  
tag: Linux
---

---

#### Linux守护进程2——守护进程的作用、用途、父进程标识的特点

---

*   **在02 子目录下写test2.c，写配套的makefile 文件，make 后可生成test2 可执行文件**

    基本概念如下：  
    **进程组 ：**  
    每个进程属于一个进程组  
    每个进程都有一个进程组号，该号等于该进程组组长的PID号  
    一个进程只能为它自己或子进程设置进程组ID号

    **会话期：**   
    会话期(session)是一个或多个进程组的集合  
    setsid()函数可以建立一个对话期。如果调用setsid的进程不是一个进程组的组长，此函数创建一个新的会话期：  
    (1)此进程变成该对话期的首进程  
    (2)此进程变成一个新进程组的组长进程。  
    (3)此进程没有控制终端，如果在调用setsid前，该进程有控制终端，那么与该终端的联系被解除。 如果该进程是一个进程组的组长，此函数返回错误。  
    (4)为了保证这一点，我们先调用fork()然后exit()，此时只有子进程在运行

    **编写守护进程的一般步骤步骤：**  
    （1）在父进程中执行fork并exit推出；  
    （2）在子进程中调用setsid函数创建新的会话；  
    （3）在子进程中调用chdir函数，让根目录 ”/” 成为子进程的工作目录；  
    （4）在子进程中调用umask函数，设置进程的umask为0；  
    （5）在子进程中关闭任何不需要的文件描述符  

        //test2.c
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/stat.h>
        #include <stdlib.h>
        #include <stdio.h>
        #include <syslog.h>
        #include <signal.h>
        
        int daemon_init(void){
            pid_t pid;
            if ((pid = fork()) < 0)
        		return -1;
        	else if (pid)   
        		exit(0);	/* parent exit */
        
        	/* child continues */
        	/* become session leader */
        	setsid();
        	/* change working directory */
            chdir("\n");
        	/* clear file mode creation mask */
        	umask(0);
            /* close stdin stdout stderr */
        	close(0);
        	close(1);
        	close(2);
        
        	return 0;
        }       
        
        void sig_term(int signo){
        	if(signo==SIGTERM){
        		syslog(LOG_INFO, "program terminated");
        		closelog();
        		exit(0);
        	}
        }
        
        #define SLEEP_TIME 5
        
        int main(){
        	
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        
        	//以下三行不是必须的，仅提供log信息
        	openlog("daemontest", LOG_PID, LOG_USER);
        	syslog(LOG_INFO, "program started");
        	signal(SIGTERM, sig_term);
        
        	while(1){
        		printf("1453381\n");
        		sleep(SLEEP_TIME);
        	}
        	
        	return 0;
        }

    makefile :

        CC=gcc
        CFLAGS=
        
        BIN=test2
        SRC=$(wildcard *.c)
        OBJ=$(SRC:.c=.o)
        
        $(BIN):$(OBJ)
        	$(CC) $(OBJ) -o $(BIN)
        
        .PHONY :clean
        clean:
        	rm -f $(BIN) $(OBJ)

    说明：  
    1. 在后台运行
    为避免挂起控制终端将Daemon放入后台执行。方法是在进程中调用fork使父进程终止，让Daemo在子
    进程中后台执行。
    if(pid=fork()) 
        exit(0);//是父进程，结束父进程，子进程继续 
    2. 脱离控制终端，登录会话和进程组
    登录会话和进程组之间的关系：进程属于一个进程组，进程组号（GID）就是进程组长的进程号（PID
    ）。登录会话可以包含多个进程组。这些进程组共享一个控制终端。这个控制终端通常是创建进程的
    登录终端。 控制终端，登录会话和进程组通常是从父进程继承下来的。我们的目的就是要摆脱它们，
    使之不受它们的影响。方法是在第1点的基础上，调用setsid()使进程成为会话组长： 
    setsid(); 
    说明：当进程是会话组长时setsid()调用失败。但第一点已经保证进程不是会话组长。setsid()调
    用成功后，进程成为新的会话组长和新的进程组长，并与原来的登录会话和进程组脱离。由于会话过
    程对控制终端的独占性，进程同时与控制终端脱离。
    3. 禁止进程重新打开控制终端 
    现在，进程已经成为无终端的会话组长。但它可以重新申请打开一个控制终端。可以通过使进程不再
    成为会话组长来禁止进程重新打开控制终端： 
    if(pid=fork()) 
        exit(0);//结束第一子进程，第二子进程继续（第二子进程不再是会话组长） 
    4. 关闭打开的文件描述符
    进程从创建它的父进程那里继承了打开的文件描述符。如不关闭，将会浪费系统资源，造成进程所在
    的文件系统无法卸下以及引起无法预料的错误。
    5. 改变当前工作目录 
    进程活动时，其工作目录所在的文件系统不能卸下。一般需要将工作目录改变到根目录。对于需要转
    储核心，写运行日志的进程将工作目录改变到特定目录如/tmpchdir("/") 
    6. 重设文件创建掩模 
    进程从创建它的父进程那里继承了文件创建掩模。它可能修改守护进程所创建的文件的存取位。为防
    止这一点，将文件创建掩模清除：umask(0); 

*   **用./test2 运行后直接成为守护进程**  
*   **test2 的进程标识、父进程标识是谁？如何查看？**

    进程 test2 的 ppid 为1，在 RedHat7.4 下为 systemd

        ps -ef |grep test2
        pstree 1
        ps -ef |grep -E "test2|systemd"
    
    测试截图：  

    ![][1]
    ![][2]

*   **在另一个控制台中能否查看到test2 进程的相关信息？**

    能，完全一样。  
    测试截图：  

    ![][3]

*   **当test2 在后台运行时，如果CTRL+D 退出控制台登录，test2 会怎样？**

    test2 是守护进程，即使 [CTRL] + D 退出登录，只要操作系统保持运行，则 test2 会保持运行。

*   **再次登录控制台后，test2 是否仍在运行？此时打印信息能否继续出现在新登录的终端上？**

    再次登陆，test2 仍在运行，打印信息不能出现在终端，原因上个题目已说。

*   **引申问题：如果想在进程中打印信息并保证始终能被查看到，应该如何做？**

    守护进程没有控制终端进行信息的输出，而有些情况还需要根据进程提供的信息进行系统管理和维护工作。因此Linux提供了syslogd守护进程，专门用于接受其他守护进程提供的信息记录在指定位置来解决日志记录的问题。

    在 RedHat7.4 中没有syslogd，但是有rsyslogd守护进程，rsyslog的配置文件是`/etc/rsyslog.conf`

    下面说明一下相关syslog函数：  

        // 函数说明
        #include <syslog.h>
        vodi openlog(const char *ident, int option, int facility);
        void syslog(int priority, const char *format, ...);
        void closelog(void);

        openlog函数原型如下：
        1）void openlog(const char* ident, int option, int facility);
                //打开系统日志链接，必须步骤。
        ident   :信息来源，哪个可执行程序
        option  :控制标志的参数，多个可以同时使用，用|操作合并
        facility:指定消息类型，与配置文件对应，日志会写入配置文件指定位置。
        
        2）void syslog(int priority,const char* format,....);
        priority：消息级别，与openlog中的facility可以共同决定日志写入位置。
        format  ：消息格式，再后面为格式对应的参数，类似printf的使用
         
        3）void closelog();
        关闭系统日志链接，可选步骤。

    在原来的程序中打印学号的地方添加一行：  

        syslog(LOG_INFO, "1453381\n");

    执行后，查看新增信息  

        tail -f /var/log/messages

    测试截图：

    ![][4]

[1]: Linux守护进程2/1.jpg
[2]: Linux守护进程2/2.jpg
[3]: Linux守护进程2/3.jpg
[4]: Linux守护进程2/4.jpg