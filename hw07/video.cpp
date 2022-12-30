#include "video.h"

Video::Video(FileContent &&content, resolution_t resolution, double duration)
    : File{std::move(content)}, resolution{resolution}, duration{duration} {}

/**
* Get the file type identifier.
*/
std::string_view Video::get_type() const {
  return "VID";
}

/**
 * Calculate the raw (after uncompressing) size of the file from metadata only.
 */
size_t Video::get_raw_size() const{
  return 3 * resolution[0] * resolution[1] * static_cast<long>(30 * duration);
}


auto Video::get_resolution() const -> resolution_t { return this->resolution; }

double Video::get_duration() const { return this->duration; }

void Video::update(FileContent &&new_content, Video::resolution_t size, double duration) {
  content.update(std::move(new_content));
  resolution = size;
  this->duration = duration;
}

