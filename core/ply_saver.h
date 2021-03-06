// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PLY_SAVER_H_
#define CORE_PLY_SAVER_H_

#include <string>

#include "core/mesh.h"

namespace core {

class PlySaver {
 public:
  PlySaver();

  void Save(Mesh *mesh);
  void Save(const std::string& name, const std::string& comment,
      const std::string &author, Mesh *mesh);

  void set_path(const std::string& path) { path_ = path; }
  void set_author(const std::string& author) { author_ = author; }
  void set_name(const std::string& name) { name_ = name; }
  void set_comment(const std::string& comment) { comment_ = comment; }

 protected:
  void SetDefaultProperties();
  void WriteToFile(Mesh *mesh);

 private:
  std::string path_;
  std::string name_;
  std::string author_;
  std::string comment_;
};

}  // namespace core

#endif  // CORE_PLY_SAVER_H_
