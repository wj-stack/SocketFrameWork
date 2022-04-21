//
// Created by wyatt on 2022/4/21.
//

#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop *loop_, int fd_,int id_) : loop(loop_),fd(fd_),channel(loop, fd),id(id_) {

}
