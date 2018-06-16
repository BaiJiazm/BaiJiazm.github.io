---

### Linux-Socket编程-TCP非阻塞方式03

---

*   **测试程序tcp_server3-1/tcp_client3-1，连接成功后，server 发数据（每次10 字节，间隔1 秒）并同时用大小100 的缓冲区收数据，client 发数据（每次15 字节，间隔3 秒）并同时用大小100 的缓冲区收数据，死循环运行**
*   **注意：死循环中不是先发送若干字节、延时、再收若干字节，而是读写并发，以client 端为例，相当于打印信息为收3 次，发1 次（也可能是其它收发值）**

    在非阻塞服务端编程中，绝对不能用 sleep 或类似的办法来让程序原地停留等待，这会让程序失去响应，因为主事件循环被挂起了，无法处理 IO 事件。这就像在 Windows 编程中绝对不能在消息循环里执行耗时的代码一样，会让程序界面失去响应。

    利用select+定时器，当发生定时信号SIGALRM后，select函数中断，返回-1，错误号errno=EINTR(4),这时进行写入。

    定时器:

        #include <unistd.h>
        unsigned int alarm(unsigned int seconds);

    非阻塞函数，定时时间到了就发送SIGALRM信号，如果在alarm之前已经调用了alarm并且时间没有到的话，那么此次返回之前alarm剩余的时间，否则返回0。

    以server端为例，关键代码如下：

    定时信号产生后执行函数

        #define ALARM_SECOND 1
        void do_alarm(int i) {
        	alarm(ALARM_SECOND);
        }

    主函数中写法

        if(signal(SIGALRM , do_alarm) == SIG_ERR) {
    		printf("register alarm error: %s(errno: %d)\n",strerror(errno),errno);
    		exit(0);
    	}
    	alarm(ALARM_SECOND);
    
    	while(1) {
    		FD_ZERO(&rfds);
    		FD_SET(connectfd, &rfds);
    		wfds=rfds;
    
    		selres = select(connectfd + 1, &rfds, NULL, NULL, NULL);
    		if(selres<0) {
    			err=errno;
    			if(err==EINTR) {
    				selres = select(connectfd + 1, NULL, &wfds, NULL, NULL);
    				if(FD_ISSET(connectfd, &wfds)) {
    					n = write(connectfd, writebuff, WRITE_SIZE);
    					writebuff[n] = '\0';
    					printf("server write %d bytes\n", n);
    				}
    				continue;
    			} else {
    				printf("select error: %s(errno: %d)\n",strerror(errno),errno);
    				exit(0);
    			}
    		}
    		if(FD_ISSET(connectfd, &rfds)) {
    			n = read(connectfd, readbuff, READ_SIZE);
    			readbuff[n] = '\0';
    			printf("server read %d bytes\n", n);
    		}
    	}

    测试截图：

    ![][031]

*   **测试程序tcp_server3-2/tcp_client3-2，连接成功后，server 发数据（每次10 字节，间隔1 秒）并同时用大小为88 的缓冲区收数据，能否在非阻塞模式下保证每次必须收到88 字节才返回，即每read 或recv 函数均返回读取了88 字节？(注：不允许采用自己写循环保证读满88 字节）client 发数据（每次15 字节，间隔3 秒）并同时用大小100 的缓冲区收数据，死循环运行**

    SO_RCVLOWAT 和 SO_SNDLOWAT套接字选项
    
    每个套接字还有一个接收低水位标记和一个发送低水位标记。由select函数使用，这两个套接字选项允许我们修改这两个低水位标记。接收低水位标记是让select返回“可读”时，套接字接收缓冲区中所需的数据量。对于TCP，UDP和SCTP套接字，其默认值为1。发送低水位标记是让select返回“可写”时套接字发送缓冲区中所需的可用空间。对于TCP套接字，其默认值通常为2048。

        #define READ_SIZE 88
    	int recv_min_size = READ_SIZE;
    	int flag = setsockopt(connectfd, SOL_SOCKET, SO_RCVLOWAT, (void *)&recv_min_size, sizeof(int));

    测试截图：

    ![][032]

[031]: Linux-Socket编程-TCP非阻塞方式03/031.jpg
[032]: Linux-Socket编程-TCP非阻塞方式03/032.jpg