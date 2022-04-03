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

