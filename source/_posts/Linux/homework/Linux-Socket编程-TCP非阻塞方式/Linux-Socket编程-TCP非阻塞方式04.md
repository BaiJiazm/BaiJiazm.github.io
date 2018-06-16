---

### Linux-Socket编程-TCP非阻塞方式04

---

*   **测试程序tcp_server4-1，接受client 的连接成功后，用getchar()进入暂停运行状态**

*   **测试程序tcp_client4-1，连接服务端成功后，用write 向服务端不断写入，直到write 失败为止，用netstat 观察读写队列的值**

    server端的recv-Q缓冲区达到一定大小时，不再接受，client端在server端recv-Q满后，写入到send-Q中，达到一定大小时，不能继续写入，select函数阻塞。

    server端的recv-Q+client端的send-Q等于total write bytes。

    测试截图：

    ![][041]

*   **测试程序tcp_server4-2，接受client 的连接成功后，用每读20 字节就延时1 秒的方法循环读数据**
*   **测试程序tcp_client4-2，连接服务端成功后，用write 向服务端不断大量写入，直到write失败为止，用netstat 观察读写队列的值，write 失败后，如何重新恢复为继续写状态？**

    client向server不断大量写入后，缓冲区慢，write失败。这时只有当server读取一定数据后，使得client发送缓冲区留有空间大于write的低水位大小时，select返回有效，可继续写入。

    测试截图：

    ![][042]

[041]: Linux-Socket编程-TCP非阻塞方式04/041.jpg
[042]: Linux-Socket编程-TCP非阻塞方式04/042.jpg