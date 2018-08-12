#define IPADDRESS   "127.0.0.1"
#define PORT        8787
#define MAXSIZE     1024
#define LISTENQ     5
#define FDSIZE      1000
#define EPOLLEVENTS 100

listenfd = socket_bind(IPADDRESS,PORT);

struct epoll_event events[EPOLLEVENTS];

//����һ��������
epollfd = epoll_create(FDSIZE);

//��Ӽ����������¼�
add_event(epollfd,listenfd,EPOLLIN);

//ѭ���ȴ�
for ( ; ; ) {
	//�ú��������Ѿ�׼���õ��������¼���Ŀ
	ret = epoll_wait(epollfd,events,EPOLLEVENTS,-1);
	//������յ�������
	handle_events(epollfd,events,ret,listenfd,buf);
}

//�¼�������
static void handle_events(int epollfd,struct epoll_event *events,int num,int listenfd,char *buf) {
	int i;
	int fd;
	//���б���;����ֻҪ�����Ѿ�׼���õ�io�¼���num�����ǵ���epoll_createʱ��FDSIZE��
	for (i = 0; i < num; i++) {
		fd = events[i].data.fd;
		//���������������ͺ��¼����ͽ��д���
		if ((fd == listenfd) &&(events[i].events & EPOLLIN))
			handle_accpet(epollfd,listenfd);
		else if (events[i].events & EPOLLIN)
			do_read(epollfd,fd,buf);
		else if (events[i].events & EPOLLOUT)
			do_write(epollfd,fd,buf);
	}
}

//����¼�
static void add_event(int epollfd,int fd,int state) {
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev);
}

//������յ�������
static void handle_accpet(int epollfd,int listenfd) {
	int clifd;
	struct sockaddr_in cliaddr;
	socklen_t  cliaddrlen;
	clifd = accept(listenfd,(struct sockaddr*)&cliaddr,&cliaddrlen);
	if (clifd == -1)
		perror("accpet error:");
	else {
		printf("accept a new client: %s:%d\n",inet_ntoa(cliaddr.sin_addr),cliaddr.sin_port);                       //���һ���ͻ����������¼�
		add_event(epollfd,clifd,EPOLLIN);
	}
}

//������
static void do_read(int epollfd,int fd,char *buf) {
	int nread;
	nread = read(fd,buf,MAXSIZE);
	if (nread == -1)     {
		perror("read error:");
		close(fd); //��סclose fd
		delete_event(epollfd,fd,EPOLLIN); //ɾ������
	} else if (nread == 0)     {
		fprintf(stderr,"client close.\n");
		close(fd); //��סclose fd
		delete_event(epollfd,fd,EPOLLIN); //ɾ������
	} else {
		printf("read message is : %s",buf);
		//�޸���������Ӧ���¼����ɶ���Ϊд
		modify_event(epollfd,fd,EPOLLOUT);
	}
}

//д����
static void do_write(int epollfd,int fd,char *buf) {
	int nwrite;
	nwrite = write(fd,buf,strlen(buf));
	if (nwrite == -1) {
		perror("write error:");
		close(fd);   //��סclose fd
		delete_event(epollfd,fd,EPOLLOUT);  //ɾ������
	} else {
		modify_event(epollfd,fd,EPOLLIN);
	}
	memset(buf,0,MAXSIZE);
}

//ɾ���¼�
static void delete_event(int epollfd,int fd,int state) {
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,&ev);
}

//�޸��¼�
static void modify_event(int epollfd,int fd,int state) {
	struct epoll_event ev;
	ev.events = state;
	ev.data.fd = fd;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&ev);
}

//ע������һ���Ҿ�ʡ��
