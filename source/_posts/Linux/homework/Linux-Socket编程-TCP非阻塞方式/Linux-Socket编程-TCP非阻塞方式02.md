---

### Linux-Socket编程-TCP非阻塞方式02

---

*   **测试程序tcp_server2-1/tcp_client2-1，连接成功后，client 发数据（每次10 字节，间隔1 秒），server 用大小100 的缓冲区收数据，死循环运行**
*   **此时在client（server）端按CTRL+C，server（client）端能否检测到连接中断？**

    client端ctrl+c后，server的connectfd仍可读，但是read函数返回0，说明client端已经关闭连接。

    client端ctrl+c，运行示例：

    ![][021]

    server端ctrl+c后，client端会收到SIGPIPE信号，说明server已经关闭连接。

    当服务器close一个连接时，若client端接着发数据。根据TCP协议的规定，会收到一个RST响应，client再往这个服务器发送数据时，系统会发出一个SIGPIPE信号给进程，告诉进程这个连接已经断开了，不要再写了。
    
    根据信号的默认处理规则SIGPIPE信号的默认执行动作是terminate(终止、退出),所以client会退出。

    若不想客户端退出可以把SIGPIPE设为SIG_IGN。如:`signal(SIGPIPE,SIG_IGN)`;这时SIGPIPE交给了系统处理。

    signal设置的信号句柄只能起一次作用，信号被捕获一次后，信号句柄就会被还原成默认值了。  
    sigaction设置的信号句柄，可以一直有效，直到你再次改变它的设置。

    	struct sigaction action;
    	action.sa_handler = handle_pipe;
    	sigemptyset(&action.sa_mask); //清空此信号集
    	action.sa_flags = 0;
    	sigaction(SIGPIPE, &action, NULL);

    server端ctrl+c，运行示例：

    ![][022]

*   **如果新开一个会话窗口，用kill -9 杀client（server）端程序，server（client）端能否检测到连接中断？**

    kill -9 结果与ctrl+c一样。

    kill -9 client

    ![][023]

    kill -9 server

    ![][024]

*   **测试程序tcp_server2-2/tcp_client2-2，连接成功后，server 发数据（每次10 字节，间隔1 秒），client 用大小100 的缓冲区收数据，死循环运行（同时观察2-1 的两种中断检测方式）**

    与2-1一样，无论是ctrl+c或者kill -9， 一端关闭，若另一端在读数据，select返回rdfs有效，但是read返回0，判断为关闭； 一端关闭，若另一端在写，会收到SIGPIPE信号。

    client端ctrl+c，运行示例：

    ![][025]

    server端ctrl+c，运行示例：

    ![][026]

    kill -9 client，运行示例：

    ![][027]

    kill -9 server，运行示例：

    ![][028]

[021]: Linux-Socket编程-TCP非阻塞方式02/021.jpg
[022]: Linux-Socket编程-TCP非阻塞方式02/022.jpg
[023]: Linux-Socket编程-TCP非阻塞方式02/023.jpg
[024]: Linux-Socket编程-TCP非阻塞方式02/024.jpg
[025]: Linux-Socket编程-TCP非阻塞方式02/025.jpg
[026]: Linux-Socket编程-TCP非阻塞方式02/026.jpg
[027]: Linux-Socket编程-TCP非阻塞方式02/027.jpg
[028]: Linux-Socket编程-TCP非阻塞方式02/028.jpg