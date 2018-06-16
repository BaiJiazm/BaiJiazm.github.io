---
title: Linux守护进程4
date: 2017/10/18 19:44:25  
tag: Linux
---

---

#### Linux守护进程4 -- 杀掉守护进程及其子进程

---

*   **在04 子目录下写test4-1.c，循环10 次，每隔3 秒产生一个子进程，循环完成后，自己进入
死循环操作，每5 秒打印"父进程号+自己的进程号+你的学号+main"信息即可；分裂出的子进
程进入死循环操作，要求每15 秒打印"父进程号+自己的进程号+你的学号+sub"；写配套的
makefile 文件，make 后可生成test4-1 可执行文件**

        //test4-1.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define FORK_NUM 10
        #define FORK_SLEEP_TIME 3
        #define MAIN_SLEEP_TIME 5
        #define SUB_SLEEP_TIME 15
        
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
        			//子进程退出
        			break;
        		else 
        			//父进程间隔3s
        			sleep(FORK_SLEEP_TIME);	
        	
        	if(pid){	//父进程
        		while(1){
        			printf("%d %d 1453381 main\n", getppid(), getpid());
        			sleep(MAIN_SLEEP_TIME);
        		}
        	}
        	else{		//子进程
        		while(1){
        			printf("%d %d 1453381 sub\n", getppid(), getpid());
        			sleep(SUB_SLEEP_TIME);
        		}
        	}
        	
        	return 0;
        }

    makefile :

        CC ?= gcc
        CFLAGS =
        
        srcNames :=test4-1 test4-2
        
        all :
        	@for src in $(srcNames) ; \
        	do \
        		$(CC) $(CFLAGS) -o $$src "$$src".c ; \
        	done
        
        clean :
        	rm -f *.o $(srcNames)
        
        .PHONY : all clean

    测试截图：

    ![][0]

*   **如何杀死test4-1 分裂出来的一个子进程？**

        kill -9 pid

*   **如何快速杀死test4-1 分裂出来的全部子进程？**

        ps -ef |grep test4-1 |grep -v grep|cut -c 9-15|xargs kill -9
        
        # 批量杀死包含关键字"test4-1"的进程
        # "ps -ef" ——查看所有进程
        # "grep test4-1" ——列出所有含有关键字"test4-1"的进程
        # "grep -v grep" ——在列出的进程中去除含有关键字"grep"的进程
        # "cut -c 9-15" ——截取输入行的第9个字符到第15个字符，而这正好是进程号PID
        # "xargs kill -9" ——xargs 命令是用来把前面命令的输出结果（PID）作为"kill -9"
          命令的参数，并执行该命令。"kill -9"会强行杀掉指定进程。

    测试截图：

    ![][1]

*   **如果杀死test4-1，其子进程会发生哪些变化？**

    子进程成为孤儿进程，被 pid=1 （RedHat7.4下名为systemd） 的进程收留，ppid成为1。

    测试截图：

    ![][2]

*   **写test4-2.c，要求与test4-1 相同，但是要求杀死test4-2 后，它的全部子进程自动退出**

    可以在父进程终止退出前，调用kill函数，kill函数相关用法如下: 

        #include <sys/types.h> 
        #include <signal.h>
        
        int kill(pid_t pid, int sig);
        
        作用：用于向任何进程组或进程发送信号。
    
        参数： 
        pid：可能选择有以下四种
        1. pid大于零时，pid是信号欲送往的进程的标识。
        2. pid等于零时，信号将送往所有与调用kill()的那个进程属同一个使用组的进程。
        3. pid等于-1时，信号将送往所有调用进程有权给其发送信号的进程，除了进程1(init)。
        4. pid小于-1时，信号将送往以-pid为组标识的进程。

    需要在main函数中第一行添加`signal(SIGTERM, kill_child);`意在父进程退出时进入kill_child函数进行处理，kill_child函数如下：

        void kill_child(int sig){
        	kill(0, SIGKILL);
        	int status;
        	while(waitpid(-1, &status, WNOHANG)>0)
        		;
        	exit(0);
        }

    test4-2.c最终代码如下：

        //test4-2.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define FORK_NUM 10
        #define FORK_SLEEP_TIME 3
        #define MAIN_SLEEP_TIME 5
        #define SUB_SLEEP_TIME 15
        
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
        
        void kill_child(int sig){
        	kill(0, SIGKILL);
        	int status;
        	while(waitpid(-1, &status, WNOHANG)>0)
        		;
        	exit(0);
        }
        
        int main(){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        
        	signal(SIGTERM, kill_child);
        
        	pid_t pid;
        	int i;
        	for (i=0; i<FORK_NUM; ++i)
        		if ((pid=fork()) < 0){
        			perror("fork error");
        			exit(errno);
        		}
        		else if(pid==0)
        			//子进程退出
        			break;
        		else 
        			//父进程间隔3s
        			sleep(FORK_SLEEP_TIME);	
        	
        	pid_t selfid=getpid();
        	pid_t faid=getppid();
        	
        	if(pid){	//父进程
        		while(1){
        			printf("%d %d 1453381 main\n", faid, selfid);
        			sleep(MAIN_SLEEP_TIME);
        		}
        	}
        	else{		//子进程
        		while(1){
        			printf("%d %d 1453381 sub\n", faid, selfid);
        			sleep(SUB_SLEEP_TIME);
        		}
        	}
        	
        	return 0;
        }

    最后需要说明的是，杀掉父进程时，需要用`kill -15 pid`结束，此时定义在main函数中的`signal(SIGTERM, kill_child)`才会生效，如果用`kill -9 pid`结束父进程，分裂的子进程父进程变为1，成为孤儿进程。

    测试截图：

    ![][3]

[0]: Linux守护进程4/0.jpg
[1]: Linux守护进程4/1.jpg
[2]: Linux守护进程4/2.jpg
[3]: Linux守护进程4/3.jpg
