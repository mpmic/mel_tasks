#include <vector>
#include "connection.h"


/// Light wrapper around the libc send(3)-function. Should return the value returned by send(3), and
/// the options should be set to 0
ssize_t net::send(int fd, std::span<const char> data){
  ssize_t send_code = ::send(fd, data.data(), data.size(), 0);
  return send_code;
}

/// Light wrapper around libc recv(3)-function. Should return the value returned by recv(3), and the
/// options should be set to 0;
ssize_t net::receive(int fd, std::span<char> buf){
  ssize_t num_bytes = ::recv(fd, buf.data(), buf.size(), 0);
  return num_bytes;
}

/// One endpoint of a TCP connection. It can send and receive data from and to the other end of the
/// endpoint.
///
/// HINT: If the compiler complains that it can't find the correct function. Try prefixing it with
/// `::` to call the global version, not the class variant.

/// Take ownership of a file descriptor
net::Connection::Connection(net::FileDescriptor &&fd){
  fd_ = std::move(fd);
}

/// Send data from a string_view to the given file descriptor.
///
/// Check out: send(3)
void net::Connection::send(std::string_view data) const {

  net::send(fd_.unwrap(), data);

}

/// Send data from an `std::istream` to the given file descriptor
/// Check out: send(3)
void net::Connection::send(std::istream &data) const {

  std::vector<char> buf(std::istreambuf_iterator<char>(data), {});

  ::send(fd_.unwrap(), buf.data(), buf.size(), 0);

}

/// Receive data from the underlying socket, and write it to the `std::ostream`. Importantly,
/// just read a chunk of data, write it to the stream and finish the function. Use 128
/// bytes/chars for the buffer size. If you want to ensure that you read all data from the
/// socket, use `receive_all`.
///
/// Return the size read from the socket.
///
/// Check out: recv(3)
ssize_t net::Connection::receive(std::ostream &stream) const {
  constexpr std::size_t kChunkSize = 128;
  char buf[kChunkSize];
  // Receive data
  ssize_t num_bytes = recv(fd_.unwrap(), buf, kChunkSize, 0);

  if(num_bytes>0){
	stream.write(buf, num_bytes);
  }

  return num_bytes;
}

/// Receive all data from the socket. This continuously reads a chunk from the socket and
/// concatenates it to the stream. It reads until no data is more available (hint hint, see the
/// man pages).
///
/// Return the size read from the socket.
///
/// Check out: recv(3)
ssize_t net::Connection::receive_all(std::ostream &stream) const {

  // Receive data in chunks and write it to the stream
  constexpr std::size_t kChunkSize = 4096;
  char buf[kChunkSize];
  while (true) {
	// Receive data
	ssize_t num_bytes = recv(fd_.unwrap(), buf, kChunkSize, 0);
	if (num_bytes <= 0) {
	  // Connection closed or error
	  return 0;
	}

	// Write the data to the stream
	stream.write(buf, num_bytes);
  }
}

/// Return the underlying file descriptor
int net::Connection::fd() const {
  return fd_.unwrap();
}