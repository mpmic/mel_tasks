#pragma once

#include "file.h"

/**
 * Represents audio files.
 */
class Audio : public File {
public:
  Audio(FileContent &&content = {}, unsigned duration = 0);

  /**
* Get the file type identifier.
*/
  std::string_view get_type() const;

  /**
   * Calculate the raw (after uncompressing) size of the file from metadata only.
   */
  size_t get_raw_size() const;

  /**
   * Get the duration of this audio file.
   */
  unsigned get_duration();

  void update(FileContent &&new_content, unsigned new_duration);

 private:
  unsigned duration;
};
