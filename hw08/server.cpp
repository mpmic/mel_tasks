#include "server.h"

net::Server::Server(uint16_t port) {

  socket_.listen(port);
}

net::Connection net::Server::accept() const {
  return socket_.accept();
}
