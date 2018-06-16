---
### Linux-Socket编程-TCP阻塞方式03
---

写一对TCP Socket 的测试程序，分为client 和server，分别运行在不同虚拟机上

*   **两台用于测试的RHEL7 虚拟机均设置多个地址（例如：192.168.80.230/ 172.18.12.230）**

*   **测试程序tcp_server3，能读到本机所有网卡的所有IP 地址，然后只绑定其中的某个IP 地址
的某个端口，要求IP 地址和端口号通过main 函数带参数的方式传入（例：./tcp_sevrer3
172.18.12.230 4000 表示只绑定172.18.12.230 的4000 端口）**

        //获取本机网卡所有IP地址 
        void getAllLocalIP(char **pIps[], int *ipcnt) {
        	char **ips;
        	int sockfd;
        	struct ifconf ifconf;
        	struct ifreq *ifreq;
        	char buf[MAXLINE];//缓冲区
        	//初始化ifconf
        	ifconf.ifc_len =MAXLINE;
        	ifconf.ifc_buf = buf;
        	if ((sockfd =socket(AF_INET,SOCK_DGRAM,0))<0) {
        		printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        		exit(0);
        	}
        	ioctl(sockfd, SIOCGIFCONF, &ifconf); //获取所有接口信息
        
        	//接下来一个一个的获取IP地址
        	ifreq = (struct ifreq*)ifconf.ifc_buf;
        //    printf("ifconf.ifc_len:%d\n",ifconf.ifc_len);
        //    printf("sizeof (struct ifreq):%d\n",sizeof (struct ifreq));
        
        	*ipcnt=(ifconf.ifc_len/sizeof (struct ifreq));
        	*pIps=(char **)malloc(*ipcnt*sizeof(char**));
        	ips=*pIps;
        
        	int i, len;
        	for (i=0; i<*ipcnt; ++i) {
        		if(ifreq->ifr_flags == AF_INET) { //for ipv4
        			//printf("name =[%s]\n" , ifreq->ifr_name);
        			//printf("local addr = [%s]\n" ,inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        			len=1+strlen(inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        			ips[i]=(char *)malloc(len*sizeof(char));
        			strcpy(ips[i], inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr));
        			ifreq++;
        		}
        	}
        	return;
        }

*   **测试程序tcp_client3，运行时带入服务端IP 地址及其中任意一个端口号，即可向服务端发
起连接，要求IP 地址、端口号通过main 函数带参数的方式传入（例：./tcp_client3
172.18.12.230 4000 则表示连接172.18.12.230 的TCP 4000 端口）**

    ![][032]

*   **如果tcp_client3 连接未绑定的IP 地址（例如：192.168.80.230），会怎样？**

    会出现以下错误：

        connect error: No route to host(errno: 113)

    ![][031]

[031]: Linux-Socket编程-TCP阻塞方式03/031.jpg
[032]: Linux-Socket编程-TCP阻塞方式03/032.jpg

**参考：**
    [Linux下编程获取本地IP地址的常见方法](http://blog.csdn.net/k346k346/article/details/48231933)