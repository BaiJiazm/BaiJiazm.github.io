---
Linux-Socket编程-TCP阻塞方式02
---

写一对TCP Socket 的测试程序，分为client 和server，分别运行在不同虚拟机上

*   **前提：第1 小题中，client 每次连接server 时，client 的端口号是随机分配的**

*   **测试程序tcp_server2，与tcp_server1 功能相同，但接受连接时打印client 端的IP 地址
和端口号**

    打印client 端的IP 地址和端口号

    	struct sockaddr_in sa;
    	int len = sizeof(sa);
    	if(getpeername(connect_fd, (struct sockaddr *)&sa, &len)==-1) {
    		printf("getpeername is error : %s(error: %d)\n", strerror(errno), errno);
    	} else {
    		printf("client login. ip: %s, port :%d\n",inet_ntoa(sa.sin_addr),ntohs(sa.sin_port));
    	}

*   **测试程序tcp_client2，要求连接server 端的时候使用固定端口号，通过main 函数带参数的方式传入（例：./tcp_client2 12345 192.168.80.230 4000 则表示client 的12345 端口连接server 的4000 端口）**

    建立连接函数代码

        /**
         * 客户端绑定固定端口 去 连接到server端
         */
        int connectServer(int localPort, char* ip, int port) {
        	int flags = 1;
        	int fd = socket(AF_INET, SOCK_STREAM, 0);
        	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (void *) &flags, sizeof(flags));
        	/* 客户端绑定固定端口*/
        	struct sockaddr_in localAddr;
        	bzero(&localAddr, sizeof(localAddr));
        	localAddr.sin_family = AF_INET;
        	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        	localAddr.sin_port = htons(localPort);
        	if( -1==bind(fd, (struct sockaddr*) &localAddr, sizeof(localAddr))) {
        		printf("bind error: %s(errno: %d)\n",strerror(errno),errno);
        		exit(0);
        	}
        
        	struct sockaddr_in remote_addr; //服务器端网络地址结构体
        	memset(&remote_addr, 0, sizeof(remote_addr)); //数据初始化--清零
        	remote_addr.sin_family = AF_INET; //设置为IP通信
        	remote_addr.sin_addr.s_addr = inet_addr(ip); //服务器IP地址
        	remote_addr.sin_port = htons(port); //服务器端口号
        	if(-1==connect(fd, (struct sockaddr*) &remote_addr,sizeof(struct sockaddr))) {
        		printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        		close(fd);
        		exit(0);
        	}
        	return fd;
        }

    测试截图：

    ![][021]

[021]: Linux-Socket编程-TCP阻塞方式02/021.jpg