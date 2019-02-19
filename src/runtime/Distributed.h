/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <string>

namespace w2l {

void initDistributedEnv(
    int worldRank,
    int worldSize,
    const std::string& rndvFilepath);

int getWorldSize();

int getWorldRank();
}
