// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
// Author: Aleksandr Derbenev <13alexac@gmail.com>

#include "base/path_impl_posix.h"

#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <iterator>
#include <string>

#include "base/logging.h"
#include "base/utils_posix.h"

namespace base {

namespace {

std::string GetCWD() {
  char buffer[MAXPATHLEN];
  if (!getcwd(buffer, MAXPATHLEN)) {
    LOG(ERROR) << "getcwd: " << GetError();
    return ".";
  }
  return buffer;
}

std::string ToAbsolute(const std::string& path) {
  if (path.empty())
    return GetCWD();
  if (path.front() != '/')
    return GetCWD() + '/' + path;
  return path;
}

std::vector<std::string> SplitPath(const std::string& path) {
  std::vector<std::string> parts;
  for (std::string::size_type begin = 0, end = 0;
      end != std::string::npos && begin != path.size(); begin = end + 1) {
    end = path.find('/', begin);
    if (std::string::npos == end) {
      parts.push_back(path.substr(begin));
      break;
    }
    parts.push_back(path.substr(begin, end - begin));
  }

  return parts;
}

}  // namespace

PathImplPosix::PathImplPosix(const std::string& path)
  : path_(SplitPath(ToAbsolute(path))) {
  Optimize();
}

void PathImplPosix::Append(const std::string& filename) {
  const std::vector<std::string>& parts = SplitPath(filename);
  std::copy(parts.begin(), parts.end(), std::back_inserter(path_));
  Optimize();
}

void PathImplPosix::ToParentDir() {
  if (!path_.empty())
    path_.pop_back();
}

bool PathImplPosix::IsExists() const {
  struct stat stat_info;
  const std::string& path = GetSpec();
  if (stat(path.c_str(), &stat_info)) {
    if (errno != ENOENT)
      LOG(ERROR) << "stat(\"" << path << "\"): " << GetError();
    return false;
  }
  return true;
}

bool PathImplPosix::IsDirectory() const {
  struct stat stat_info;
  const std::string& path = GetSpec();
  if (stat(path.c_str(), &stat_info)) {
    if (errno != ENOENT)
      LOG(ERROR) << "stat(\"" << path  << "\"): " << GetError();
    return false;
  }
  return stat_info.st_mode & S_IFDIR;
}

bool PathImplPosix::IsEqualTo(const Impl* path) const {
  if (!path)
    return false;

  // On posix platform each impl of path must be a PathImplPosix.
  const PathImplPosix* posix_path = static_cast<const PathImplPosix*>(path);
  auto it1 = path_.begin(), it2 = posix_path->path_.begin(),
      end1 = path_.end(), end2 = posix_path->path_.end();
  for (; it1 != end1 && it2 != end2; ++it1, ++it2)
    if (*it1 != *it2)
      return false;
  return it1 == end1 && it2 == end2;
}

std::string PathImplPosix::GetSpec() const {
  std::string spec;
  for (auto part : path_)
    spec.append('/' + part);
  if (spec.empty())
    spec = "/";
  return spec;
}

std::string PathImplPosix::GetUrl() const {
  std::string url;
  return "file://" + GetSpec();
}

std::unique_ptr<Path::Impl> PathImplPosix::Copy() const {
  return std::unique_ptr<Path::Impl>(new PathImplPosix(*this));
}

void PathImplPosix::Optimize() {
  if (path_.size() == 1 && "/" == path_[0]) {
    path_.clear();
    return;
  }
  for (std::vector<std::string>::iterator it = path_.begin();
      it != path_.end();) {
    if (it->empty()
        || ("." == *it && path_.begin() != it)
        || (".." == *it && path_.begin() == it)) {
      it = path_.erase(it);
      continue;
    }
    if (".." == *it
          && it != path_.begin()
          && ".." != *(it - 1)
          && "." != *(it - 1)) {
        it = path_.erase(it - 1, it + 1);
        continue;
    }
    ++it;
  }
}

}  // namespace base
