// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <13alexac@gmail.com>

#ifndef BASE_PATH_H_
#define BASE_PATH_H_

#include <memory>
#include <string>

namespace base {

/// Cross-platform abstraction around the filesystem path.
class Path {
 public:
  /// Platform-dependent implementation class.
  class Impl {
   public:
    /// Destructor.
    virtual ~Impl() {}
    /// Appends part to the current path.
    /// @param[in] filename Part of path.
    virtual void Append(const std::string& filename) = 0;
    /// Create new path from current and filename.
    /// @param[in] filename Part of path.
    /// @returns Implementation of new path.
    virtual std::unique_ptr<Impl> Join(const std::string& filename) const;
    /// Removes last part of the current path.
    virtual void ToParentDir() = 0;
    /// Creates new path from current, without last part.
    /// @returns Implementation for parrent directory path.
    virtual std::unique_ptr<Impl> ParentDir() const;
    /// Checks existing of the current path.
    /// @returns true if current path exists.
    virtual bool IsExists() const = 0;
    /// Checks that current path exists and it's a directory.
    /// @returns true if current path is a directory.
    virtual bool IsDirectory() const = 0;
    /// Checks that current path is equivalent of argument.
    /// @param[in] path Path to compare with.
    /// @returns true if path is equivalent.
    virtual bool IsEqualTo(const Impl* path) const = 0;
    /// Converts to the string with absolute path.
    /// @returns absolute path in the current platfrom form.
    virtual std::string GetSpec() const = 0;
    /// Converts to the file:/// Url.
    /// @returns absolute path with file:// schema and / as a path delimiter.
    virtual std::string GetUrl() const = 0;
    /// Makes a copy of current implementation.
    /// @returns New instance of current class.
    virtual std::unique_ptr<Impl> Copy() const = 0;
  };

  /// Constructor from string representation.
  /// @param[in] path String representation of the path.
  explicit Path(const std::string& path);
  /// Copy constructor.
  /// @param[in] path Object to copy from.
  Path(const Path& path);
  /// Move constructor.
  /// @param[in] path Object to move from.
  Path(Path&& path);
  /// Default constructor.
  ///
  /// Creates path to the root.
  Path();

  /// Assign-copy operator.
  /// @param[in] path Object to copy from.
  /// @returns reference to the this object.
  Path& operator=(const Path& path);
  /// Assign-move operator.
  /// @param[in] path Object to move from.
  /// @returns reference to the this object.
  Path& operator=(Path&& path);

  /// Checks that paths are equal.
  /// @param[in] path Path to compare with the current object.
  bool operator==(const Path& path) const {
    return impl_->IsEqualTo(path.impl_.get());
  }

  /// Checks that paths are not equal.
  /// @param[in] path Path to compare with the current object.
  bool operator!=(const Path& path) {
    return !impl_->IsEqualTo(path.impl_.get());
  }

  /// Appends part of path to the current one.
  /// @param[in] filename Part of path to append.
  void Append(const std::string& filename) {
    impl_->Append(filename);
  }
  /// Creates a new path from current one and part of path.
  /// @param[in] filename Part of path to append.
  /// @returns new Path created from this one and given part.
  Path Join(const std::string& filename) const {
    return Path(impl_->Join(filename));
  }

  /// Throws away last part of path.
  void ToParentDir() {
    impl_->ToParentDir();
  }

  /// Creates a new path by deleting last part of this path.
  /// @returns Path to the parent directory.
  Path ParentDir() const {
    return Path(impl_->ParentDir());
  }

  /// Checks that path exsists.
  /// @returns true if path exsists.
  bool IsExists() const {
    return impl_->IsExists();
  }
  /// Checks that path exsists and file on path is a directory.
  /// @returns true if path points to the directory.
  bool IsDirectory() const {
    return impl_->IsDirectory();
  }

  /// Get platform-specific string representation of the path.
  /// @returns String with path.
  std::string spec() const {
    return impl_->GetSpec();
  }
  /// Get url-like representation of the path.
  ///
  /// On each platforms this url will begin with schema file:// and all path
  /// separators will be '/'.
  std::string url() const {
    return impl_->GetUrl();
  }

 protected:
  friend class PathService;
  /// Construct a path with given implementation object.
  /// @param impl Implementation object.
  explicit Path(std::unique_ptr<Impl>&& impl);

 private:
  std::unique_ptr<Impl> impl_;
};

}  // namespace base

#endif  // BASE_PATH_H_
