---
title: nginx架构
date: 2018/1/29 20:18:27  
tags:
- Nginx
---

nginx在启动后，在unix系统中会以daemon的方式在后台运行，后台进程包含一个master进程和多个worker进程。

nginx进程模型图：
![](http://tengine.taobao.org/book/_images/chapter-2-1.PNG)

master进程接收来自外界的信号，向各worker进程发送信号，监控worker进程的运行状态，当worker进程退出后(异常情况下)，会自动重新启动新的worker进程。而基本的网络事件，则是放在worker进程中来处理了。多个worker进程之间是对等的，他们同等竞争来自客户端的请求，各进程互相之间是独立的。

每个worker进程都是从master进程fork过来，在master进程里面，先建立好需要listen的socket（listenfd）之后，然后再fork出多个worker进程。所有worker进程的listenfd会在新连接到来时变得可读，为保证只有一个进程处理该连接，所有worker进程在注册listenfd读事件前抢accept_mutex，抢到互斥锁的那个进程注册listenfd读事件，在读事件里调用accept接受该连接。当一个worker进程在accept这个连接之后，就开始读取请求，解析请求，处理请求，产生数据后，再返回给客户端，最后才断开连接，一个请求，只在一个worker进程中处理。

进程模型好处：对于每个worker进程来说，独立的进程，不需要加锁；采用独立的进程，可以让互相之间不会影响，一个进程退出后，其它进程还在工作，服务不会中断；worker进程的异常退出，会导致当前worker上的所有请求失败，不会影响到所有请求，降低了风险。

nginx采用了异步非阻塞的方式来处理请求，即利用select|poll|epoll|kqueue这样的库函数。它们提供了一种机制，让你可以同时监控多个事件，调用他们是阻塞的，但可以设置超时时间，在超时时间之内，如果有事件准备好了，就返回，当读写返回EAGAIN时，我们将它再次加入到epoll里面（以epoll为例）。

为什么要推荐设置worker的个数为cpu的核数？更多的worker数，只会导致进程来竞争cpu资源了，从而带来不必要的上下文切换。nginx提供了cpu亲缘性的绑定选项,不会因为进程的切换带来cache的失效。

对于一个基本的web服务器来说，事件通常有三种类型，网络事件、信号、定时器。首先，信号的处理，如果nginx正在等待事件（epoll_wait时），如果程序收到信号，在信号处理函数处理完后，epoll_wait会返回错误，然后程序可再次进入epoll_wait调用。定时器的处理，由于epoll_wait等函数在调用的时候是可以设置一个超时时间的，所以nginx借助这个超时时间来实现定时器。nginx里面的定时器事件是放在一颗维护定时器的红黑树里面，每次在进入epoll_wait前，先从该红黑树里面拿到所有定时器事件的最小时间，在计算出epoll_wait的超时时间后进入epoll_wait。所以，当没有事件产生，也没有中断信号时，epoll_wait会超时，定时器事件到了，nginx会检查所有的超时事件，将他们的状态设置为超时，然后再去处理网络事件。