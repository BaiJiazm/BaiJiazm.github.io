---
title: Git多个网站账户配置 Mac Linux
date: 2018/11/24 14:09:20 
tags:
- Git
---

## 应用场景
我们经常将代码托管到github、gitlab这样的网站上。在实际开发中，往往要将代码托管到多个不同的网站上。比如：公司的代码需要托管到Gitlab上，自己的开源代码托管到GitHub上，私有代码托管到gitlab上等等，每个托管网站都对应一个git账户。

为了避免每次push代码时都要输入用户名和密码，通常会选择使用ssh协议，将公钥保存到托管网站上。下面介绍如何在一个Git终端中配置多个账户，同时管理多个托管网站的代码。

## 配置方法

### 1.生成密钥
可以为不同网站生成不同密钥，也可以所有网站共用同一个密钥。为了简单方便，我用同一密钥在不同网站使用。

一般情况下用邮箱生成密钥，

```shell
cd ~/.ssh	#进入保存秘钥的目录
ssh-keygen -t rsa -C "xx@xx.com"	#用邮箱生成密钥
	#出现以下提示：
Generating public/private rsa key pair.
Enter file in which to save the key (~/.ssh/id_rsa): #秘钥命名，回车默认id_rsa
	#直接进行回车
ls	#查看密钥文件，生成完毕
cat id_rsa.pub	#查看公钥
```
密钥对id_rsa和id_rsa.pub。其中id_rsa.pub是公钥。

### 2.公钥添加到托管网站
以GitHub为例，登录GitHub，点击右上角头像选择settings，在打开的页面中选择SSH and GPG keys。在打开的页面的Key输入框中粘贴复制本地的公钥，然后点击下方的Add SSH key按钮。

使用测试命令测试：
```
ssh -T git@github.com
```

### 3.config配置
在Mac系统下，config文件在`~/.ssh/config`,

```shell
vim ~/.ssh/config
Host gitlab.com
HostName gitlab.com
User xxx

Host github.com
Hostname github.com
User xxx
```

### 4.具体仓库操作
不同仓库直接操作，不会影响。

例如Github仓库：`git clone git@github.com:xx/xx.git`
例如Gitlab仓库：`git clone git@gitlab.com:xx/xx.git`

## 参考
* [git配置多个账户](https://my.oschina.net/ironwill/blog/796348)
* [Mac下配置多个Git账户](https://segmentfault.com/a/1190000016269686)