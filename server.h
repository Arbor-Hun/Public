//
// Created by Sixzeroo on 2018/6/6.
//

#ifndef CHARROOM_SERVER_H
#define CHARROOM_SERVER_H

#include <unordered_map>
#include <memory>
#include "socket_epoll.h"
#include "threadpool.h"

#define WELCOM_MES "Welcome to ChatRoom!"
#define ONLY_ONE_CAUTION "There is only one people in the chatroom!"

class ServerEpollWatcher : public SocketEpollWatcher {
public:
    virtual int on_accept(EpollContext &epoll_context);

    virtual int on_readable(EpollContext &epoll_context, std::unordered_map<int, std::string> &client_list);
};

class ChatRoomServer {
private:
    ServerEpollWatcher _server_handler; //多态，在Server构造时绑定地址
    SocketEpoll _socket_epoll; //关于Socket的所有操作

public:
    ChatRoomServer();

    int start_server(const std::string bind_ip, int port, int backlog, int max_events);

    int stop_server();
};

#endif //CHARROOM_SERVER_H
