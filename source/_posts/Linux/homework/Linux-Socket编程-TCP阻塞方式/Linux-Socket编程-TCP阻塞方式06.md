---
### Linux-Socket编程-TCP阻塞方式06
---

写一对TCP Socket 的测试程序，分为client 和server，分别运行在不同虚拟机上

*   **测试程序tcp_server6-1，接受client 的连接成功后，进入死循环，死循环中先read 再write反复进行；测试程序tcp_client6-1，连接服务端成功后，也进入死循环，死循环中同样先read 再write 反复进行，此时双方能否正常收发数据？**

    不能

    ![][061]

*   **测试程序tcp_server6-2，接受client 的连接成功后，进入死循环，死循环中先write 再read反复进行；测试程序tcp_client6-2，连接服务端成功后，也进入死循环，死循环中也是先write 再read 反复进行，此时双方能否正常收发数据？**

    1.  假设双方每次read/write 都是1000 字节

    能

    ![][0621]

    2.  假设server 端每次read 1000 字节/write 500 字节，client 端每次read 500 字节write1000 字节
    
    能

    ![][0622]

    3.  假设server 端每次read/write 是1000 字节，client 端每次read/write 是700 字节
    
    能

    ![][0623]

*   **测试程序tcp_server6-3，接受client 的连接成功后，进入死循环，死循环中先write 再read反复进行；测试程序tcp_client6-3，连接服务端成功后，也进入死循环，死循环中先read再write 反复进行，此时双方能否正常收发数据？**

    1.  假设双方每次read/write 都是1000 字节

    能

    ![][0631]

    2.  假设server 端每次read 1000 字节/write 500 字节，client 端每次read 500 字节write1000 字节
    
    能

    ![][0632]

    3.  假设server 端每次read/write 是1000 字节，client 端每次read/write 是700 字节
    
    能

    ![][0633]

*   **测试程序tcp_server6-4，接受client 的连接成功后，进入死循环，死循环中先read 再write反复进行；测试程序tcp_client6-4，连接服务端成功后，也进入死循环，死循环中先write再read 反复进行，此时双方能否正常收发数据？**

    1.  假设双方每次read/write 都是1000 字节
    
    能

    ![][0641]

    2.  假设server 端每次read 1000 字节/write 500 字节，client 端每次read 500 字节write1000 字节
    
    能

    ![][0642]

    3.  假设server 端每次read/write 是1000 字节，client 端每次read/write 是700 字节
    
    能

    ![][0643]

[061]: Linux-Socket编程-TCP阻塞方式06/061.jpg
[0621]: Linux-Socket编程-TCP阻塞方式06/0621.jpg
[0622]: Linux-Socket编程-TCP阻塞方式06/0622.jpg
[0623]: Linux-Socket编程-TCP阻塞方式06/0623.jpg
[0631]: Linux-Socket编程-TCP阻塞方式06/0631.jpg
[0632]: Linux-Socket编程-TCP阻塞方式06/0632.jpg
[0633]: Linux-Socket编程-TCP阻塞方式06/0633.jpg
[0641]: Linux-Socket编程-TCP阻塞方式06/0641.jpg
[0642]: Linux-Socket编程-TCP阻塞方式06/0642.jpg
[0643]: Linux-Socket编程-TCP阻塞方式06/0643.jpg
