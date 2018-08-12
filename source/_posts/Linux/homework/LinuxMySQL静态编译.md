---
title: Linux-MySQL的静态编译
date: 2017/10/8 23:26:12 
tag:
- Linux
---

---

1.  **准备**

    1.1 **libc 静态库文件**  
    若缺少libc静态库文件，则会失败，如下信息。
    
    ![][1]

    安装如下：
    首先下载相应安装包，[下载地址](https://centos.pkgs.org/7/centos-x86_64/glibc-static-2.17-196.el7.x86_64.rpm.html)，放置到任意目录下（此处为/home），进入该目录，

    执行如下命令，升级安装完成。

    ![][2]

    可查看系统安装的GLIBC包，如下

    ![][3]

    1.2 **获取libmysqlclient.a静态库**  
    **下载源码编译方式**

    获取MariaDB源码
    下载相应源码，下载地址https://downloads.mariadb.org/mariadb/5.5.56/ 下载后传到虚拟机中，解压然后编译。
    
    ![][4]

    准备安装环境

        yum –y groupinstall Development tools
        yum –y install ncurses-devel
        yum –y install cmake
    
    编译MariaDB得到libmysqlclient.a

        tar –zxvf mariadb-5.5.56.tar.gz
        cd mariadb-5.5.56
        cmake . –LH		#查看cmake的可用选项
        cmake . -DCMAKE_INSTALL_PREFIX=/home/mysql
                -DMYSQL_DATADIR=/home/mysqldata -DSYSCONFDIR=/etc 
                -DWITHOUT_TOKUDB=1 -DMYSQL_UNIX_ADDR=/home/tmp/mysql.sock 
                -DDEFAULT_CHARSET=gbk -DDEFAULT_COLLATION=gbk_general_ci
        make
        make install
    	
    完成上述命令后，在/home/mysql/lib/目录下可得到libmysqlclient.a静态库文件
    
    **直接获取libmysqlclient.a方式**  
    下载编译后的MariaDB文件，[下载地址](https://downloads.mariadb.org/mariadb/5.5.56/)
    
    ![][5]

    解压

        tar -zxvf mariadb-5.5.56-linux-x86_64.tar.gz

    在mariadb-5.5.56-linux-x86_64目录下的lib文件夹下得到libmysqlclient.a文件
    
    ![][6]

2. **编译**

    **编译说明：**  

        这里采用的是半静态半动态编译方法。libmysqlclient.a库用静态连接，一些常用的库用动态连接。
        因为程序是运行在linux中的，常用库系统默认都会有的。  
    
    **编译命令：**  

        g++ mysql_demo.cpp -o mysql_demo -I/usr/include/mysql -Wl,-dn -L. 
         -lmysqlclient  -Wl,-dy -lpthread -lm -ldl -lcrypt

    **命令解释：**  

        -Idir
        编译时优先在选项后的目录中查找包含的头文件,然后将到系统缺省的头文件目录查找
        -Ldir
        指定编译搜索库的路径
        -llibrary
        制定编译的时候使用的库
        -Wl,-dn
        后面是静态链接
        -Wl,-dy
        后面是动态连接

    **库解释：** 

        -lpthread
        使用POSIX线程库添加对多线程的支持
        -lm 
        链接的是数学函数库
        -ldl
        显式加载动态库的动态函数库（dlopen(),dlclose() , dlerror() , dlsym()）

3.  **运行**

    ln命令用来为文件创件连接，连接类型分为硬连接和符号连接两种，默认的连接类型是硬连接。如果要创建符号连接必须使用"-s"选项。

        ln -s /var/lib/mysql/mysql.sock /tmp/mysql.sock

    ![][7]

[1]: LinuxMySQL静态编译/1.png
[2]: LinuxMySQL静态编译/2.jpg
[3]: LinuxMySQL静态编译/3.png
[4]: LinuxMySQL静态编译/4.png
[5]: LinuxMySQL静态编译/5.jpg
[6]: LinuxMySQL静态编译/6.png
[7]: LinuxMySQL静态编译/7.jpg
