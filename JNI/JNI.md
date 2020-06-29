# JNI
----
### 01. JNI概念

JNI就是java native interface,用于java和c语言之间的互相调用,jni实际是一种规范,充当翻译的角色

### 02. 为啥要用JNI

1. 市场需求
2. java和c的相互调用
	1. java通过c来调用底层的硬件
	2. 复用c代码,且c已经发展了很多年了
	3. c效率过且不容易被反编译,重要的业务点都是c语言写的

### 03. 如何学习

1. 熟悉java,c,jni规范,ndk.