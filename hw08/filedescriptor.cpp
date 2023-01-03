#include "filedescriptor.h"
#include <unistd.h>

net::FileDescriptor::FileDescriptor() : fd_{} {
}

net::FileDescriptor::FileDescriptor(int fd) : fd_{fd} {
}

net::FileDescriptor::~FileDescriptor() {
  if (fd_.has_value()) {
	close(fd_.value());
  }

}
int net::FileDescriptor::unwrap() const {
  return fd_.value_or(-1);
}

net::FileDescriptor::FileDescriptor(net::FileDescriptor &&fd) noexcept: fd_{fd.fd_} {
//  Move constructor, meaning fd_ will be uninitialized.
  fd.fd_.reset();
}

net::FileDescriptor &net::FileDescriptor::operator=(net::FileDescriptor &&fd) noexcept {
	//  In assignment constructors, always check for self-assignment
  if (this == &fd) {
	return *this;
  }

	//	Close current file descriptor
  if (this->fd_.has_value()) {
	close(this->fd_.value());
  }
	//Move in new file descriptor
  this->fd_ = fd.fd_;

	//	Set the old objects file descriptor to invalid state.
  fd.fd_.reset();
  return *this;
}
