---
title: Linux-service自定义
date: 2017/10/22 11:20:37  
tag: Linux
---

---

##### 制作service的基本知识

1.  **服务权限**

    systemd有系统和用户区分；系统（`/user/lib/systemd/system/`）、用户（`/etc/lib/systemd/user/`）.一般系统管理员手工创建的单元文件建议存放在`/etc/systemd/system/`目录下面。

2.  **创建服务文件**

    **[Unit]**  
    **Description** : 服务的简单描述  
    **Documentation** ： 服务文档  
    **Before、After** :定义启动顺序。Before=xxx.service,代表本服务在xxx.service启动之前启动。After=xxx.service,代表本服务在xxx.service之后启动。  
    **Requires**：这个单元启动了，它需要的单元也会被启动；它需要的单元被停止了，这个单元也停止了。  
    **Wants**：这个单元启动了，它需要的单元也会被启动；它需要的单元被停止了，对本单元没有影响。

    **[Service]**  
    **Type=simple（默认值）**：systemd认为该服务将立即启动。服务进程不会fork。如果该服务要启动其他服务，不要使用此类型启动，除非该服务是socket激活型。  
    **Type=forking**：systemd认为当该服务进程fork，且父进程退出后服务启动成功。对于常规的守护进程（daemon），除非你确定此启动方式无法满足需求，使用此类型启动即可。使用此启动类型应同时指定 PIDFile=，以便systemd能够跟踪服务的主进程。  
    **Type=oneshot**：这一选项适用于只执行一项任务、随后立即退出的服务。可能需要同时设置 RemainAfterExit=yes 使得 systemd 在服务进程退出之后仍然认为服务处于激活状态。  
    **Type=notify**：与 Type=simple 相同，但约定服务会在就绪后向 systemd 发送一个信号。这一通知的实现由 libsystemd-daemon.so 提供。  
    **Type=dbus**：若以此方式启动，当指定的 BusName 出现在DBus系统总线上时，systemd认为服务就绪。  
    **Type=idle**: systemd会等待所有任务(Jobs)处理完成后，才开始执行idle类型的单元。除此之外，其他行为和Type=simple 类似。   
    **PIDFile**：pid文件路径  
    **ExecStart**：指定启动单元的命令或者脚本，ExecStartPre和ExecStartPost节指定在ExecStart之前或者之后用户自定义执行的脚本。Type=oneshot允许指定多个希望顺序执行的用户自定义命令。   
    **ExecReload**：指定单元停止时执行的命令或者脚本。  
    **ExecStop**：指定单元停止时执行的命令或者脚本。  
    **PrivateTmp**：True表示给服务分配独立的临时空间   
    **Restart**：这个选项如果被允许，服务重启的时候进程会退出，会通过systemctl命令执行清除并重启的操作。    
    **RemainAfterExit**：如果设置这个选择为真，服务会被认为是在激活状态，即使所以的进程已经退出，默认的值为假，这个选项只有在Type=oneshot时需要被配置。   

    **[Install]**  
    **Alias**：为单元提供一个空间分离的附加名字。     
    **RequiredBy**：单元被允许运行需要的一系列依赖单元，RequiredBy列表从Require获得依赖信息。    
    **WantBy**：单元被允许运行需要的弱依赖性单元，Wantby从Want列表获得依赖信息。    
    **Also**：指出和单元一起安装或者被协助的单元。     
    **DefaultInstance**：实例单元的限制，这个选项指定如果单元被允许运行默认的实例。   

---
##### Linux test-1453381.service实际制作

1.  编写service文件

    test-1453381.service

        # cp test-1453381.service /etc/systemd/system/
        
        [Unit]
        Description=test-1453381
        
        [Service]
        Type=forking
        #下面这行对于此次的test.service需要注释掉
        #PIDFile=/usr/local/test-1453381.pid
        ExecStart=/usr/sbin/test-1453381 &
        ExecReload=/bin/kill -s HUP $MAINPID
        ExecStop=/bin/kill -s QUIT $MAINPID
        
        [Install]
        WantedBy=multi-user.target

    为避免`systemctl start ` 时遇到`Warning: test-1453381.service changed on disk. Run 'systemctl daemon-reload' to reload units.`,可在start前先执行 `systemctl enable teset.service` 。

2.  拷贝编写好的service文件至特定目录

        cp test-1453381.service /etc/systemd/system/

3.  测试

    ![][1]

参考链接：

*   [参考1](https://www.freedesktop.org/software/systemd/man/systemd.service.html#)
*   [参考2](https://wiki.archlinux.org/index.php/systemd_(%E7%AE%80%E4%BD%93%E4%B8%AD%E6%96%87))
*   [参考3](http://blog.chinaunix.net/uid-26790551-id-3189115.html)


[1]: Linux-service自定义/1.jpg