1. 四大启动模式还是值得挖掘啊
2. Error running mainapp: Instant Run requires 'Tools | Android | Enable ADB integration' to be enabled.
3. TypeToken,追踪运行的时候的泛型,所有的泛型在运行时候都是object,知道运行的时候泛型有利于序列化和反序列化.
4. arrays,对基本类型数据排序的时候本省就是comparTo方法,而string是按照object的自然排序,注意String的排序,compareToIgnoreCase.
5. Gson的先关操作,变成json字符串
6. 不可逾越时间的设置,9点都是必须设置成可以进行预约的吗????
7. 文件流的简单操作.
8. 对象.for即可进行for或者增强for等快捷键.settings–>Editor–>Live Templates查看快捷键的操作.
9. 列表的局部刷新,
10. 2/8/2018 4:18:57 PM    清楚权限问题.配置debugkeystore(调试的时候一般使用的默认的android自带的keystore).友盟统计的时候除了分享需要使用第三方的jar包,其他的东西可以不要(实在自动集成基础组建的时候) .
11. 待办事项:页面的统计(因为界面比较多,所以并没有做这个复杂的工作) 
12. 学习网络编程,网易云课堂.
13. TypeToken运行过程中的泛型跟踪
14. 注解的学习.
15. RecyclerView中item出现的数量协调问题不是太懂,
16. 输入框的问题:https://blog.csdn.net/smileiam/article/details/69055963
17. 添加头部出现的问题:1,加载更多出现加载重复,重复头部
18. fragment在viewPager中的生命周期问题.https://blog.csdn.net/linfeng24/article/details/26491407
19. progressbar颜色设置等.https://blog.csdn.net/qq_21445563/article/details/71159540
20. baseAdapter:装饰者模式设计的给RecyclerView增加头部和尾部
21. SoftboradBlockEdittext 键盘弹起的设计方式.https://blog.csdn.net/smileiam/article/details/69055963
22. 输入框加上背景就会被遮挡,why ?
23. ImageView的scType只是在src的模式下起作用,backGround是不起作用的.字面意思即可理解.

	>代码中设置src

	>setImageDrawable(Drawable drawable);
	
	>setImageBitmap(Bitmap bm);
	
	>setImageResource(int resId);

	>代码中设置背景.

	>setBackgroundReource(int resid)

	>setBackground(Drawable background)
	
	>setBackgroundColor(int color)

	>setBackgroundDrawable(Drawable background) //This method was deprecated in API level 16. use setBackground(Drawable) instead

24. 自定义view介绍:https://www.cnblogs.com/jiayongji/p/5560806.html
25. 自定义控件注意其命名空间的问题,一般都是resauto,若是其他极有可能是错误的.
26. 低版本编译的包是能在高版本系统上运行的,比如6.0之下的apk包没有做权限控制,但是你照样是可以在6.0之上运行的,系统向下兼容.buildTools有版本的区别,而targetSdk可能就是方法的不同了.
27. weight是与父布局有关系的.
28. 2018-8-6:android里边的observeable,observer和jdk内置的有什么区别呢?
29. 2018-8-14:https://blog.csdn.net/javazejian/article/details/52665164,序列化的操作.
30. https://blog.csdn.net/lovingning/article/details/79990856
31. 2018.8.15:activity之间传递bitmap对象和map集合.https://blog.csdn.net/xueerfei008/article/details/23046341.activity之间图片的限制是40k,最好是使用图片的保存之后传递地址.
32. string.format();保留两位小数中间不能使用long类型的数据,因为是其整型?
33. 8-16:集合是不能够进行强转的,需要进行泛型的擦除,https://blog.csdn.net/lovingning/article/details/79990856
34. toast是能在所有的界面上显示的,注意context的问题,dialog是需要依附于activity上显示的,可以使用task管理获取最上层的activity,或者在baseactivity中写入方法并且在需要的地方进行发消息.
35. 阿里推送,直接打开app并且打开相应的界面,也可以配置自己手动的操作打开app等操作.
36. 推送:在我们杀死app,并且杀死它里边的service的时候,那么这个时候我们客户端就与服务端断开了长连接是收不到消息的.
37. 推送:homeactivity的context不为null,为啥后边还要重新的开启homeactivity呢?有点问题吧.
38. setUserVisibleHint详细解释.https://blog.csdn.net/czhpxl007/article/details/51277319
39. 8.20:scrollView的不自觉滑动,在其父布局中添加;android:focusableInTouchMode="true"即可.
40. 8.23:listview的item点击失效问题解决.https://blog.csdn.net/qq_27853161/article/details/53139369,去掉点击效果:在ListView布局中加上android:listSelector="@android:color/transparent" 这句代码
41. 传递ArrayList数据,放在bundle中能写泛型而在intent中难以写泛型.???
42. 8.30:stylist的升级逻辑.
43. 9.13:包含有虚拟按键的屏幕高度.
44. 9.18:app被后台杀死之后,但是此时停留在某一个界面,之后再点击此界面所走的逻辑:被杀死之后,但是此界面的成员变量等是存在的,但是你若调用了系统的登录信息什么的,那么此时就被置为了null,再回来操作什么的,就会出错的.解决办法:在baseactivity或者是application(后台杀死程序重新创建也会先走application的)中判断是否非正常启动的,若是非正常启动则重新调用登录方法再去重新的赋值数据.