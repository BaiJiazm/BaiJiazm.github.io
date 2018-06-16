---
### Linux-Socket编程-TCP阻塞方式01
---

写一对TCP Socket 的测试程序，分为client 和server，分别运行在不同虚拟机上

*   **测试程序tcp_server1（源程序名任意，允许多个，C/C++语言任选，make 后得到tcp_server1即可，下同），运行后绑定某个TCP 端口号，并进入等待连接状态（下面称为LISTEN 状态），要求端口号通过main 函数带参数的方式传入（例：./tcp_server1 4000 表示绑定TCP 4000
端口）**

*   **如果服务端绑定的端口号已被使用（比如两次运行 ./tcp_server1 4000 或 ./tcp_server180），则无法进入LISTEN 状态，会在哪个函数上出错？**

    bind函数出错

    ![][011]

*  **测试程序tcp_client1，运行时带入服务端IP 地址及端口号，即可向服务端发起连接，要求
IP 地址、端口号通过main 函数带参数的方式传入（例：./tcp_client1 192.168.80.230 4000
则表示连接192.168.80.230 的TCP 4000 端口）**

*   **如果client 端连接时的IP 地址不正确（例如不存在的IP 地址），会在哪一步出错？如果连
接的端口号不正确，会在哪一步出错？**

    IP地址不正确：connect函数出错

    ![][012]

    端口号不正确：connect函数出错

    ![][013]

*   **连接成功后，双方给出相应的提示信息，双方均进入read(recv)状态，此时read/recv 函数
会阻塞**

    如图：

    ![][014]

*   **连接成功后，用CTRL+C 中断client（server）端，Server（client）端能否能侦测到连接已中断？**

    中断client, server能侦测连接中断

    ![][0151]

    中断server, client能侦测连接中断

    ![][0152]

*   **连接成功后，用kill -9 杀死client（server）端，Server（client）端能否能侦测到连接
已中断？（另外启动一个SecureCRT 的会话来做kill）**

    kill client, server能侦测连接中断

    ![][0161]

    kill server, client能侦测连接中断

    ![][0162]

*   **在双方连接成功后，再新的会话中再启动一个tcp_client1 连接server，会出现什么情况？**

    两个tcp_client1能同时连接server,但是退出情况不同

    第一个tcp_client1先退出：

    ![][0171]

    第二个tcp_client1先退出，

    ![][0172]

*   **tcp_sevrer1 运行终止后，立即再次启动，绑定相同端口号，能否成功？（REUSEADDR 选项的作用，加或不加的区别是什么？）**

    若在bind()函数前加上以下代码，设置reuse为0,则不能；否则能。
    
        int reuse=0;
	    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse);

    设置reuse为0测试截图：

    ![][018]

    一般来说，一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用。未设置此项前，若服务端开启后，又关闭，此时sock处于TIME_WAIT状态，与之绑定的socket地址不可重用，而导致再次开启服务端失败。经过setsockopt设置之后， 即使处于TIME_WAIT些状态也可以立即被重用。

    SO_REUSEADDR提供如下四个功能：

    1.  SO_REUSEADDR允许启动一个监听服务器并捆绑其众所周知端口，即使以前建立的将此端口用做他们的本地端口的连接仍存在。这通常是重启监听服务器时出现，若不设置此选项，则bind时将出错。
    2.  SO_REUSEADDR允许在同一端口上启动同一服务器的多个实例，只要每个实例捆绑一个不同的本地IP地址即可。对于TCP，我们根本不可能启动捆绑相同IP地址和相同端口号的多个服务器。
    3.  SO_REUSEADDR允许单个进程捆绑同一端口到多个套接口上，只要每个捆绑指定不同的本地IP地址即可。这一般不用于TCP服务器。
    4.  SO_REUSEADDR允许完全重复的捆绑：当一个IP地址和端口绑定到某个套接口上时，还允许此IP地址和端口捆绑到另一个套接口上。一般来说，这个特性仅在支持多播的系统上才有，而且只对UDP套接口而言（TCP不支持多播）。

[011]:  Linux-Socket编程-TCP阻塞方式01/011.jpg
[012]:  Linux-Socket编程-TCP阻塞方式01/012.jpg
[013]:  Linux-Socket编程-TCP阻塞方式01/013.jpg
[014]:  Linux-Socket编程-TCP阻塞方式01/014.jpg
[0151]:  Linux-Socket编程-TCP阻塞方式01/0151.jpg
[0152]:  Linux-Socket编程-TCP阻塞方式01/0152.jpg
[0161]:  Linux-Socket编程-TCP阻塞方式01/0161.jpg
[0162]:  Linux-Socket编程-TCP阻塞方式01/0162.jpg
[0171]:  Linux-Socket编程-TCP阻塞方式01/0171.jpg
[0172]:  Linux-Socket编程-TCP阻塞方式01/0172.jpg
[018]:  Linux-Socket编程-TCP阻塞方式01/018.jpg