---

### Linux-Socket编程-TCP非阻塞方式01

---

*   **测试程序tcp_server1-1（源程序名任意，允许多个，C/C++语言任选，make 后得到tcp_server1即可，下同），运行后绑定某个TCP 端口号，并进入等待连接状态（下面称为LISTEN 状态），要求端口号通过main 函数带参数的方式传入（例：./tcp_server1 4000 表示绑定TCP 4000端口）**
*   **测试程序tcp_client1-1，运行时带入服务端IP 地址及端口号，即可向服务端发起连接，要求IP 地址、端口号通过main 函数带参数的方式传入（例：./tcp_client1 192.168.80.230 4000则表示连接192.168.80.230 的TCP 4000 端口）**
*   **Server 端用于listen 的socket，不设置为非阻塞方式，accept 成功后，将accept 的socket设置为非阻塞方式**
*   **Client 端建立的socket，先不设置为非阻塞方式，待connect 成功后，再设置为非阻塞方式**
*   **连接成功后，双方均进入read(recv)状态，read(recv)函数后直接关闭socket，程序退出**

*  **read(recv)函数的表现会如何？程序会阻塞在read(recv)还是立即结束？ read(recv)函数
返回什么？**

    read(recv)函数不会阻塞，读取了0字节，read(recv)函数返回0。

    ![][011]

*   **测试程序tcp_server1-2/tcp_client1-2，在1-1 的基础上，用select 使read(recv)停下来而不立即返回**

    用select函数处于无限阻塞
    
    	fd_set rfds;
    	FD_ZERO(&rfds);
    	FD_SET(socket_fd, &rfds);
    
    	int selres = select(socket_fd + 1, &rfds, NULL, NULL, NULL);

    ![][012]

*   **测试程序tcp_server1-3，要求socket 建立成功后，先设置非阻塞方式，再进行bind、listen和accept，accept 的新socket，也是立即设置为非阻塞方式，再进行后续操作**
*   **测试程序tcp_client1-3，要求socket 建立成功后，先设置非阻塞方式，再connect**
*   **要求tcp_client1-3 能连接tcp_server1-3 成功,并在连接成功后，使双方都停在read(recv)上而不立即返回**

    server 非阻塞accpet失败后用select建立新的连接，核心代码：

    	if( (connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
    		FD_ZERO(&rfds);
    		FD_SET(listenfd, &rfds);
    		selres = select(listenfd + 1, &rfds, NULL, NULL, NULL);
    		if(selres<0) {
    			perror("select error\n");
    			exit(0);
    		}
    
    		if(FD_ISSET(listenfd, &rfds)) {
    			if( (connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
    				printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);
    				exit(0);
    			}
    		}
    	}
    
    	setNonBlock(connectfd);
    	outputConnect(connectfd);

    client端建立非阻塞连接时，select返回大于0，调用connect可能返回-1,若errno被设为EINPROGRESS,意即connect仍旧在进行还没有完成，需要时间建立，多次select。

        int nonBlockConnect(int socket_fd, struct sockaddr_in *servaddr) {
        	fd_set rfds, wfds;
        	struct timeval tv;
        	int selres, conres, ret;
        
        	conres=connect(socket_fd, (struct sockaddr*)servaddr, sizeof(struct sockaddr_in));
        	if (0 == conres) {
        		printf("socket connect succeed immediately.\n");
        		ret=0;
        	} else {
        		printf("get the connect result by select().\n");
        		if (errno == EINPROGRESS) {
        			int times = 0;
        			while (times++ < 5) {
        				FD_ZERO(&rfds);
        				FD_ZERO(&wfds);
        				FD_SET(socket_fd, &rfds);
        				FD_SET(socket_fd, &wfds);
        
        				tv.tv_sec = 10;
        				tv.tv_usec = 0;
        				selres=select(socket_fd + 1, &rfds, &wfds, NULL, &tv);
        				if(-1==selres) {
        					printf("select error\n");
        					ret = -1;
        				} else if(0==selres) {
        					printf("select time out\n");
        					ret = -1;
        				} else {
        					if (FD_ISSET(socket_fd, &rfds) || FD_ISSET(socket_fd, &wfds)) {
        						connect(socket_fd, (struct sockaddr *)servaddr, sizeof(struct sockaddr_in));
        						int err = errno;
        						if  (err == EISCONN) {
        							printf("connect finished.\n");
        							ret = 0;
        						} else {
        							printf("connect failed. errno = %d\n", errno);
        							printf("FD_ISSET(socket_fd, &rfds) :%d FD_ISSET(socket_fd, &wfds) :%d\n",
        							       FD_ISSET(socket_fd, &rfds) , FD_ISSET(socket_fd, &wfds));
        							ret = errno;
        						}
        					}
        				}
        
        				if (-1 != selres && (ret != 0)) {
        					printf("check connect result again... times=%d\n", times);
        					continue;
        				} else {
        					break;
        				}
        			}
        		} else {
        			printf("connect error: %s(errno: %d)\n",strerror(errno),errno);
        			ret = errno;
        			exit(0);
        		}
        	}
        	return ret;
        }

    ![][013]

[011]: Linux-Socket编程-TCP非阻塞方式01/011.jpg
[012]: Linux-Socket编程-TCP非阻塞方式01/012.jpg
[013]: Linux-Socket编程-TCP非阻塞方式01/013.jpg