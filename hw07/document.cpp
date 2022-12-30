#include "document.h"


Document::Document(FileContent &&content) : File{std::move(content)} {}

/**
* Get the file type identifier.
*/
std::string_view Document::get_type() const{
  return "DOC";
}

/**
 * Calculate the raw (after uncompressing) size of the file from metadata only.
 */
size_t Document::get_raw_size() const {
  auto fileContentPointer = content.get();
  return fileContentPointer->size();
}

unsigned Document::get_character_count() const {

  auto fileContentPointer = content.get();
  auto numberOfWhiteSpaces = std::count(fileContentPointer->begin(),fileContentPointer->end(), ' ');
  auto numberOfNewLines = std::count(fileContentPointer->begin(),fileContentPointer->end(), '\n');
  auto numberOfTabs = std::count(fileContentPointer->begin(),fileContentPointer->end(), '\t');
  return fileContentPointer->size() - numberOfNewLines - numberOfTabs - numberOfWhiteSpaces;
}
void Document::update(FileContent &&new_content) {
  content.update(std::move(new_content));
}
