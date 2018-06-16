---
### Linux-Socket编程-TCP阻塞方式04
---

写一对TCP Socket 的测试程序，分为client 和server，分别运行在不同虚拟机上

*   **测试程序tcp_server4-1，接受client 的连接成功后，用read 函数一次读20 字节（此时对
方无输入，应进入阻塞状态）**

*   **测试程序tcp_client4-1-1，连接服务端成功后，用write 函数向服务端一次性写入超过
20 字节的内容，观察sevrer 端read 函数的返回值和返回内容，是否与client 端写入内
容一致**

    内容一致，测试截图：

    ![][041]

*   **测试程序tcp_client4-1-2，连接服务端成功后，用write 函数向服务端写入，要求每次
写2 字节，然后延时1 秒，再写2 字节…，观察server 端read 函数的返回值和返回内
容，是否与client 端写入内容一致**

    内容不一致，读取一次后读到2字节，执行后续程序，终止。测试截图：

    ![][042]

*   **测试程序tcp_server4-2/tcp_client_4-2-1/tcp-client_4-2-2 ， 将read/write 换成
recv/send 函数，用法是否相同？结果是否相同？**

    此时问题分情况。

    tcp_4-2-1 tcp_4-2-2中send函数的flags均设置为0；

    当tcp_server4-2中recv函数的flags设置为0时，结果如下：

    ![][043]

    当tcp_server4-2中recv函数的flags设置为MSG_WAITALL时，结果如下：

    ![][044]

*   **如果要求read/recv 函数执行后，不读满20 字节一直不返回(client 端要求每次写2 字节，
然后延时1 秒，再写2 字节…)，能否做到？如何做到？(注：不允许采用自己写循环保证读
满20 字节）**

    recv函数能做到，方法如下：

        recv(connect_fd, buff, 20, MSG_WAITALL)

*   **给出read/recv 函数的使用区别，给出write/send 函数的使用区别**

    read/write与recv/send的区别

    *   在功能上，read/write是recv/send的子集。read/wirte是更通用的文件描述符操作，而recv/send在socket领域则更“专业”一些。
    *   当recv/send的flag参数设置为0时，则和read/write是一样的。
    *   如果有如下几种需求，则read/write无法满足，必须使用recv/send:
        *   为接收和发送进行一些选项设置
        *   从多个客户端中接收报文
        *   发送带外数据（out-of-band data)

[041]:  Linux-Socket编程-TCP阻塞方式04/041.jpg
[042]:  Linux-Socket编程-TCP阻塞方式04/042.jpg
[043]:  Linux-Socket编程-TCP阻塞方式04/043.jpg
[044]:  Linux-Socket编程-TCP阻塞方式04/044.jpg