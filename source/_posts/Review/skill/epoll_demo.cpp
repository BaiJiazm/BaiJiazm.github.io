#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <string.h>

#define SERV_PORT           8031
#define MAX_EVENT_NUMBER    1024
#define BUFFER_SIZE         10

/* ���ļ������� fd �ϵ� EPOLLIN ע�ᵽ epollfd ָʾ�� epoll �ں��¼����� */

void addfd(int epollfd, int fd) {
	struct epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET;
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}

void et(struct epoll_event *events, int number, int epollfd, int listenfd) {
	char buf[BUFFER_SIZE];
	for (int i = 0; i < number; ++i) {
		int sockfd = events[i].data.fd;
		if (sockfd == listenfd) {
			struct sockaddr_in client_address;
			socklen_t length = sizeof(client_address);
			int connfd = accept(listenfd, (struct sockaddr *) &client_address, &length);
			printf("����һ������������: %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

			addfd(epollfd, connfd);
		} else if (events[i].events & EPOLLIN) {
			/* ��δ��벻�ᱻ�ظ���������������ѭ����ȡ���ݣ���ȷ���� socket �����е��������ݶ�ȡ*/
			while (1) {
				memset(buf, '\0', BUFFER_SIZE);
				int ret = recv(sockfd, buf, BUFFER_SIZE - 1, 0);
				if (ret < 0) {
					/* �Է����� IO �����������������ʾ�����Ѿ�ȫ����ȡ��ϡ��˺� epoll �����ٴδ��� sockfd �ϵ� EPOLLIN �¼�����������һ�ζ����� */
					if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
						printf("read later\n");
						break;
					}
					close(sockfd);
					break;
				} else if (ret == 0) {
					printf("�Ͽ�һ������\n");
					close(sockfd);
				} else {
					printf("get %d bytes of content: %s\n", ret, buf);
				}
			}
		}
	}
}

int main(void) {
	int lfd, epollfd,ret;
	struct sockaddr_in serv_addr;

	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("�׽�������������ʧ��");
		exit(1);
	}

	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);

	if (bind(lfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
		perror("��ʧ��");
		exit(1);
	}

	if (listen(lfd, 5) == -1) {
		perror("����ʧ��");
		exit(1);
	}

	struct epoll_event events[MAX_EVENT_NUMBER];
	if ((epollfd = epoll_create(5)) == -1) {
		perror("����ʧ��");
		exit(1);
	}

	// �ѷ������� lfd ��ӵ� epollfd ָ���� epoll �ں��¼����У����һ�� lfd �ɶ����¼�
	addfd(epollfd, lfd);
	while (1) {
		// �����ȴ��¿ͻ��˵����ӻ��߿ͻ��˵�����д�룬������Ҫ������¼���Ŀ
		if ((ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1)) < 0) {
			perror("epoll_waitʧ��");
			exit(1);
		}

		et(events, ret, epollfd, lfd);
	}

	close(lfd);
	return 0;
}
