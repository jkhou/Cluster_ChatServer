# Cluster_ChatServer
**简介**: 工作在nginx tcp负载均衡环境中的集群聊天服务器和客户端, 网络模块基于muduo实现，使用基于发布-订阅的redis消息队列。

## 业务功能
```
    {"help", "显示所有支持的命令，格式 help"},
    {"chat", "一对一聊天，格式 chat:friendid:message"},
    {"addfriend", "添加好友，格式 addfriend:friendid"},
    {"delfriend", "删除好友，格式 delfriend:friendid"},
    {"creategroup", "创建群组，格式 creategroup:groupname:groupdesc"},
    {"addgroup", "加入群组，格式 addgroup:groupid"},
    {"delgroup", "删除群组，格式 delgroup:groupid"},
    {"groupchat", "群聊，格式 groupchat:groupid:message"},
    {"showfriendlist", "显示好友列表，格式 showfriendlist"},
    {"loginout", "注销，格式 loginout"}
```

## 表的设计
### user表
![git-command.jpg](https://img-blog.csdnimg.cn/20210127144426745.png)

### friend表
![git-command.jpg](https://img-blog.csdnimg.cn/20210127144758137.png)

### offlinemessage表
![git-command.jpg](https://img-blog.csdnimg.cn/20210127145215562.png)

### allgroup表
![git-command.jpg](https://img-blog.csdnimg.cn/20210127145609263.png)

### groupuser表
![git-command.jpg](https://img-blog.csdnimg.cn/20210127150030734.png)

## JSON通信格式
```
1.登录
json["msgid"] = LOGIN_MSG;
json["id"]			//用户id
json["password"]	//密码

2.登录反馈
json["msgid"] = LOGIN_MSG_ACK;
json["id"]			//登录用户id
json["name"]		//登录用户密码
json["offlinemsg"]	//离线消息
json["friends"]		//好友信息,里面有id、name、state三个字段
json["groups"]		//群组信息,里面有id，groupname，groupdesc，users三个字段
					//users里面则有id，name，state，role四个字段
json["errno"]		//错误字段，错误时被设置成1，用户不在线设置成2
json["errmsg"]		//错误信息

3.注册
json["msgid"] = REG_MSG;
json["name"]		//用户姓名
json["password"]	//用户姓名

4.注册反馈
json["msgid"] = REG_MSG_ACK;
json["id"]			//给用户返回他的id号
json["errno"]		//错误信息，失败会被设置为1

5.加好友
json["msgid"] = ADD_FRIEND_MSG;
json["id"]			//当前用户id
json["friendid"]	//要加的好友的id

6.一对一聊天
json["msgid"] = ONE_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["to"]			//接受者id
json["msg"]			//消息内容
json["time"]		//发送时间

7.创建群
json["msgid"] = CREATE_GROUP_MSG;
json["id"]			//群创建者id
json["groupname"]	//群名
json["groupdesc"]	//群描述

8.加入群
json["msgid"] = ADD_GROUP_MSG;
json["id"]			//用户id
json["groupid"]		//群id

9.群聊
json["msgid"] = GROUP_CHAT_MSG;
json["id"]			//发送者id
json["name"]		//发送者姓名
json["groupid"]		//发送者姓名
json["msg"]			//消息内容
json["time"]		//发送时间

10.注销
json["msgid"] = LOGINOUT_MSG;
json["id"]			//要注销的id

11.删除群组
json["msgid"] = DEL_GROUP_MSG;
json["id"]			//发送者id
json["groupid"]

12.删除好友
json["msgid"] = DEL_FRIEND_MSG;
json["id"]			//发送者id
json["friendid"]	//要删除的好友的id

13.显示当前好友列表
json["msgid"] = SHOW_FRIEND_LIST_MSG;
json["id"]			//发送者id
```
## 网络模块
这是muduo网络库采用的reactor模型，有点像Nginx的负载均衡，但是也有差别，Nginx采用的是多进程，而muduo是多线程。
在muduo设计中，有一个main reactor负责接收来自客户端的连接。然后使用轮询的方式给sub reactor去分配连接，而客户端的读写事件都在这个sub reactor上进行。咋样，像不像Nginx的io进程+工作进程的组合。

![git-command.jpg](https://www.cyhone.com/img/reactor/single_thread_reactor.png)

muduo提供了两个非常重要的注册回调接口：**连接回调和消息回调**
```
//注册连接回调
server_.setConnectionCallback(bind(&ChatServer::on_connection, this, _1));

//注册消息回调
server_.setMessageCallback(bind(&ChatServer::on_message, this, _1, _2, _3));
```

设置一个处理有关连接事件的方法和处理读写事件的方法
```
//上报连接相关信息的回调函数
void on_connection(const TcpConnectionPtr &);

//上报读写时间相关信息的回调函数
void on_message(const TcpConnectionPtr &, Buffer *, Timestamp);
```
- 当用户进行连接或者断开连接时便会调用**on_connection**方法进行处理，其执行对象应该是**main reactor**;
- 发生读写事件时，则会调用**on_message**方法，执行对象为**sub reactor**.






