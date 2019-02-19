/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <cstdlib>
#include <unordered_map>

#include <flashlight/distributed/distributed.h>

#include "Distributed.h"
#include "common/Defines.h"

namespace w2l {

#if BUILD_DISTRIBUTED
  void initDistributedEnv(
      int worldRank,
      int worldSize,
      const std::string& rndvFilepath) {

    if (rndvFilepath.empty()) {
      distributedInit(
          fl::DistributedInit::MPI,
          -1, // unused for MPI
          -1, // unused for MPI
          {{fl::DistributedConstants::kMaxDevicePerNode,
            std::to_string(kMaxDevicePerNode)}});
    } else {
      distributedInit(
          fl::DistributedInit::FILE_SYSTEM,
          worldRank,
          worldSize,
          {{fl::DistributedConstants::kMaxDevicePerNode,
            std::to_string(kMaxDevicePerNode)},
          {fl::DistributedConstants::kFilePath, rndvFilepath}});
    }
  }

  int getWorldSize() {
    fl::getWorldSize();
  }

  int getWorldRank() {
    fl::getWorldRank();
  }
#else
  void initDistributedEnv(
    int worldRank,
    int worldSize,
    const std::string& rndvFilepath) { return; }

  int getWorldSize() { return 1; }

  int getWorldRank() { return 0; }
#endif

} // namespace w2l
