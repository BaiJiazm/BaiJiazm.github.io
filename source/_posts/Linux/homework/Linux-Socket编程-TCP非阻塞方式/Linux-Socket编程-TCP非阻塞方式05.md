---

### Linux-Socket编程-TCP非阻塞方式05

---

*   **测试程序tcp_server5，接受连接成功后，server 发数据（每次10 字节，间隔1 秒）并同时用大小100 的缓冲区收数据，死循环运行**
*   **测试程序tcp_client5-1，连接成功后，client 发数据（每次15 字节，间隔3 秒）并同时用大小100 的缓冲区收数据，死循环运行**
*   **Server 端先接受一个Client 端的连接，进入死循环读写状态**
*   **要求此时Server 能接受一个新的Client 端的连接，也进入死循环读写状态（Server 端一个程序维护一个listen socket 和多个accept 的socket，要保证accept 的socket 进入死循环读写的同时，仍然能接受新的Client 端的连接）**

    利用select进行单进程并发。

    将所有fds（包括listenfd, 所有connectfd）加入到allfds中管理；  

        rfds=allfds; 
        selres=select(maxfd+1, &rfds, NULL, NULL, NULL);

    selres<0 判断是否为定时中断，可写时执行写操作；  
    selres>0   
        若FD_ISSET(listenfd, &rfds)有效，建立新的连接，新连接设置为非阻塞，加入到allfds中；
        若其他连接的connectfd在FD_ISSET(connectfd, &rfds)有效，进行读操作。

    关键代码：

    	for (i=0; i<FD_SETSIZE; ++i) {
    		client[i]=-1;
    	}
    	maxfd=listenfd;
    	maxi=-1;
    	FD_ZERO(&allfds);
    	FD_SET(listenfd, &allfds);
    
    	while(1) {
    		rfds=allfds;
    		wfds=allfds;
    
    		selres=select(maxfd+1, &rfds, NULL, NULL, NULL);
    		if(selres<0) {
    			err=errno;
    			if(err==EINTR) {
    				wfds=allfds;
    				selres = select(maxfd + 1, NULL, &wfds, NULL, NULL);
    				for (i=0; i<=maxi; ++i) {
    					if((connectfd=client[i])<0)
    						continue;
    					if(FD_ISSET(connectfd, &wfds)) {
    						n = write(connectfd, writebuff, WRITE_SIZE);
    						writebuff[n] = '\0';
    						printf("server write %d bytes to client's fds %d \n", n, connectfd);
    					}
    				}
    				continue;
    			} else {
    				printf("select error: %s(errno: %d)\n",strerror(errno),errno);
    				exit(0);
    			}
    		}
    
    		nready=selres;
    		if(FD_ISSET(listenfd, &rfds)) {
    			if( (connectfd = accept(listenfd, (struct sockaddr*)NULL, NULL)) == -1) {
    				printf("accept socket error: %s(errno: %d)\n",strerror(errno),errno);
    				exit(0);
    			}
    			setNonBlock(connectfd);
    			outputConnect(connectfd);
    			for (i=0; i<FD_SETSIZE; ++i) {
    				if(client[i]<0) {
    					client[i]=connectfd;
    					break;
    				}
    			}
    			if(i==FD_SETSIZE) {
    				printf("too many clients \n");
    				exit(0);
    			}
    
    			FD_SET(connectfd, &allfds);
    			maxfd=maxfd>connectfd? maxfd: connectfd;
    			maxi=maxi>i? maxi: i;
    
    			if (--nready == 0)
    				continue;
    		}
    
    		for (i = 0; i <= maxi; i++) {
    			if ( (connectfd = client[i]) < 0)
    				continue;
    			if (FD_ISSET(connectfd, &rfds)) {
    				n = read(connectfd, readbuff, READ_SIZE);
    				if (n == 0) {
    					/* connection closed by client */
    					if(close(connectfd) == -1) {
    						printf("close socket error: %s(errno: %d)\n",strerror(errno),errno);
    						exit(0);
    					}
    					printf("client fds %d closed \n", connectfd);
    					FD_CLR(connectfd, &allfds);
    					client[i] = -1;
    				} else {
    					printf("server read %d bytes from client's fds %d\n", n, connectfd);
    				}
    				if (--nready == 0)
    					break;
    			}
    		}
    	}

    测试截图：

    启动两个client：

    ![][051]

    关闭两个client：

    ![][052]

*   **用两个会话窗口分别启动两个tcp_server5 （如：./tcp_server5 4000 ./tcp_server5 5000）**
*   **测试程序tcp_client5-2，运行时带两个端口号（例：./tcp_client5-2 192.168.80.230 40005000），表示在一个程序中建立两个socket，分别连接两个不同端口号的server 端，client发数据（每次15 字节，间隔3 秒）并同时用大小100 的缓冲区收数据，死循环运行，允许在不同会话窗口启动多个client 端**
*   **Server 端和Client 端均不允许采用分裂进程的方式，只能是一个程序**

    tcp_client5-2 原理类似server程序，不再赘述。

    测试截图：

    启动两个server，启动两个client，每个client连接两个server。

    ![][053]

[051]: Linux-Socket编程-TCP非阻塞方式05/051.jpg
[052]: Linux-Socket编程-TCP非阻塞方式05/052.jpg
[053]: Linux-Socket编程-TCP非阻塞方式05/053.jpg
