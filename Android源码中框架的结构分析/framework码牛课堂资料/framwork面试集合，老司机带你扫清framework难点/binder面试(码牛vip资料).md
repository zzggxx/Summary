> 面试官: 谈一谈Binder的原理和实现一次拷贝的流程

> 心理分析：能问出该问题，面试官对binder的理解是非常深入的。想问求职者对Android底层有没有深入理解

> 求职者:应该从linux进程通信原理的两次拷贝说起，然后引申为什么binder却只有一次拷贝 ，最后阐述内核空间 与用户空间的定义

1 Linux 下传统的进程间通信原理

了解 Linux IPC 相关的概念和原理有助于我们理解 Binder 通信原理。因此，在介绍 Binder 跨进程通信原理之前，我们先聊聊 Linux 系统下传统的进程间通信是如何实现。

1.1 基本概念介绍

这里我们先从 Linux 中进程间通信涉及的一些基本概念开始介绍，然后逐步展开，向大家说明传统的进程间通信的原理。

![腾讯面试题——谈一谈Binder的原理和实现一次拷贝的流程](http://p3.pstatp.com/large/pgc-image/3ca508047d374f48b55e45f4693ceaa7)

 

上图展示了 Liunx 中跨进程通信涉及到的一些基本概念：

- 进程隔离
- 进程空间划分：用户空间(User Space)/内核空间(Kernel Space)
- 系统调用：用户态/内核态

进程隔离

简单的说就是操作系统中，进程与进程间内存是不共享的。两个进程就像两个平行的世界，A 进程没法直接访问 B 进程的数据，这就是进程隔离的通俗解释。A 进程和 B 进程之间要进行数据交互就得采用特殊的通信机制：进程间通信（IPC）。

进程空间划分：用户空间(User Space)/内核空间(Kernel Space)



**面试官**：谈谈你对 binder 的理解

🤔️：binder 是 Android 中主要的跨进程通信方式，binder 驱动和 service manager 分别相当于网络协议中的路由器和 DNS，并基于 mmap 实现了 IPC 传输数据时只需一次拷贝。

binder 包括 BinderProxy、BpBinder 等各种 Binder 实体，以及对 binder 驱动操作的 ProcessState、IPCThreadState 封装，再加上 binder 驱动内部的结构体、命令处理，整体贯穿 Java、Native 层，涉及用户态、内核态，往上可以说到 Service、AIDL 等，往下可以说到 mmap、binder 驱动设备，是相当庞大、繁琐的一个机制。

我自己来谈的话，一天时间都不够，还是问我具体的问题吧。

**面试官**：基于 mmap 又是如何实现一次拷贝的？

🤔️：其实很简单，我来画一个示意图吧：

![img](F:/BaiduNetdiskDownload/Binder机制详解（驱动层）/资料/Binder文章/2.jpg)

Client 与 Server 处于不同进程有着不同的虚拟地址规则，所以无法直接通信。而一个页框可以映射给多个页，那么就可以将一块物理内存分别与 Client 和 Server 的虚拟内存块进行映射。

如图， Client 就只需 copy_from_user 进行一次数据拷贝，Server 进程就能读取到数据了。另外映射的虚拟内存块大小将近 1M (1M-8K)，所以 IPC 通信传输的数据量也被限制为此值。

**面试官**：怎么理解页框和页？ 

🤔️：页框是指一块实际的物理内存，页是指程序的一块内存数据单元。内存数据一定是存储在实际的物理内存上，即页必然对应于一个页框，页数据实际是存储在页框上的。

页框和页一样大，都是内核对内存的分块单位。一个页框可以映射给多个页，也就是说一块实际的物理存储空间可以映射给多个进程的多个虚拟内存空间，这也是 mmap 机制依赖的基础规则。

**面试官**：简单说下 binder 的整体架构吧

🤔️：再来画一个简单的示意图吧，这是一个比较典型的、两个应用之间的 IPC 通信流程图：

![img](https://mmbiz.qpic.cn/sz_mmbiz_png/af0927004qrz9gGbgaNC9Qe4ec8rbR1ibfPBPIyHNnQ64saicm2GD09OeUnSg5ZiaxCxiaDickCyibRyuZYBfF4FGibtQ/640?wx_fmt=png&tp=webp&wxfrom=5&wx_lazy=1&wx_co=1)

​		Client 通过 ServiceManager 或 AMS 获取到的远程 binder 实体，一般会用 **Proxy** 做一层封装，比如 ServiceManagerProxy、 AIDL 生成的 Proxy 类。而被封装的远程 binder 实体是一个 **BinderProxy**。

​	**BpBinder** 和 BinderProxy 其实是一个东西：远程 binder 实体，只不过一个 Native 层、一个 Java 层，BpBinder 内部持有了一个 binder 句柄值 handle。

​	**ProcessState** 是进程单例，负责打开 Binder 驱动设备及 mmap；**IPCThreadState** 为线程单例，负责与 binder 驱动进行具体的命令通信。

​		由 Proxy 发起 transact() 调用，会将数据打包到 Parcel 中，层层向下调用到 BpBinder ，在 BpBinder 中调用 IPCThreadState 的 transact() 方法并传入 handle 句柄值，IPCThreadState 再去执行具体的 binder 命令。

​		由 binder 驱动到 Server 的大概流程就是：Server 通过 IPCThreadState 接收到 Client 的请求后，层层向上，最后回调到 **Stub** 的 onTransact() 方法。

​		当然这不代表所有的 IPC 流程，比如 Service Manager 作为一个 Server 时，便没有上层的封装，也没有借助 IPCThreadState，而是初始化后通过 binder_loop() 方法直接与 binder 驱动通信的。



