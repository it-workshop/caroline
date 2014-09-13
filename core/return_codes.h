// Copyright (c) 2014 The Caroline authors. All rights reserved.
// Use of this source file is governed by a MIT license that can be found in the
// LICENSE file.
/// @author Aleksandr Derbenev <alex@technoworks.ru>

#ifndef CORE_RETURN_CODES_H_
#define CORE_RETURN_CODES_H_

namespace core {

// Possible return codes of application. Append your own to the end of this
// list.
enum ReturnCode {
  // Program exits cleanly.
  RETURN_OK,
  // Application can't initialize right.
  RETURN_APPLICATION_INIT_FAIL,
  // Less than two frames was captured.
  RETURN_WRONG_FRAMES_COUNT,
};

}  // namespace core

#endif  // CORE_RETURN_CODES_H_
