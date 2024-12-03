# 一.pms介绍

pms就是用来管理所有的package信息的,包括安装,卸载,更新以及解析AndroidManifest.xml(四大组件)以组织相应的数据结构.他就是为ams做准备的,具体怎么做操作是由ams来完成的.主要功能就是在开机时候的操作,占据了开机的70%时间.将apk的信息缓存起来就是pms的主要任务.

用户点击app图标就开始进入到ams中,ams是管理activity的生命周期,启动activity,与pms进行交互.



