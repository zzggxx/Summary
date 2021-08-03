> ### Android Init Language
>
> Android Init Language中由5类语法组成，分别是Actions, Commands, Services, Options, and Imports 
>
>
>  每一行是一个语句，单词之间用空格分开，如果单词中有空格可以用反斜杠转义，也可以用双引号来引用文本避免和空格冲突，如果一行语句太长可以用 \ 换行，用 # 表示注释
>
>
>  Actions和Services可以作为一个独立的Section,所有的Commands和Options从属于紧挨着的Actions或Services，定义在第一个Section前的Commands和Options将被忽略掉 
>
>
>  Actions和Services都是唯一的，如果定义了两个一样的Action，第二个Action的Command将追加到第一个Action，
>  如果定义了两个一样的Service，第二个Service将被忽略掉并打印错误日志
>
> ### Init .rc Files
>
> Android Init Language是用后缀为.rc的纯文本编写的,而且是由多个分布在不同目录下的.rc文件组成,如下所述 
>
> 
>  /init.rc 是最主要的一个.rc文件，它由init进程在初始化时加载，主要负责系统初始化,它会导入 /init.${ro.hardware}.rc ，这个是系统级核心厂商提供的主要.rc文件
>
> 
>  当执行 mount_all 语句时，init进程将加载所有在 /{system,vendor,odm}/etc/init/ 目录下的文件，挂载好文件系统后，这些目录将会为Actions和Services服务
>
> 
>  有一个特殊的目录可能被用来替换上面的三个默认目录，这主要是为了支持工厂模式和其他非标准的启动模式,上面三个目录用于正常的启动过程
>
> 
>  这三个用于扩展的目录是
>
> 1. /system/etc/init/ 用于系统本身，比如SurfaceFlinger, MediaService, and logcatd.
>
> 2. /vendor/etc/init/ 用于SoC(系统级核心厂商，如高通),为他们提供一些核心功能和服务
>
> 3. /odm/etc/init/ 用于设备制造商（odm定制厂商，如华为、小米），为他们的传感器或外围设备提供一些核心功能和服务
>
>    
>
> 所有放在这三个目录下的Services二进制文件都必须有一个对应的.rc文件放在该目录下，并且要在.rc文件中定义service结构,
>  有一个宏LOCAL_INIT_RC,可以帮助开发者处理这个问题. 每个.rc文件还应当包含一些与该服务相关的actions
>
> 
>  举个例子，在system/core/logcat目录下有logcatd.rc和Android.mk这两个文件. Android.mk文件中用LOCAL_INIT_RC这个宏，在编译时将logcatd.rc放在/system/etc/init/目录下,init进程在调用 mount_all 时将其加载，在合适的时机运行其定义的service并将action放入队列
>
> 
>  将init.rc根据不同服务分拆到不同目录，要比之前放在单个init.rc文件好. 这种方案确保init读取的service和action信息能和同目录下的Services二进制文件更加符合,不再像以前单个init.rc那样.
>  另外，这样还可以解决多个services加入到系统时发生的冲突，因为他们都拆分到了不同的文件中
>
> 
>  在 mount_all 语句中有 "early" 和 "late" 两个可选项，当 early 设置的时候，init进程将跳过被 latemount 标记的挂载操作，并触发fs encryption state 事件，
>  当 late 被设置的时候，init进程只会执行 latemount 标记的挂载操作，但是会跳过导入的 .rc文件的执行. 默认情况下，不设置任何选项，init进程将执行所有挂载操作
>
> ### Actions
>
> Actions由一行行命令组成. trigger用来决定什么时候触发这些命令,当一个事件满足trigger的触发条件时，
>  这个action就会被加入到处理队列中（除非队列中已经存在）
>
> 
>  队列中的action按顺序取出执行，action中的命令按顺序执行. 这些命令主要用来执行一些操作（设备创建/销毁，属性设置，进程重启）
>
> 
>  Actions的格式如下：



```xml
    on <trigger> [&& <trigger>]*
       <command>
       <command>
       <command>
```

> ### Services
>
> Services是init进程启动的程序,它们也可能在退出时自动重启. Services的格式如下：



```c
    service <name> <pathname> [ <argument> ]*
       <option>
       <option>
       ...
```

> ### Options
>
> Options是Services的参数配置. 它们影响Service如何运行及运行时机
>
> 
>  console [<console>]
>
>  Service需要控制台. 第二个参数console的意思是可以设置你想要的控制台类型，默认控制台是/dev/console ,/dev 这个前缀通常是被忽略的，比如你要设置控制台 /dev/tty0 ,那么只需要设置为console tty0
>
> 
>  critical
>
>  表示Service是严格模式. 如果这个Service在4分钟内退出超过4次，那么设备将重启进入recovery模式
>
> 
>  disabled
>
>  表示Service不能以class的形式启动，只能以name的形式启动
>
> 
>  setenv <name> <value>
>
>  在Service启动时设置name-value的环境变量
>
> 
>  socket <name> <type> <perm> [ <user> [ <group> [ <seclabel> ] ] ]
>
>  创建一个unix域的socket,名字叫/dev/socket/name , 并将fd返回给Service. type 只能是 "dgram", "stream" or "seqpacket".
>  User 和 group 默认值是 0. 'seclabel' 是这个socket的SELinux安全上下文,它的默认值是service安全策略或者基于其可执行文件的安全上下文.
>  它对应的本地实现在libcutils的android_get_control_socket
>
> 
>  file <path> <type>
>
>  打开一个文件，并将fd返回给这个Service. type 只能是 "r", "w" or "rw". 它对应的本地实现在libcutils的android_get_control_file 
>
> 
>  user <username>
>
>  在启动Service前将user改为username,默认启动时user为root(或许默认是无).
>  在Android M版本，如果一个进程想拥有Linux capabilities（相当于Android中的权限吧），也只能通过设置这个值. 以前，一个程序要想有Linux capabilities，必须先以root身份运行，然后再降级到所需的uid.
>  现在已经有一套新的机制取而代之，它通过fs_config允许厂商赋予特殊二进制文件Linux capabilities. 这套机制的说明文档在http://source.android.com/devices/tech/config/filesystem.html.
>  当使用这套新的机制时，程序可以通过user参数选择自己所需的uid,而不需要以root权限运行. 在Android O版本，
>  程序可以通过capabilities参数直接申请所需的能力，参见下面的capabilities说明
>
> 
>  group <groupname> [ <groupname>\* ]
>
>  在启动Service前将group改为第一个groupname,第一个groupname是必须有的，
>  默认值为root（或许默认值是无），第二个groupname可以不设置，用于追加组（通过setgroups）.
>
> 
>  capabilities <capability> [ <capability>\* ]
>
>  在启动Service时将capabilities设置为capability. 'capability' 不能是"CAP_" prefix, like "NET_ADMIN" or "SETPCAP". 参考
>  http://man7.org/linux/man-pages/man7/capabilities.7.html ，里面有capability的说明.
>
> 
>  seclabel <seclabel>
>
>  在启动Service前将seclabel设置为seclabel. 主要用于在rootfs上启动的service，比如ueventd, adbd.
>  在系统分区上运行的service有自己的SELinux安全策略，如果不设置，默认使用init的安全策略.
>
> 
>  oneshot
>
>  退出后不再重启
>
> 
>  class <name> [ <name>\* ]
>
>  为Service指定class名字. 同一个class名字的Service会被一起启动或退出,默认值是"default",第二个name可以不设置，用于service组.
>
> 
>  animation class
>
>  animation class 主要包含为开机动画或关机动画服务的service. 它们很早被启动，而且直到关机最后一步才退出.
>  它们不允许访问/data 目录，它们可以检查/data目录，但是不能打开 /data 目录，而且需要在 /data 不能用时也正常工作 .
>
> 
>  onrestart 
>
>  在Service重启时执行命令.
>
> 
>  writepid <file> [ <file>\* ]
>
>  当Service调用fork时将子进程的pid写入到指定文件. 用于cgroup/cpuset的使用，当/dev/cpuset/下面没有文件但ro.cpuset.default的值却不为空时,
>  将pid的值写入到/dev/cpuset/cpuset_name/tasks文件中
>
> 
>  priority <priority>
>
>  设置进程优先级. 在-20～19之间，默认值是0,能过setpriority实现
>
> 
>  namespace <pid|mnt>
>
>  当fork这个service时，设置pid或mnt标记
>
> 
>  oom_score_adjust <value>
>
>  设置子进程的 /proc/self/oom_score_adj 的值为 value,在 -1000 ～ 1000之间.
>
> 
>
> ### Triggers
>
> Triggers 是个字符串，当一些事件发生满足该条件时，一些actions就会被执行
>
> 
>  Triggers分为事件Trigger和属性Trigger
>
> 
>  事件Trigger由trigger 命令或QueueEventTrigger方法触发.它的格式是个简单的字符串，比如'boot' 或 'late-init'.
>
> 
>  属性Trigger是在属性被设置或发生改变时触发. 格式是'property:<name>=<value>'或'property:<name>=*',它会在init初始化设置属性的时候触发.
>
> 
>  属性Trigger定义的Action可能有多种触发方式，但是事件Trigger定义的Action可能只有一种触发方式
>
> 
>  比如：
>
>  on boot && property:a=b 定义了action的触发条件是，boot Trigger触发，并且属性a的值等于b
>
> 
>  on property:a=b && property:c=d 这个定义有三种触发方式:
>
> 1. 在初始化时，属性a=b,属性c=d.
> 2. 在属性c=d的情况下，属性a被改为b.
> 3. A在属性a=b的情况下，属性c被改为d.
>
> ### Commands
>
> ```
> bootchart [start|stop]
> ```
>
>  启动或终止bootcharting. 这个出现在init.rc文件中，但是只有在/data/bootchart/enabled文件存在的时候才有效，否则不能工作
>
> 
>  chmod <octal-mode> <path>
>
>  修改文件读写权限
>
> 
>  chown <owner> <group> <path>
>
>  修改文件所有者或所属用户组
>
> 
>  class_start <serviceclass>
>
>  启动所有以serviceclass命名的未启动的service(service有一个name，也有个class，
>  这里的serviceclass就是class,class_start和后面的start是两种启动方式，class_start是class形式启动，start是name形式启动)
>
> 
>  class_stop <serviceclass> 
>
>  终止所有以serviceclass命名的正在运行的service
>
> 
>  class_reset <serviceclass>
>
>  终止所有以serviceclass命名的正在运行的service,但是不禁用它们. 它们可以稍后被class_start重启
>
> 
>  class_restart <serviceclass>
>
>  重启所有以serviceclass命名的service
>
> 
>  copy <src> <dst>
>
>  复制一个文件，与write相似，比较适合二进制或比较大的文件.
>
>  对于src,从链接文件、world-writable或group-writable复制是不允许的.
>
>  对于dst，如果目标文件不存在，则默认权限是0600,如果存在就覆盖掉
>
> 
>  domainname <name>
>
>  设置域名
>
> 
>  enable <servicename>
>
>  将一个禁用的service设置为可用.
>  如果这个service在运行，那么就会重启.
>  一般用在bootloader时设置属性，然后启动一个service，比如
>  on property:ro.boot.myfancyhardware=1
>  enable my_fancy_service_for_my_fancy_hardware
>  exec [ <seclabel> [ <user> [ <group>\* ] ] ] -- <command> [ <argument>\* ]
>  新建子进程并运行一个带指定参数的命令. 这个命令指定了seclabel（安全策略），user(所有者)，group(用户组).
>  直到这个命令运行完才可以运行其他命令，seclabel可以设置为 - 表示用默认值，argument表示属性值.
>  直到子进程新建完毕，init进程才继续执行.
>
> 
>  exec_start <service>
>
>  启动一个service，只有当执行结果返回，init进程才能继续执行. 这个跟exec相似，只是将一堆参数的设置改在在service中定义
>
> 
>  export <name> <value>
>
>  设置环境变量name-value. 这个环境变量将被所有已经启动的service继承
>
> 
>  hostname <name> 
>
>  设置主机名
>
> 
>  ifup <interface>
>
>  开启指定的网络接口
>
> 
>  insmod [-f] <path> [<options>]
>
>  安装path下的模块，指定参数options.
>
>  -f 表示强制安装，即便是当前Linux内核版本与之不匹配
>
> 
>  load_all_props
>
>  加载/system, /vendor等目录下的属性，这个用在init.rc中
>
> 
>  load_persist_props
>
>  加载/data 下的持久化属性. 这个用在init.rc中
>
> 
>  loglevel <level>
>
>  设置日志输出等级，level表示等级
>
> 
>  mkdir <path> [mode] [owner] [group]
>
>  创建一个目录，path是路径，mode是读写权限，默认值是755,owner是所有者，默认值root,group是用户组,默认值是root.
>  如果该目录已存在，则覆盖他们的mode,owner等设置
>
> 
>  mount_all <fstab> [ <path> ]\* [--<option>]
>
>  当手动触发 "early" 和 "late"时，调用fs_mgr_mount_all 函数，指定fstab配置文件，并导入指定目录下的.rc文件
>  详情可以查看init.rc文件中的有关定义
>
> 
>  mount <type> <device> <dir> [ <flag>\* ] [<options>]
>
>  在dir目录下挂载一个名叫device的设备
>
>  _flag 包括 "ro", "rw", "remount", "noatime", ...
>
>  options 包括 "barrier=1", "noauto_da_alloc", "discard", ... 用逗号分开，比如 barrier=1,noauto_da_alloc
>
> 
>  restart <service>
>
>  终止后重启一个service,如果这个service刚被重启就什么都不做，如果没有在运行，就启动
>
> 
>  restorecon <path> [ <path>\* ]
>
>  恢复指定目录下文件的安全上下文.第二个path是安全策略文件. 指定目录不需要必须存在，因为它只需要在init中正确标记
>
> 
>  restorecon_recursive <path> [ <path>\* ]
>
>  递归地恢复指定目录下的安全上下文，第二个path是安全策略文件位置
>
> 
>  rm <path>
>
>  调用 unlink(2)删除指定文件. 最好用exec -- rm ...代替，因为这样可以确保系统分区已经挂载好
>
> 
>  rmdir <path>
>
>  调用 rmdir(2) 删除指定目录
>
> 
>  setprop <name> <value>
>
>  设置属性name-value 
>
> 
>  setrlimit <resource> <cur> <max>
>
>  指定一个进程的资源限制
>
> 
>  start <service> 
>
>  启动一个未运行的service
>
> 
>  stop <service>
>
>  终止一个正在运行的service
>
> 
>  swapon_all <fstab>
>
>  调用 fs_mgr_swapon_all，指定fstab配置文件.
>
> 
>  symlink <target> <path>
>
>  在path下创建一个指向target的链接
>
> 
>  sysclktz <mins_west_of_gmt>
>
>  重置系统基准时间(如果是格林尼治标准时间则设置为0)
>
> 
>  trigger <event>
>
>  触发事件event，由一个action触发到另一个action队列
>
> 
>  umount <path>
>
>  卸载指定path的文件系统
>
> 
>  verity_load_state
>
>  内部实现是加载dm-verity的状态
>
> 
>  verity_update_state <mount-point>
>
>  内部实现是设置dm-verity的状态，并且设置partition.mount-point.verified的属性. 用于adb重新挂载，
>  因为fs_mgr 不能直接设置它。 
>
> 
>  wait <path> [ <timeout> ]
>
>  查看指定路径是否存在. 如果发现则返回,可以设置超时时间，默认值是5秒
>
> 
>  wait_for_prop <name> <value>
>
>  等待name属性的值被设置为value，如果name的值一旦被设置为value，马上继续
>
> 
>  write <path> <content>
>
>  打开path下的文件，并用write(2)写入content内容. 如果文件不存在就会被创建，如果存在就会被覆盖掉
>
> 
>
> ### Imports
>
> import关键字不是一个命令，但是如果有.rc文件包含它就会马上解析它里面的section,用法如下：
>
> 
>  import <path>
>
>  解析path下的.rc文件 ，括展当前文件的配置。如果path是个目录，这个目录下所有.rc文件都被解析，但是不会递归,
>  import被用于以下两个地方：
>
>  1.在初始化时解析init.rc文件
>
>  2.在mount_all时解析{system,vendor,odm}/etc/init/等目录下的.rc文件
>
>
>  后面的内容主要是一些跟调试init进程相关的东西，比如init.svc.<name>可以查看service启动的状态，
>  ro.boottime.init记录一些关键的时间点，Bootcharting是一个图表化的性能监测工具等

