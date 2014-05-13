// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#ifndef CORE_PLY_READER_H_
#define CORE_PLY_READER_H_

#include <string>
#include <vector>

namespace core {

class Mesh;

class PlyReader
{
 public:
  PlyReader();

  void SetFile(const std::string& file_name);
  bool IsFile() const;

  Mesh* ReadMesh() const;

 protected:
  bool ParseFileCap();
  bool ParseVertexes();
  bool ParseFaces();

 private:
  std::string file_name_;
};

}  // namespace core

#endif // CORE_PLY_READER_H_
