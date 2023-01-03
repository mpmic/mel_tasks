#pragma once
#include "socket.h"
namespace net {

/**
 * TCP socket server. Listens for your request to deliver you juicy data!
 */
class Server {

 public:

  Server(uint16_t port);

  [[nodiscard]] Connection accept() const;

 private:

  Socket socket_;
};

} // namespace net
