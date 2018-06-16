---
title: Linux shell 脚本参数传递
date: 2018/2/5 10:59:19  
tags:
- Linux
- Shell
---

在 Shell中，调用函数时可以向其传递参数。在函数体内部，通过 n的形式来获取参数的值，例如，1 表示第一个参数，$2 表示第二个参数...

特殊变量	说明
`$0` 当前脚本文件名
`$n` 传递给脚本或函数的参数，n是一个数字，表示第几个参数，$1表示第一个参数，$2表示第二个参数 ...
`$#` 传递给脚本或函数的参数个数
`$*` 传递给脚本或函数的所有参数，当它被双引号（" "）包含时，`"$*"` 会将所有的参数作为一个整体，以`"$1 $2 ... $n"`的形式输出所有参数
`$@` 传递给脚本或函数的所有参数，当它被双引号（" "）包含时，与`$*`稍有不同，`"$@"` 会将各个参数分开，以`"$1" "$2" … "$n"` 的形式输出所有参数
`$*`和`$@`不被双引号(" ")包含时，都以`"$1" "$2" … "$n"` 的形式输出所有参数

另外还有：
`$?` 上一个命令的退出状态，或函数的返回值，如果正常退出则返回0，反之为非0值
`$$` 当前shell进程pid，对于shell脚本，就是这些脚本所在的进程ID
`$!` 上一个命令的pid

写一个测试脚本：
    #!/bin/bash
    echo "\$*=" $*
    echo "\$@=" $@
    
    echo "print each param from \$*" 
    for var in $*
    do
        echo $var
    done
    
    echo "print each param from \$@"
    for var in $@
    do
        echo $var
    done
    
    echo "print each param from \"\$*\""
    for var in "$*"
    do
        echo $var
    done
    
    echo "print each param from \"\$@\""
    for var in "$@"
    do
        echo $var
    done

结果如下：
    # sh test.sh a b c d
    $*= a b c d
    $@= a b c d
    print each param from $*
    a
    b
    c
    d
    print each param from $@
    a
    b
    c
    d
    print each param from "$*"
    a b c d
    print each param from "$@"
    a
    b
    c
    d
