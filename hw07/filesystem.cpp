#include "filesystem.h"

#include <algorithm>
#include <iomanip>
#include <numeric>
#include <sstream>

Filesystem::Filesystem() {}

bool Filesystem::register_file(const std::string &name,
                               const std::shared_ptr<File> &file) {

  if( name.empty() ||
  		file == nullptr){
	return false;
  }

  if( file->get_content().string_ == nullptr){
	return false;
  }

  if ((files.find(name) == files.end()) && (file->get_name().empty())) {
	// not found
	file->name = name;
	file->partOfFileSystem = std::move(this->shared_from_this());
	files[name] = file;
	return true;
  }

	return false;
}

bool Filesystem::remove_file(std::string_view name) {
  auto it = (files.find(static_cast<std::string>(name)));

  if (it == files.end()) {
	// not found
	return false;
  }

  auto fileToErase = it->second;
  fileToErase->name = "";

  files.erase(it);

  return true;
}

bool Filesystem::rename_file(std::string_view source, std::string_view dest) {

  if( source.empty() ||
	  dest.empty()){
	return false;
  }

  auto source_it = (files.find(static_cast<std::string>(source)));
  auto destination_it = (files.find(static_cast<std::string>(dest)));

  if ((source_it != files.end()) && (destination_it == files.end())) {
	// If source is found in files and destination not found
	auto file = source_it->second;
	file->name = dest;

	auto nodeHandle = files.extract(static_cast<std::string>(source));
	nodeHandle.key() = static_cast<std::string>(dest);
	files.insert(std::move(nodeHandle));


	return true;
  }


  return false;


}

std::shared_ptr<File> Filesystem::get_file(std::string_view name) const {

  if( name.empty()){
	return nullptr;
  }

  if ((files.find(static_cast<std::string>(name)) == files.end())) {
	// not found
	return nullptr;
  }

  return files.at(static_cast<std::string>(name));

}

size_t Filesystem::get_file_count() const {
  return files.size();
}

size_t Filesystem::in_use() const {
  size_t sumOfFileSizes = 0;
  for(auto const& [name,file] : files){

	sumOfFileSizes += file->get_size();
  }

  return sumOfFileSizes;
}

// convenience function so you can see what files are stored
std::string Filesystem::file_overview(bool sort_by_size) {
  std::ostringstream output;
  // this function is not tested, but it may help you when debugging.

  output << "files in filesystem: " << std::endl;

   for (auto&& entry : this->files) {
       output << "Name:" << entry.first << " Size:" << entry.second->get_size() << std::endl;
       output << "Type" << entry.second->get_type() << std::endl;
       output << "Content:" << entry.second->content.string_ << std::endl<< std::endl;
   }
  return std::move(output).str();
}

std::vector<std::shared_ptr<File>>
Filesystem::files_in_size_range(size_t max, size_t min) const {
  std::vector<std::shared_ptr<File>> sized_files;

  for(auto const& [name,file] : files){

	if ((file->get_size() >= min) && (file->get_size() <= max)){
		sized_files.emplace_back(file);
	}


  }

  return sized_files;
  }
