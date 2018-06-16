---
title: Github新建本地仓库并同步远程仓库
date: 2018/2/8 21:09:20 
tags:
- Github
---

### 如何直接在本地创建仓库并提交到远程仓库?

前提：在github上手动创建仓库gitRepo，且没有其它任何提交，包括readme.md。

**步骤如下：**

```shell
mkdir gitRepo #如果是已存在的工程项目，则直接cd到项目根目录下，不需要新建。
cd gitRepo
git init #初始化本地仓库
git add xxx #添加要push到远程仓库的文件或文件夹
git commit -m ‘first commit’
git remote add origin https://github.com/yourgithubID/gitRepo.git #建立远程仓库
git push -u origin master #将本地仓库push到远程仓库
```

**有关问题：**

* 如果没有创建gitRepo，则会报以下错误: `ERROR:Repository not found.`

* 由于在github上手动创建的仓库包括：README.md文件，本地仓库没有此文件，则在执行git push -u origin master命令时报如下的错误: `ERROR:failed to push some refs to ...`
解决方法为：
第一步：可以通过如下命令进行代码合并[注：pull=fetch+merge]
    `git pull --rebase origin master`
执行上面代码后可以看到本地代码库中多了README.md文件
第二步：此时再执行语句 `git push -u origin master`即可完成代码上传到github