---
title: Linux守护进程3
date: 2017/10/18 19:44:25  
tag: Linux
---

---

#### Linux守护进程3 —— 守护进程分裂子进程

---

*   **在03 子目录下写test3-1.c，循环10 次，每隔3 秒产生一个子进程，循环完成后，自己进入
死循环操作，每5 秒打印"父进程号+自己的进程号+你的学号+main"信息即可；分裂出的子进
程要求每25 秒打印"父进程号+自己的进程号+你的学号+sub"，打印三次后子进程退出；写配
套的makefile 文件，make 后可生成test3-1 可执行文件**

        //test3-1.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define FORK_NUM 10
        #define SUB_NUM 3
        #define MAIN_SLEEP_TIME 5
        #define SUB_SLEEP_TIME 25
        
        int daemon_init(void){
            pid_t pid;
            if ((pid = fork()) < 0)
        		return -1;
        	else if (pid)   
        		exit(0);	/* parent exit */
        
        	/* child continues */
        	setsid();	/* become session leader */
            chdir("/");	/* change working directory */
        	umask(0);	/* clear file mode creation mask */
        	close(0);    /* close stdin stdout stderr */
        //	close(1);
        	close(2);
        	return 0;
        }
        
        int main(){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        
        	pid_t pid;
        	int i;
        	for (i=0; i<FORK_NUM; ++i)
        		if ((pid=fork()) < 0){
        			perror("fork error");
        			exit(errno);
        		}
        		else if(pid==0)
        			break;
        	
        	if(pid){
        		while(1){
        			printf("%5d %5d 1453381 main\n", getppid(), getpid());
        			sleep(MAIN_SLEEP_TIME);
        		}
        	}
        	else{
        		for (i=1; i<=SUB_NUM; ++i){
        			printf("%5d %5d 1453381 sub\n", getppid(), getpid());
        			sleep(SUB_SLEEP_TIME);
        		}
        	}
        	
        	return 0;
        }

    测试截图：

    ![][1]

*   **分裂出的子进程和守护进程之间的进程id 有何关联？**

    子进程的ppid为父进程的pid。

    测试截图：  

    ![][2]

*   **什么叫僵尸进程？僵尸进程的产生原因？**

    **僵尸进程：** 一个进程使用fork创建子进程，如果子进程退出，而父进程并没有调用wait或waitpid获取子进程的状态信息，那么子进程的进程描述符仍然保存在系统中。这种进程称之为僵死进程。

    **原因：** 在Linux进程的状态中，僵尸进程是非常特殊的一种，它需要它的父进程来为它收尸，如果他的父进程没安装SIGCHLD信号处理函数调用wait或waitpid（）等待子进程结束，又没有显式忽略该信号，那么它就一直保持僵尸状态，如果这时父进程结束了，那么init进程自动会接手这个子进程，为它收尸，它还是能被清除的。但是如果父进程是一个循环，不会结束，那么子进程就会一直保持僵尸状态，这就是为什么系统中有时会有很多的僵尸进程。

*   **如何杀死僵尸进程？**

    一般杀掉进程可用 `kill -9 pid`命令，但是有些僵尸进程通过此命令不行。可以通过杀掉父进程结束。

    测试截图：  

    ![][3]

*   **写test3-2.c，要求同test3-1，但是子进程退出后不能存在僵尸进程**

    **僵尸进程解决办法**

    （1）通过信号机制  
    子进程退出时向父进程发送SIGCHILD信号，父进程处理SIGCHILD信号。在信号处理函数中调用wait进行处理僵尸进程。  

        //test3-2.c 新增函数
        void sig_child(int signo){
        	int status;
        	pid_t pid;
        	while((pid=waitpid(-1, &status, WNOHANG))>0)
        		;
        }

        //在main函数第一行添加
        signal(SIGCHLD, sig_child);

    在Linux下 可以简单地将 SIGCHLD信号的操作设为SIG_IGN。

        signal(SIGCHLD,SIG_IGN);

    （2）fork两次  
    原理是将子进程成为孤儿进程，从而其的父进程变为init进程，通过init进程可以处理僵尸进程。

    基于第一种方法实现：

        //test3-2.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define FORK_NUM 10
        #define SUB_NUM 3
        #define MAIN_SLEEP_TIME 5
        #define SUB_SLEEP_TIME 25
        
        int daemon_init(void){
            pid_t pid;
            if ((pid = fork()) < 0)
        		return -1;
        	else if (pid)   
        		exit(0);	/* parent exit */
        
        	/* child continues */
        	setsid();	/* become session leader */
            chdir("/");	/* change working directory */
        	umask(0);	/* clear file mode creation mask */
        	close(0);    /* close stdin stdout stderr */
        //	close(1);
        	close(2);
        	return 0;
        }
        
        void sig_child(int signo){
        	int status;
        	pid_t pid;
        	while((pid=waitpid(-1, &status, WNOHANG))>0)
        		;
        }
        
        int main(){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        	
        	signal(SIGCHLD, sig_child);
        
        	pid_t pid;
        	int i;
        	
        	for (i=0; i<FORK_NUM; ++i)
        		if ((pid=fork()) < 0){
        			perror("fork error");
        			exit(errno);
        		}
        		else if(pid==0)
        			break;
        	
        	if(pid){
        		while(1){
        			printf("%5d %5d 1453381 main\n", getppid(), getpid());
        			sleep(MAIN_SLEEP_TIME);
        		}
        	}
        	else{
        		for (i=1; i<=SUB_NUM; ++i){
        			printf("%5d %5d 1453381 sub\n", getppid(), getpid());
        			sleep(SUB_SLEEP_TIME);
        		}
        	}
        	
        	return 0;
        }

[1]: Linux守护进程3/1.jpg
[2]: Linux守护进程3/2.jpg
[3]: Linux守护进程3/3.jpg
