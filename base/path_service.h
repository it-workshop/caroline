// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_PATH_SERVICE_H_
#define BASE_PATH_SERVICE_H_

#include <memory>
#include <string>

#include "base/macro.h"
#include "base/path.h"
#include "base/singleton.h"

namespace base {

/// Service that manages a pathes to the files.
///
/// This service is a singleton. Use static method GetInstance() to retrive a
/// pointer to the object.
class PathService : public Singleton<PathService> {
 public:
  PathService() {}
  /// Destructor.
  virtual ~PathService() {}

  /// Initialize instance with path to the executable. On POSIX there is no
  /// other way to get it.
  /// @param[in] executable_path Path to the current executable file.
  static bool Init(const std::string& executable_path);

  /// Create a Path object with given string path.
  /// @returns Path to the given file.
  static Path MakePath(const std::string& path);

  /// Root path can't change, so just keep it inside.
  /// @returns Path to the rood node of file system.
  Path root_path() const { return root_path_; }
  /// Path to the current executable file.
  ///
  /// Assume that path of executable file can't change while program run.
  /// It's true on Windows, but on POSIX system it can be changed, but we have
  /// no ways to know about it.
  /// @returns Path to the current executable file.
  Path executable_path() const { return executable_path_; }
  /// Path to the current directory.
  /// @returns Path to the current working directory.
  virtual Path GetWorkingDirectoryPath() const = 0;;

 protected:
  /// Path to the current executable file.
  Path executable_path_;
  /// Path to the root node of filesystem.
  Path root_path_;

  /// Create Path for given Path::Impl.
  static Path PathFromImpl(std::unique_ptr<Path::Impl>&& impl);

 private:
  DISALLOW_COPY_AND_ASSIGN(PathService);
};

}  // namespace base

#endif  // BASE_PATH_SERVICE_H_
