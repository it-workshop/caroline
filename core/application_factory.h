// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_APPLICATION_FACTORY_H_
#define CORE_APPLICATION_FACTORY_H_

#include <memory>

namespace base {
class CommandLine;
}

namespace core {

class Caroline;

/// Creates main application class. It can be core::Caroline or it's child for
/// demos, for example.
std::unique_ptr<Caroline> CreateApplication(base::CommandLine* command_line);

}  // namespace core

#endif  // CORE_APPLICATION_FACTORY_H_

