---
title: Linux守护进程5
date: 2017/10/18 19:44:25  
tag: Linux
---

---

#### Linux守护进程5 — 守护进程分裂子进程极限测试

---

*   **在05 子目录下写test5-1.c，用main 函数带参数方式带入一个参数表示循环次数，然后循环
指定参数产生子进程，每个子进程中定义一个大小为1024 的字符数组，任意赋值，然后进入
死循环（为了屏幕干净，不用打印信息）；主进程每分裂若干个子进程（例如：10 个/100 个
等，可自行决定）后打印一次"已分裂\*\*\*个子进程"，循环结束后/或分裂子进程失败后打印
分裂成功的总数，然后进入死循环；写配套的makefile 文件，make 后可生成test5-1 可执行
文件，执行方法为 ./test5-1 1000 表示分裂1000 个子进程**

        //test5-1.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define DEFAULT_NUMS 500
        #define PRINT_CYCLE 1000
        #define SUB_STR_SIZE 1024
        #define SUB_SLEEP_TIME 1
        #define MAIN_SLEEP_TIME 1
        
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
        	while(waitpid(-1, NULL, WNOHANG)>0)
        		;
        }
        
        int main(int argc, char **argv){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        	signal(SIGCHLD, sig_child);
        	signal(SIGINT, sig_child);
            signal(SIGTERM, sig_child);
        	
        	int forkNums=argc>1 ? atoi(argv[1]): DEFAULT_NUMS;
        	int i;
        	
        	pid_t pid;
        	
        	for (i=0; i<forkNums; ++i){
        		if (i%PRINT_CYCLE==0)
        			printf("已分裂 %6d 个子进程\n", i);
        		
        		if ((pid=fork()) < 0){
        			perror("fork error");
        			break;
        		}
        		else if(pid==0)		//子进程退出
        			break;
        	}
        
        	if (pid==0) {	//子进程
        		char str [SUB_STR_SIZE];
        		//初始随机，算是随机赋值了吧
        		while (1)
        			sleep(SUB_SLEEP_TIME);
        	}
        	else {
        		printf("目标分裂:%d 成功分裂:%d \n", forkNums, i);
        		while (1)
        			sleep(MAIN_SLEEP_TIME);
        	}
        	
        	return 0;
        }

    makefile

        CC ?= gcc
        CFLAGS =
        
        srcNames :=test5-1 test5-2 test5-3
        
        all :
        	@for src in $(srcNames) ; \
        	do \
        		$(CC) $(CFLAGS) -o $$src "$$src".c ; \
        	done
        
        clean :
        	rm -f *.o $(srcNames)
        
        .PHONY : all clean

*   **虚拟机的内存设置为512MB，分裂数量达到多少时，分裂子进程会失败？**

    ![][1]

*   **虚拟机的内存为1024MB、2048MB 时，分裂最大数量又是多少？**

    ![][2]

    ![][3]

*   **把`char str[1024]`改为`char str[1024*10]`，再次测试三种内存下的最大分裂数量**

    512M：  

    ![][4]

    1024M：  

    ![][5]

    2048M：  

    ![][6]

*   **写test5-2.c，要求与test5-1 相同，但是子进程给str 赋值完后，不要死循环，等待20 秒
后子进程退出，在这种情况下，如何做到在小内存的情况下分裂完成指定大数量的子进程？（例如：在512MB 内存情况下，分别100000 个子进程且必须都分裂成功）**

    要做到在限定内存下同时存在大数量子进程是不行的，但是因为一些子进程会在一定时间推出，只要做到能够收到子进程退出信号回收内存，同时父进程在不能成功fork时休眠一段时间再尝试fork，这样总能成功，完成分裂指定大数量的子进程。测试截图见下一题。

        //test5-2.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define DEFAULT_NUMS 500
        #define PRINT_CYCLE 1000
        #define SUB_STR_SIZE 1024
        #define SUB_SLEEP_TIME 20
        #define MAIN_SLEEP_TIME 1
        
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
        	close(0);   /* close stdin stdout stderr */
        //	close(1);
        	close(2);
        	return 0;
        }
        
        void sig_child(int signo){
        	while(waitpid(-1, NULL, WNOHANG)>0)
        		;
        }
        
        int main(int argc, char **argv){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        	signal(SIGCHLD, sig_child);
        	signal(SIGINT, sig_child);
            signal(SIGTERM, sig_child);
        	
        	int forkNums=argc>1 ? atoi(argv[1]): DEFAULT_NUMS;
        	int i, max_pid=0;
        	
        	pid_t pid;
        	
        	for (i=0; i<forkNums; ){
        		if (i%PRINT_CYCLE==0)
        			printf("已分裂 %6d 个子进程\n", i);
        		
        		pid=fork();
        		if(pid==0)
        			break;
        		else if(pid>0)
        			++i;
        		else
        			sleep(MAIN_SLEEP_TIME);
        
        		max_pid= max_pid<pid? pid :max_pid;
        	}
        
        	if (pid==0) {	//子进程
        		char str [SUB_STR_SIZE];
        		//初始随机，算是随机赋值了吧
        		sleep(SUB_SLEEP_TIME);
        		exit(0);
        	}
        	else {			//父进程
        		printf("目标分裂:%d 成功分裂:%d \n", forkNums, i);
        		printf("max_pid = %d \n", max_pid);
        		while (1)
        			sleep(MAIN_SLEEP_TIME);
        	}
        	
        	return 0;
        }

*   **在test5-2.c 中加适当的语句，看分裂的子进程的最大进程号是多少？**

    测试1截图（10w）：

    ![][70]
    
    测试2截图(10w)：

    ![][71]

    测试3截图(100w)：

    ![][72]

    测试4截图(100w)：

    ![][73]

*   **写test5-3.c，基本要求同test5-2，但是由守护进程负责回收每个子进程退出信号，设置两
个全局变量做为计数器，一个记录分裂成功的数量，一个记录回收成功的数量，要求全部分
裂完成后，且所有子进程都退出后，两个计数器的值要相同（测试数量≧50000）**

    在test5-2.c的基础上，只要做到子进程退出时，通知到父进程，父进程调用函数处理即可。

        //test5-3.c
        
        #include <unistd.h>
        #include <stdio.h>
        #include <stdlib.h>
        #include <sys/types.h>
        #include <signal.h>
        #include <errno.h>
        #include <sys/wait.h>
        
        #define DEFAULT_NUMS 500
        #define PRINT_CYCLE 1000
        #define SUB_STR_SIZE 1024
        #define SUB_SLEEP_TIME 1
        #define MAIN_SLEEP_TIME 1
        #define ERROR_SLEEP_TIME SUB_SLEEP_TIME
        
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
        	close(0);   /* close stdin stdout stderr */
        //	close(1);
        	close(2);
        	return 0;
        }
        
        int exitNums=0;
        
        void sig_child(int signo){
        	while(waitpid(-1, NULL, WNOHANG)>0)
        		++exitNums;
        }
        
        int main(int argc, char **argv){
        	if (daemon_init() == -1){
        		printf("can't fork self\n");
        		exit(0);
        	}
        	
        	signal(SIGCHLD, sig_child);
        	signal(SIGINT, sig_child);
            signal(SIGTERM, sig_child);
        	
        	int forkNums=argc>1 ? atoi(argv[1]): DEFAULT_NUMS;
        	int i, max_pid=0;
        	
        	pid_t pid;
        	
        	for (i=0; i<forkNums; ){
        		if (i%PRINT_CYCLE==0)
        			printf("已分裂 %6d 个子进程\n", i);
        		
        		pid=fork();
        		if(pid==0)
        			break;
        		else if(pid>0)
        			++i;
        		else
        			sleep(MAIN_SLEEP_TIME);
        
        		max_pid= max_pid<pid? pid :max_pid;
        	}
        
        	if (pid==0) {	//子进程
        		char str [SUB_STR_SIZE];
        		//初始随机，算是随机赋值了吧
        		sleep(SUB_SLEEP_TIME);
        		exit(0);
        	}
        	else {			//父进程
        		printf("目标分裂:%d 成功分裂:%d \n", forkNums, i);
        		printf("max_pid = %d \n", max_pid);
        		while (1){
        			printf("成功分裂%d个， 成功回收%d个\n", i, exitNums);
        			sleep(MAIN_SLEEP_TIME);
        		}
        	}
        	
        	return 0;
        }

    测试截图

    ![][80]
    ![][81]

*   **提示：当极限测试导致子进程分裂失败后，Linux 系统还能正常操作吗？**

    已经不能正常工作。

    ![][9]

[1]: Linux守护进程5/1.jpg
[2]: Linux守护进程5/2.jpg
[3]: Linux守护进程5/3.jpg
[4]: Linux守护进程5/4.jpg
[5]: Linux守护进程5/5.jpg
[6]: Linux守护进程5/6.jpg
[70]: Linux守护进程5/70.jpg
[71]: Linux守护进程5/71.jpg
[72]: Linux守护进程5/72.jpg
[73]: Linux守护进程5/73.jpg
[80]: Linux守护进程5/80.jpg
[81]: Linux守护进程5/81.jpg
[9]: Linux守护进程5/9.jpg