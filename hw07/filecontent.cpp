#include <iostream>
#include "filecontent.h"


FileContent::FileContent(const std::string &content){
  string_ = std::make_shared<std::string>(content);
}

FileContent::FileContent(std::string &&content) {
  string_ = std::make_shared<std::string>(std::move(content));
}

FileContent::FileContent(const char *content){

  string_ = std::make_shared<std::string>(content);
}

size_t FileContent::get_size() const {
  if(!string_){
	return 0;
  }
  return string_->size();
}
std::shared_ptr<const std::string> FileContent::get() const {
  return std::shared_ptr<const std::string>(string_);
}
void FileContent::update(FileContent &&new_content) {
  string_ = new_content.string_;
}


