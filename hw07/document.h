#pragma once

#include "file.h"

/**
 * Derived class for text documents.
 */
class Document : public File {
public:
  Document(FileContent &&content = {});

  /**
 * Get the file type identifier.
 */
  std::string_view get_type() const;

  /**
   * Calculate the raw (after uncompressing) size of the file from metadata only.
   */
  size_t get_raw_size() const;


  void update(FileContent &&new_content);


  /**
   * Return the number of non-whitespace characters in the file content.
   */
  unsigned get_character_count() const;


};
