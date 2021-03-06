// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Glazachev Vladimir <glazachev.vladimir@gmail.com>

#include <fstream> // NOLINT
#include <vector>

#include "core/ply_saver.h"

namespace core {

PlySaver::PlySaver() {
  name_ = "mesh";
  comment_ = "";
}

void PlySaver::Save(Mesh *mesh) {
  if (!mesh) {
    return;
  }

  std::string file_name = name_ + ".ply";
  std::ofstream outfile;

  outfile.open(file_name);

  if (!outfile.is_open()) {
    return;
  }

  std::vector<cv::Point3d> vertexes = mesh->vertexes();
  std::vector<Triangle> faces = mesh->faces();

  outfile << "ply\n";
  outfile << "format ascii 1.0\n";

  if (!author_.empty()) {
    outfile << "comment author: " << author_ << "\n";
  }

  if (!comment_.empty()) {
    outfile << "comment object: " << comment_ << "\n";
  }

  outfile << "element vertex " << vertexes.size() << "\n";
  outfile << "property float x\n";
  outfile << "property float y\n";
  outfile << "property float z\n";

  outfile << "element face " << faces.size() << "\n";
  outfile << "property list uchar int vertex_index\n";
  outfile << "end_header\n";

  for (size_t i = 0; i < vertexes.size(); i++) {
    cv::Point3d point = vertexes.at(i);

    outfile << point.x << " "
            << point.y << " "
            << point.z << "\n";
  }

  for (size_t i = 0; i < faces.size(); i++) {
    Triangle face = faces.at(i);

    outfile << face.Point1() << " "
            << face.Point2() << " "
            << face.Point3() << "\n";
  }

  outfile.close();
}

void PlySaver::Save(const std::string &name, const std::string &comment,
    const std::string& author, Mesh *mesh) {
  name_ = name;
  comment_ = comment;
  author_ = author;

  Save(mesh);
}

}  // namespace core
