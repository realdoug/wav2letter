/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#pragma once

#include <unordered_map>

#include <flashlight/flashlight.h>

#include "SpeechStatMeter.h"

#define LOG_MASTER(lvl) LOG_IF(lvl, (w2l::getWorldRank() == 0))

namespace w2l {
struct EditDistMeters {
  fl::EditDistanceMeter edit, wordedit;
};

struct TrainMeters {
  fl::TimeMeter runtime;
  fl::TimeMeter timer{true};
  fl::TimeMeter sampletimer{true};
  fl::TimeMeter fwdtimer{true}; // includes network + criterion time
  fl::TimeMeter critfwdtimer{true};
  fl::TimeMeter bwdtimer{true}; // includes network + criterion time
  fl::TimeMeter optimtimer{true};

  EditDistMeters train;
  std::unordered_map<std::string, EditDistMeters> valid;

  fl::AverageValueMeter loss;
  SpeechStatMeter stats;
};

struct TestMeters {
  fl::TimeMeter timer;
  fl::EditDistanceMeter werSlice;
  fl::EditDistanceMeter wer;
  fl::EditDistanceMeter lerSlice;
  fl::EditDistanceMeter ler;
};

std::pair<std::string, std::string> getStatus(
    TrainMeters& meters,
    int64_t epoch,
    double lr,
    double lrcrit,
    bool verbose = false,
    bool date = false,
    const std::string& separator = " ");

void appendToLog(std::ofstream& logfile, const std::string& logstr);

af::array allreduceGet(fl::AverageValueMeter& mtr);
af::array allreduceGet(fl::EditDistanceMeter& mtr);
af::array allreduceGet(SpeechStatMeter& mtr);
af::array allreduceGet(fl::CountMeter& mtr);
af::array allreduceGet(fl::TimeMeter& mtr);

void allreduceSet(fl::AverageValueMeter& mtr, af::array& val);
void allreduceSet(fl::EditDistanceMeter& mtr, af::array& val);
void allreduceSet(SpeechStatMeter& mtr, af::array& val);
void allreduceSet(fl::CountMeter& mtr, af::array& val);
void allreduceSet(fl::TimeMeter& mtr, af::array& val);

template <typename T>
void syncMeter(T& mtr) {
  #if BUILD_DISTRIBUTED
    if (!fl::isDistributedInit()) {
      return;
    }
    af::array arr = allreduceGet(mtr);
    fl::allReduce(arr);
    allreduceSet(mtr, arr);
  #else
    return;
  #endif
}
template <>
void syncMeter<TrainMeters>(TrainMeters& mtrs);

} // namespace w2l
