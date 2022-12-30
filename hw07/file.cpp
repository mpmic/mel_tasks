#include "file.h"

#include "filesystem.h"


bool File::rename(std::string_view new_name) {
  if(name.empty()){
	return false;
  }

  auto current_name = this->name;

  if (std::shared_ptr<Filesystem> spt = partOfFileSystem.lock()) {
	return spt->rename_file(current_name, new_name);
  }

  return false;

}

const std::string &File::get_name() const { return this->name; }

const FileContent &File::get_content() const { return this->content; }

File::File(FileContent &&content, std::string_view name) {

  this->content.update(std::move(content));

  this->name = name;
}
