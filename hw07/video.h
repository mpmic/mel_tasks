#pragma once

#include "file.h"

#include <array>

/**
 * Derived class for storing videos.
 * Additionally stores the video resolution and its duration.
 */
class Video : public File {
public:
  using resolution_t = std::array<size_t, 2>;

  Video(FileContent &&content = {}, resolution_t resolution = {0, 0},
        double duration = 0);

  /**
* Get the file type identifier.
*/
  std::string_view get_type() const;

  /**
   * Calculate the raw (after uncompressing) size of the file from metadata only.
   */
  size_t get_raw_size() const;

  resolution_t get_resolution() const;
  double get_duration() const;

  void update(FileContent &&new_content, resolution_t size, double duration);

protected:
  resolution_t resolution;
  double duration;
};
