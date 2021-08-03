# ProcessState和线程池

   Android系统启动完成后，ActivityManager, PackageManager等各大服务都运行在system_server进程，

app应用需要使用系统服务都是通过binder来完成进程之间的通信。

   对于binder线程是如何管理的呢，又是如何创建的呢？其实无论是system_server进程，还是app进程，

都是在进程fork完成后，便会在新进程中执行onZygoteInit()的过程中，启动binder线程池。

  Binder线程创建与其所在进程的创建中产生，Java层进程的创建都是通过Process.start()方法，向Zygote

进程发出创建进程的socket消息，Zygote收到消息后会调用Zygote.forkAndSpecialize()来fork出新进程，

在新进程中会调用到RuntimeInit.nativeZygoteInit方法，该方法经过jni映射，最终会调用到app_main.cpp中

的onZygoteInit,该方法如下,



```html

virtual void onZygoteInit()
    {
        sp<ProcessState> proc = ProcessState::self();
        ALOGV("App process: starting thread pool.\n");
        proc->startThreadPool();
	}
```



1,首先获取ProcessState对象。

2,调用startThreadPool方法启动进程内的binder线程池。

 

Native的守护进程也会在main方法中调用如下代码,

main_surfaceflinger.cpp中的main方法相关代码如下,

```html
ProcessState::self()->setThreadPoolMaxThreadCount(4);//设置线程池大小
    // start the thread pool
    sp<ProcessState> ps(ProcessState::self());
    ps->startThreadPool();
```



## 3.1,ProcessState解析



ProcessState.cpp的Self方法调用流程图如下,

![img](https://img-blog.csdn.net/20170612194721484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzOTQxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



Self方法如下,

```html
sp<ProcessState> ProcessState::self()
{
    Mutex::Autolock _l(gProcessMutex); //多线程同步
    if (gProcess != NULL) {
        return gProcess;
    }
    gProcess = new ProcessState;
    return gProcess;
}
```

单例对象,每个进程仅有一个。

ProcessState的构造方法如下,

```html

ProcessState::ProcessState()
    : mDriverFD(open_driver())
    , mVMStart(MAP_FAILED)
    , mThreadCountLock(PTHREAD_MUTEX_INITIALIZER)
    , mThreadCountDecrement(PTHREAD_COND_INITIALIZER)
    , mExecutingThreadsCount(0)
    , mMaxThreads(DEFAULT_MAX_BINDER_THREADS)
    , mManagesContexts(false)
    , mBinderContextCheckFunc(NULL)
    , mBinderContextUserData(NULL)
    , mThreadPoolStarted(false)
    , mThreadPoolSeq(1)
{
    if (mDriverFD >= 0) {
        // XXX Ideally, there should be a specific define for whether we
        // have mmap (or whether we could possibly have the kernel module
        // availabla).
#if !defined(HAVE_WIN32_IPC)
        // mmap the binder, providing a chunk of virtual address space to receive transactions.
        mVMStart = mmap(0, BINDER_VM_SIZE, PROT_READ, MAP_PRIVATE | MAP_NORESERVE, mDriverFD, 0);
        if (mVMStart == MAP_FAILED) {
            // *sigh*
            ALOGE("Using /dev/binder failed: unable to mmap transaction memory.\n");
            close(mDriverFD);
            mDriverFD = -1;
        }
#else
        mDriverFD = -1;
#endif
    }
 
    LOG_ALWAYS_FATAL_IF(mDriverFD < 0, "Binder driver could not be opened.  Terminating.");
}

```

### 3.1.1 打开binder驱动

首先调用open_driver方法打开/dev/binder驱动设备，再利用mmap()映射内核的地址空间，将Binder驱动的

fd赋值ProcessState对象中的变量mDriverFD，用于交互操作。

open_driver方法如下,

```html
static int open_driver()
{
    int fd = open("/dev/binder", O_RDWR);//打开binder驱动
    if (fd >= 0) {
        fcntl(fd, F_SETFD, FD_CLOEXEC);
        int vers = 0;
        status_t result = ioctl(fd, BINDER_VERSION, &vers);//检查版本号
        if (result == -1) {
            ALOGE("Binder ioctl to obtain version failed: %s", strerror(errno));
            close(fd);
            fd = -1;
        }
        if (result != 0 || vers != BINDER_CURRENT_PROTOCOL_VERSION) {
            ALOGE("Binder driver protocol does not match user space protocol!");
            close(fd);
            fd = -1;
        }
        size_t maxThreads = DEFAULT_MAX_BINDER_THREADS;
        result = ioctl(fd, BINDER_SET_MAX_THREADS, &maxThreads);//设置最大线程数,该数值为15
        if (result == -1) {
            ALOGE("Binder ioctl to set max threads failed: %s", strerror(errno));
        }
    } else {
        ALOGW("Opening '/dev/binder' failed: %s\n", strerror(errno));
    }
    return fd;
}
```

该方法逻辑代码挺简单,首先打开binder驱动,然后检查binder版本,最后设置最大的线程数。

### 3.1.2 mHandleToObject

ProcessState中另一个比较有重要的域是mHandleToObject：

```html
Vector<handle_entry>mHandleToObject;
```

handle_entry定义如下,

```html
struct handle_entry {
                IBinder* binder;
                RefBase::weakref_type* refs;
            };
```

它是本进程中记录所有BpBinder的向量表，非常重要,BpBinder是代理端的核心。

## 3.2,启动binder线程池

ProcessState.cpp的启动binder线程池流程图如下,

![img](https://img-blog.csdn.net/20170612194932632?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzOTQxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

startThreadPool方法如下,

```html
void ProcessState::startThreadPool()
{
    AutoMutex _l(mLock);
    if (!mThreadPoolStarted) {
        mThreadPoolStarted = true;
        spawnPooledThread(true);
    }
}
```

启动Binder线程池后, 则设置mThreadPoolStarted=true. 通过变量mThreadPoolStarted来保证每个应用进程

只允许启动一个binder线程池, 且本次创建的是binder主线程(isMain=true). 其余binder线程池中的线程都是由

Binder驱动来控制创建的。

spawnPooledThread方法如下,

```html
void ProcessState::spawnPooledThread(bool isMain)
{
    if (mThreadPoolStarted) {
        String8 name = makeBinderThreadName();
        ALOGV("Spawning new pooled thread, name=%s\n", name.string());
        sp<Thread> t = new PoolThread(isMain);
        t->run(name.string());
    }
}
```

从函数名看起来是创建线程池，其实就只是创建一个线程，该PoolThread继承Thread类。t->run()方法最终调用

内部类 PoolThread的threadLoop()方法。

如下,

```html
protected:
    virtual bool threadLoop()
    {
        IPCThreadState::self()->joinThreadPool(mIsMain);
        return false;
    }
    
    const bool mIsMain;
};
```

IPCThreadState.cpp的joinThreadPool方法如下,

```html

void IPCThreadState::joinThreadPool(bool isMain)
{
    LOG_THREADPOOL("**** THREAD %p (PID %d) IS JOINING THE THREAD POOL\n", (void*)pthread_self(), getpid());
 
    mOut.writeInt32(isMain ? BC_ENTER_LOOPER : BC_REGISTER_LOOPER); //创建Binder线程
    
    // This thread may have been spawned by a thread that was in the background
    // scheduling group, so first we will make sure it is in the foreground
    // one to avoid performing an initial transaction in the background.
    set_sched_policy(mMyThreadId, SP_FOREGROUND);
        
    status_t result;
    do {
        processPendingDerefs();
        // now get the next command to be processed, waiting if necessary
        result = getAndExecuteCommand();
 
        if (result < NO_ERROR && result != TIMED_OUT && result != -ECONNREFUSED && result != -EBADF) {
            ALOGE("getAndExecuteCommand(fd=%d) returned unexpected error %d, aborting",
                  mProcess->mDriverFD, result);
            abort();
        }
        
        // Let this thread exit the thread pool if it is no longer
        // needed and it is not the main process thread.
        if(result == TIMED_OUT && !isMain) {
            break;
        }
    } while (result != -ECONNREFUSED && result != -EBADF);
 
    LOG_THREADPOOL("**** THREAD %p (PID %d) IS LEAVING THE THREAD POOL err=%p\n",
        (void*)pthread_self(), getpid(), (void*)result);
    
    mOut.writeInt32(BC_EXIT_LOOPER);
    talkWithDriver(false); //false代表bwr数据的read_buffer为空
}
```

这样进程 有关binder跨进程调用的准备工作就做完了。

IPCThreadState是什么呢?有什么作用呢?

从名字上看，IPCThreadState是“和跨进程通信（IPC）相关的线程状态”。那么很显然，一个具有多个线程的进程

里应该会有多个IPCThreadState对象了，只不过每个线程只需一个IPCThreadState对象而已。所以要放在binder

线程池中统一管理。

  虽然binder驱动对应的句柄在进程的ProcessState中持有,但是具体和binder驱动交互的还是IPCThreadState类。

mHandleToObject本进程中记录所有BpBinder的向量表,在Binder架构中，应用进程是通过“binder句柄”来找到

对应的BpBinder的。

![img](https://img-blog.csdn.net/20170612195304789?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzOTQxNg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)