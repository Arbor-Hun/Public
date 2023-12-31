#ifndef CHARROOM_SOCKET_EPOLL_H
#define CHARROOM_SOCKET_EPOLL_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sys/epoll.h>
#include "threadpool.h"


enum SocketEpollStatus {
    S_RUN = 0,
    S_REJECT_CONN = 1,
    S_STOP = 2
};

class EpollContext {
public:
    void *ptr;
    int fd;
    std::string client_ip;
    int client_port;
};

class SocketEpollWatcher {
public:

    virtual int on_accept(EpollContext &epoll_context) = 0;

    virtual int on_readable(EpollContext &epoll_context) = 0;
};

class SocketEpoll {
private:
    int bind_on(unsigned int ip, int port);

    int listen_on();

    int create_epoll();

    int handle_event(epoll_event &e);

    int handle_accept_event(const int &epollfd, epoll_event &event, SocketEpollWatcher *socket_watcher);
    int handle_readable_event(epoll_event &event, SocketEpollWatcher *socket_watcher);

    int _epollfd;
    int _max_events;
    std::string _bind_ip;
    int _backlog; // listen的参数
    int _port;
    int _listen_socket;
    int _status; //见SocketEpollStatus，3种运行状态
    int _clients;
    SocketEpollWatcher *_watcher; // 待绑定地址的纯虚对象，作为业务接口
    std::unique_ptr<ThreadPool> _threadpool;  // 线程池

public:
    SocketEpoll();

    ~SocketEpoll();

    void set_bind_ip(const std::string &_bind_ip);

    void set_backlog(int _backlog);

    void set_port(int _port);

    void set_max_events(int _max_events);

    int set_nonblocking(int fd);
    // 绑定watcher接口地址
    void set_watcher(SocketEpollWatcher *_watcher);

    int add_listen_sock_to_epoll();

    int start_epoll();

    int start_epoll_loop();

    int stop_epoll();
};

#endif //CHARROOM_SOCKET_EPOLL_H
