---
title: Linux守护进程0
date: 2017/10/18 19:44:25  
tag: Linux
---

---

signal函数

    #include <signal.h>

    typedef void (*sighandler_t)(int);
    sighandler_t signal(int signum, sighandler_t handler);
    
    作用1：站在应用程序的角度，注册一个信号处理函数
    作用2：忽略信号，设置信号默认处理 信号的安装和回复
    
    参数
    --  signal是一个带signum和handler两个参数的函数，准备捕捉或屏蔽的信号由参数signum
        给出，接收到指定信号时将要调用的函数有handler给出
    --  handler这个函数必须有一个int类型的参数（即接收到的信号代码），它本身的类型是void
    --  handler也可以是下面两个特殊值：① SIG_IGN 屏蔽该信号  ② SIG_DFL 恢复默认行为

    SIGABRT        进程停止运行    6
    SIGALRM        警告钟    
    SIGFPE        算述运算例外
    SIGHUP        系统挂断
    SIGILL        非法指令
    SIGINT        终端中断   2
    SIGKILL        停止进程（此信号不能被忽略或捕获）
    SIGPIPE        向没有读的管道写入数据
    SIGSEGV        无效内存段访问
    SIGQOUT        终端退出    3
    SIGTERM        终止
    SIGUSR1        用户定义信号1
    SIGUSR2        用户定义信号2
    SIGCHLD        子进程已经停止或退出
    SIGCONT        如果被停止则继续执行
    SIGSTOP        停止执行
    SIGTSTP        终端停止信号
    SIGTOUT        后台进程请求进行写操作
    SIGTTIN        后台进程请求进行读操作

kill函数

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
    
    sig：准备发送的信号代码，假如其值为零则没有任何信号送出，但是系统会执行错误检查，通常
    会利用sig值为零来检验某个进程是否仍在执行。
    
    返回值说明： 成功执行时，返回0。失败返回-1，errno被设为以下的某个值
    EINVAL：指定的信号码无效（参数 sig 不合法） 
    EPERM；权限不够无法传送信号给指定进程 
    ESRCH：参数 pid 所指定的进程或进程组不存在

wait和waitpid函数

    #include <sys/types.h>   
    #include <sys/wait.h>  
    
    pid_t waitpid(pid_t pid,int *status,int options);  

    1）wait()函数用于使父进程（也就是调用wait()的进程）阻塞，直到一个子进程结束或者该进程
    接收到了一个指定的信号为止。如果该父进程没有子进程或者它的子进程已经结束，则wait()函数
    就会立即返回。
    2) waitpid()的作用和wait()一样，但它并不一定要等待第一个终止的子进程（它可以指定需要
    等待终止的子进程），它还有若干选项，如可提供一个非阻塞版本的 wait()功能，也能支持作业
    控制。实际上，wait()函数只是 waitpid()函数的一个特例，在Linux 内部实现 wait()函数
    时直接调用的就是waitpid()函数。

    1）pid_t pid
    参数pid为欲等待的子进程识别码，其具体含义如下：
    参数值	说明
    pid<-1	等待进程组号为pid绝对值的任何子进程。
    pid=-1	等待任何子进程，此时的waitpid()函数就退化成了普通的wait()函数。
    pid=0	等待进程组号与目前进程相同的任何子进程，也就是说任何和调用waitpid()函数的进
            程在同一个进程组的进程。
    pid>0	等待进程号为pid的子进程。 

    2）int *status
    这个参数将保存子进程的状态信息，有了这个信息父进程就可以了解子进程为什么会推出，是正常
    退出还是出了什么错误。也可以传入空指针。

    3）int options
    参数options提供了一些另外的选项来控制waitpid()函数的行为。如果不想使用这些选项，则可
    以把这个参数设为0。
    主要使用的有以下两个选项：
    参数	说明
    WNOHANG	    如果pid指定的子进程没有结束，则waitpid()函数立即返回0，而不是阻塞在这
                个函数上等待；如果结束了，则返回该子进程的进程号。
    WUNTRACED	如果子进程进入暂停状态，则马上返回。