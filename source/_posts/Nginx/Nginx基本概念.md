---
title: nginx基础概念
date: 2018/1/29 20:23:42  
tags:
- Nginx
---

### connection
在nginx中connection就是对tcp连接的封装，其中包括连接的socket，读事件，写事件。nginx连接上限与系统对fd的限制不一样。nginx在实现时，是通过一个连接池来管理的，每个worker进程都有一个独立的连接池，连接池的大小是worker_connections。连接池里面保存的其实不是真实的连接，它只是一个worker_connections大小的一个ngx_connection_t结构的数组。nginx会通过一个链表free_connections来保存所有的空闲ngx_connection_t，每次获取一个连接时，就从空闲连接链表中获取一个，用完后，再放回空闲连接链表里面。（worker_connections表示每个worker进程所能建立连接的最大值）

### request
nginx指的是http请求，具体到nginx中的数据结构是ngx_http_request_t，是对一个http请求的封装。处理流程图：
![](http://tengine.taobao.org/book/_images/chapter-2-2.PNG)

### 请求相关的概念

### keepalive
http请求是基于TCP协议之上的，客户端在发起请求前，需要先与服务端建立TCP连接，需要三次握手来确定的，三次交互也会带来网络流量。http请求是请求应答式的，如果能知道每个请求头与响应体的长度，可以在一个连接上面执行多个请求的，这就是长连接。

### http协议中关于响应body长度的确定：
对于http1.0协议，如果响应头中有content-length头，则以content-length的长度就可以知道body的长度了，客户端在接收body时，就可以依照这个长度来接收数据。而如果没有content-length头，则客户端会一直接收数据，直到服务端主动断开连接，才表示body接收完了。
对于http1.1协议，如果响应头中的Transfer-encoding为chunked传输，则表示body是流式输出，body会被分成多个块，每块的开始会标识出当前块的长度，此时，body不需要通过长度来指定。如果是非chunked传输，而且有content-length，则按照content-length来接收数据。否则，如果是非chunked，并且没有content-length，则客户端接收数据，直到服务端主动断开连接。

当服务端在输出完body之后，会可以考虑使用长连接。如果客户端的请求头中的connection为close，则表示客户端需要关掉长连接；如果为keep-alive，则客户端需要打开长连接；如果客户端的请求中没有connection这个头，那么根据协议，http1.0默认为close，http1.1默认为keep-alive。如果结果为keepalive，nginx在输出完响应体后，会设置当前连接的keepalive属性，然后等待客户端下一次请求。当然，nginx不可能一直等待下去，当nginx设置了keepalive等待下一次的请求时，同时也会设置一个最大等待时间，这个时间是通过选项keepalive_timeout来配置的，如果配置为0，则表示关掉keepalive，强制为close。

### pipe
在http1.1中，引入了一种新的特性，即pipeline。它可以看作为keepalive的一种升华，因为pipeline也是基于长连接的，目的就是利用一个连接做多次请求。如果客户端要提交多个请求，对keepalive来说，第二个请求必须要等到第一个请求的响应接收完全后，才能发起，这和TCP的停止等待协议是一样的，得到两个响应的时间至少为2*RTT。对pipeline来说，客户端不必等到第一个请求处理完后，就可以马上发起第二个请求，得到两个响应的时间可能能够达到1*RTT。
nginx对pipeline中的多个请求的处理却不是并行的，依然是一个请求接一个请求的处理，只是在处理第一个请求的时候，客户端就可以发起第二个请求。nginx利用pipeline减少了处理完一个请求后等待第二个请求的请求头数据的时间。nginx在读取数据时，会将读取的数据放到一个buffer里面，如果nginx在处理完前一个请求后，如果发现buffer里面还有数据，就认为剩下的数据是下一个请求的开始，然后就接下来处理下一个请求，否则就设置keepalive。

### lingering_close
lingering_close就是延迟关闭，当nginx要关闭连接时，并非立即关闭连接，而是先关闭tcp连接的写，再等待一段时间后再关掉连接的读。考虑以下场景：nginx在接收客户端的请求时，可能由于客户端或服务端出错了，要立即响应错误信息给客户端，而nginx在响应错误信息后，大分部情况下是需要关闭当前连接。nginx执行完write()系统调用把错误信息发送给客户端，write()系统调用返回成功并不表示数据已经发送到客户端，有可能还在tcp连接的write buffer里。接着如果直接执行close()系统调用关闭tcp连接，内核会首先检查tcp的read buffer里有没有客户端发送过来的数据留在内核态没有被用户态进程读取，如果有则发送给客户端RST报文来关闭tcp连接丢弃write buffer里的数据，如果没有则等待write buffer里的数据发送完毕，然后再经过正常的4次分手报文断开连接。所以,当在某些场景下出现tcp write buffer里的数据在write()系统调用之后到close()系统调用执行之前没有发送完毕，且tcp read buffer里面还有数据没有读，close()系统调用会导致客户端收到RST报文且不会拿到服务端发送过来的错误信息数据。
