#include "client.h"

net::Connection net::Client::connect(std::string destination, uint16_t port) {
  return socket_.connect(destination, port);
}

net::Connection net::Client::connect(uint16_t port) {
  return socket_.connect(port);
}
