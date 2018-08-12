---
title: Linux-rpm包的制作
date: 2017/10/22 11:21:19 
tag: 
- Linux
---

---

#### RPM相关知识准备

----
[官方文档](https://fedoraproject.org/wiki/How_to_create_an_RPM_package/zh-cn#RPM_.E5.9F.BA.E7.A1.80.E7.9F.A5.E8.AF.86)

##### RPM 基础知识

若要构建一个标准的 RPM 包，您需要创建 .spec 文件，其中包含软件打包的全部信息。然后，对此文件执行 rpmbuild 命令，经过这一步，系统会按照步骤生成最终的 RPM 包。

一般情况，您应该把源代码包，比如由开发者发布的以 .tar.gz 结尾的文件，放入 ~/rpmbuild/SOURCES 目录。将.spec 文件放入 ~/rpmbuild/SPECS 目录，并命名为 "软件包名.spec" 。当然， 软件包名 就是最终 RPM 包的名字。为了创建二进制（Binary RPM）和源码软件包（SRPM），您需要将目录切换至 ~/rpmbuild/SPECS 并执行：

     $ rpmbuild -ba NAME.spec   

当执行此命令时，rpmbuild 会自动读取 .spec 文件并按照下表列出的步骤完成构建。下表中，以 % 开头的语句为预定义宏，每个宏的作用如下：  

![][rpm1]

在 rpmbuild 中，对上表中的每个宏代码都有对应的目录：

![][rpm2]

如果某一阶段失败，请查看输出信息以了解失败原因，并根据需要修改 .spec 文件。

做好准备打包一个特殊程序

如果这里有特殊的程序，它们需要被安装或者运行以便让您打包的普通程序正常工作，那么请先安装它们，然后记录下诸如软件包等相关信息。

不可以使用预编译代码进行打包。将源代码（通常是 .tar.gz 文件）放入 "~/rpmbuild/SOURCES" 目录（注意用户）。

仔细阅读该软件的安装说明。我们建议您先手工安装一次以了解具体情况。除少数情况外，所有二进制文件和程序库都必须由源码包中的源码编译而成。

##### 新建一个 .spec 文件

现在，您需要在 ~/rpmbuild/SPECS 目录下，新建一个 SPEC 文件。文件应命名为 "软件包名.spec"。名称根据软件包名或通用名填写即可。但是，必须要遵守 软件包命名规定。  

如果您首次创建 .spec 文件，vim 或 emacs 会自动生成模板：

     $ cd ~/rpmbuild/SPECS
     $ vim program.spec

示例（仅供参考）：   

    Name:
    Version:
    Release:	1%{?dist}
    Summary:
    Group:
    License:
    URL:
    Source0:
    BuildRoot:	%{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
    
    BuildRequires:
    Requires:
    
    %description
    
    %prep
    %setup -q
    
    %build
    %configure
    make %{?_smp_mflags}
    
    %install
    rm -rf %{buildroot}
    make install DESTDIR=%{buildroot}
    
    %clean
    rm -rf %{buildroot}
    
    %files
    %defattr(-,root,root,-)
    %doc
    
    %changelog
    
您可以使用 $RPM_BUILD_ROOT 代替 %{buildroot}，两者都可以使用。
您也可以使用 rpmdev-newspec 命令来创建 SPEC 文件。rpmdev-newspec 软件包名 可以创建一个初始 SPEC 文件，该工具从软件包名判断使用哪个模板，支持指定模板。 /etc/rpmdevtools/spectemplate-*.spec 包含所有可用的模板，使用 rpmdev-newspec --help 命令了解更多信息。例如，为 python 模块创建 SPEC 文件：
    
    cd ~/rpmbuild/SPECS
    rpmdev-newspec python-antigravity
    vi python-antigravity.spec

##### SPEC 文件综述

您需要遵守这些规定：软件包命名规定，打包规定 和 软件包审核规定。

"#" 字符表示注释，但需要避免注释宏（以 % 开头），因为它们会首先被替换展开。使用 %% 注释宏。另外，还要避免在脚本命令的相同行中使用行内注释。

以下介绍了主要的标签。注意 %{name}，%{version} 和 %{release} 代表 Name, Version 和 Release 这三个标签。只要更改标签，宏就会使用新值。

![][rpm3]
![][rpm4]

##### SPEC 文件剖析

*   **%prep部分**

    描述了解压源码包的方法。一般而言，其中包含 "%autosetup" 命令。另外，还可以使用 "%setup" 和 "%patch" 命令来指定操作 Source0 等标签的文件。

*   **%prep 部分：%autosetup 命令**

    "%autosetup" 命令用于解压源码包。可用选项包括：  
    -n name : 如果源码包解压后的目录名称与 RPM 名称不同，此选项用于指定正确的目录名称。例如，如果 tarball 解压目录为 FOO，则使用 "%autosetup -n FOO"。   
    -c name : 如果源码包解压后包含多个目录，而不是单个目录时，此选项可以创建名为 name 的目录，并在其中解压。    
    如果使用 "%setup" 命令，通常使用 -q' 抑止不必要的输出。

*   **%build 部分**

    "%build" 部分有时会有点复杂；在这里你可以配置，并编译用于安装的文件。     
    许多程序使用 GNU configure 进行配置。默认情况下，文件会安装到前缀为 "/usr/local" 的路径下，对于手动安装很合理。然而，打包时需要修改前缀为 "/usr"。共享库路径视架构而定，安装至 /usr/lib 或 /usr/lib64 目录。     
    由于 GNU configure 很常见，可使用 "%configure" 宏来自动设置正确选项（例如，设置前缀为 /usr）。一般用法如下：  
   
         %configure
         make %{?_smp_mflags}

    若需要覆盖 makefile 变量，请将变量作为参数传递给 make：

        make %{?_smp_mflags} CFLAGS="%{optflags}" BINDIR=%{_bindir}

*   **%install 部分**

    此部分包含安装阶段需要执行的命令，即从 %{_builddir} 复制相关文件到 %{buildroot} 目录（通常表示从 ~/rpmbuild/BUILD 复制到 ~/rpmbuild/BUILDROOT) 目录，并根据需要在 %{buildroot} 中创建必要目录。

    容易混淆的术语：    
    "build 目录"，也称为 %{_builddir}，实际上与 "build root"，又称为 %{buildroot}，是不同的目录。在前者中进行编译，并将需要打包的文件从前者复制到后者。   
    在 %build 阶段，当前目录为 %{buildsubdir}，是 %prep 阶段中在 %{_builddir} 下创建的子目录。这些目录通常名为 ~/rpmbuild/BUILD/%{name}-%{version}。    
     %install 阶段的命令不会在用户安装 RPM 包时执行，此阶段仅在打包时执行。 
    一般，这里执行 "make install" 之类的命令：   

        %install
        rm -rf %{buildroot} # 仅用于 RHEL 5
        %make_install

    理想情况下，对于支持的程序，你应该使用 %make_install，它等同于 DESTDIR=%{buildroot}，它会将文件安装到 %{buildroot} 目录中。

##### Scriptlets

当用户安装 RPM 时，您可能想要执行一些命令。这可以通过 scriptlets 完成。请查看 Packaging/ScriptletSnippets。

脚本片段可以：

*   在软体包安装之前 (%pre) 或之后 (%post) 执行
*   在软体包卸载之前 (%preun) 或之后 (%postun) 执行
*   在事务开始 (%pretrans) 或结束 (%posttrans) 时执行

---

#### test-1453381.rpm包的制作过程

----

1.  开始前的准备

    安装rpmbuild软件包

        yum -y install rpm-build
    
    Linux RedHat 7.4已经安装了。

2.  生成相关目录

        mkdir -p $(rpmDir)/SOURCES $(rpmDir)/SPECS $(rpmDir)/BUILD $(rpmDir)/RPMS $(rpmDir)/SRPMS

    目录结构如下：

        rpmbuild/
        ├── BUILD
        ├── BUILDROOT
        ├── RPMS
        ├── SOURCES
        ├── SPECS
        └── SRPMS

3.  编写.spec文件

        Name:	test-1453381
        Version:	1.0
        Release:	1%{?dist}
        Summary:	test-rpm
        
        Group:		none
        License:	none
        URL:		none
        Source0:	test-1453381-1.0.tar.gz
        
        %description
        test-1453381-rpm
        
        %prep
        %setup -q 
        
        %pre
        echo "准备安装test-1453381"
        
        %post
        echo "完成安装test-1453381"
        
        %build
        make
        
        %install
        make install DESTDIR=%{buildroot}
        
        %preun
        echo "准备卸载test-1453381"
        
        %postun
        cd %{_builddir}/%{name}-%{version}/
        make uninstall 
        echo "完成卸载test-1453381"
        
        %files
        %doc
        
        %changelog

4.  拷贝相关文件到指定目录

        mkdir $(releaseName)
    	cp $(rpmSrcFile) $(releaseName)/
    	tar -zcvf $(rpmDir)/SOURCES/$(rpmGz) $(releaseName)
    	rm -rf $(releaseName)
    	cp $(rpmSpec) $(rpmDir)/SPECS

5.  生成rpm包

        rpmbuild -bb $(rpmDir)/SPECS/$(rpmSpec)
    	cp $(rpmDir)/RPMS/x86_64/$(releaseName)-1.el7.x86_64.rpm $(binFinal).rpm

6.  测试

    ![][rpmp1]

    ![][rpmp2]

[rpm1]: Linux-rpm制作/rpm1.jpg
[rpm2]: Linux-rpm制作/rpm2.jpg
[rpm3]: Linux-rpm制作/rpm3.jpg
[rpm4]: Linux-rpm制作/rpm4.jpg
[rpmp1]: Linux-rpm制作/rpmp1.jpg
[rpmp2]: Linux-rpm制作/rpmp2.jpg

可供参考：[链接](http://www.cnblogs.com/postgres/p/5726339.html)
