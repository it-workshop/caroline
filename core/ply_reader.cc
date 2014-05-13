// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <fstream> // NOLINT

#include "core/ply_reader.h"
#include "core/mesh.h"

namespace core {

const bool kFileOpened = true;
const bool kFileNotOpened = false;

PlyReader::PlyReader()
{
}

bool PlyReader::IsFile() const
{
  std::ofstream inputfile;

  inputfile.open(file_name_);

  if (!inputfile.is_open()) {
    return kFileNotOpened;
  }

  inputfile.close();
  return kFileOpened;
}

Mesh *PlyReader::ReadMesh() const
{
  std::ifstream inputfile;

  inputfile.open(file_name_);

  if (!inputfile.is_open()) {
    return nullptr;
  }

  std::vector<Point3D> vertexes;
  std::vector<Triangle> faces;

  std::string buf;

  inputfile >> buf;
  inputfile.close();
}

}  // namespace core
