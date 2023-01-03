#include "socket.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "filedescriptor.h"
#include <errno.h>
#include <string.h>


/// Return true, if the given file descriptor is listening.
/// Use getsockopt(3), retrieve options for the socket level, and the option is `SO_ACCEPTCONN`. If
/// you are unsure, read the man pages :-)
bool net::is_listening(int fd) {

  if(fd < 0){
	return false;
  }

  int option_value = 1;
  socklen_t option_length = sizeof(option_value);
  int opt_val = getsockopt(fd, SOL_SOCKET, SO_ACCEPTCONN, &option_value, &option_length);

  if (!opt_val){
	return true;
  }
  printf("Oh dear, something went wrong with read()! errno: %d %s\n",errno,strerror(errno));
  return false;

}

/// A Linux Socket. Sockets are communication end points, in our case there are TCP endpoints. In
/// Linux as pretty much everything, they are represented by a file descriptor.
///
/// See also socket(7)


/// Initialize the TCP socket for IPv4. The type should be `SOCK_STREAM`. Check out socket(2)
/// and ip(7).
net::Socket::Socket() {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0 );

  fd_ = FileDescriptor(sockfd);

}


/// Bind and then listen on the given port. Listen on any incoming address. Be sure to use the
/// correct endianness for the port.
/// For server.
/// Check out bind(3), ip(7) and listen(2), htons(3)

void net::Socket::listen(uint16_t port) const {

  // Set up the sockaddr_in structure
  struct sockaddr_in addr;
//  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(port);  // Set the port, converting to network byte order

  // Bind the socket to the local address and port
  if (fd_.unwrap() < 0 ){
	return;
  }

  int bind_err = bind(fd_.unwrap(),  reinterpret_cast<struct sockaddr*>(&addr), sizeof(addr)) ;

  if (bind_err < 0 ){
	return;
  }

  // Listen

  int listen_err = ::listen(fd_.unwrap(), 10);

  if (listen_err < 0 ){
	return;
  }

}


/// Wait for a connection to appear, and then return the newly created connection. Check that
/// the socket is already listening, throw an instance of `std::runtime_error` if the socket is
/// not listening. The returned connection shall take ownership to close the new socket file
/// descriptor.
///
/// Check out accept(3)
net::Connection net::Socket::accept() const {

  if(!net::is_listening(fd_.unwrap())){
	throw std::runtime_error("Error in  net::Socket::accept(), not listening for connections yet");
  }

  // Set up the sockaddr_in structure
  struct sockaddr_in addr{};
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;

  socklen_t addrlen = sizeof(addr);

  int socketfd = ::accept(fd_.unwrap(),  reinterpret_cast<struct sockaddr*>(&addr) , &addrlen);


  return net::Connection{net::FileDescriptor{socketfd}};
}

/// Connect to the destination on the given port (be sure of endianness!). `destination` can
/// either be an IPv4 address of the form "8.8.8.8" or "127.0.0.1", but also accept "localhost"
/// and convert that to the correct IPv4 address.
///
/// The connection is one shot, i.e. after the connection is closed, the socket should be closed
/// as well. The responsibility is transferred to the Connection.
///
/// Check out: inet_addr(3), connect(3), gethostbyname(3), htons(3)
net::Connection net::Socket::connect(std::string destination, uint16_t port) {

  // Set up the sockaddr_in structure
  std::string destination_mod;

  if ( destination == "localhost") {
	destination_mod = "127.0.0.1";
  }
  else{
	destination_mod = destination;
  }

  struct sockaddr_in addr{};
  memset(&addr, 0, sizeof(addr));
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, destination_mod.c_str(), &addr.sin_addr);

  int connectfd = ::connect(fd_.unwrap(),  reinterpret_cast<struct sockaddr*>(&addr) , sizeof(addr));

  return net::Connection{std::move(fd_)};
}

/// Connect to localhost on the given port, see the other overload
net::Connection net::Socket::connect(uint16_t port) {

  struct sockaddr_in addr{};
  memset(&addr, 0, sizeof(addr));
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

  int connectfd = ::connect(fd_.unwrap(),  reinterpret_cast<struct sockaddr*>(&addr) , sizeof(addr));

  return net::Connection{std::move(fd_)};
}

/// Return the int to the file descriptor
int net::Socket::fd() const {
  return fd_.unwrap();
}

