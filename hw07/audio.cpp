#include "audio.h"

Audio::Audio(FileContent &&content, unsigned duration)
    : File{std::move(content)}, duration{duration} {}


/**
* Get the file type identifier.
*/
std::string_view Audio::get_type() const {
  return "AUD";
}

/**
 * Calculate the raw (after uncompressing) size of the file from metadata only.
 */
size_t Audio::get_raw_size() const{
  return 2*48000*2*duration;
}


unsigned Audio::get_duration() { return this->duration; }


void Audio::update(FileContent &&new_content, unsigned int new_duration) {
  content.update(std::move(new_content));
  duration = new_duration;

}
