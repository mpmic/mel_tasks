#include "image.h"

Image::Image(FileContent &&content, resolution_t res)
    : File{std::move(content)}, resolution{res} {}

/**
* Get the file type identifier.
*/
std::string_view Image::get_type() const {
  return "IMG";
}

/**
 * Calculate the raw (after uncompressing) size of the file from metadata only.
 */
size_t Image::get_raw_size() const{
  return resolution[0] * resolution[1] * 4 ;
}


auto Image::get_resolution() const -> resolution_t { return this->resolution; }


void Image::update(FileContent &&new_content, Image::resolution_t size) {
  content.update(std::move(new_content));
  resolution = size;
}