// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers, xsubyte dev's
//
// This file is part of Bytecoin.
//
// Bytecoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// xsubyte is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with xsubyte.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <atomic>
#include <thread>

#include <System/Dispatcher.h>
#include <System/Event.h>
#include <System/RemoteContext.h>

#include "CryptoNote.h"
#include "CryptoNoteCore/Difficulty.h"

#include "Logging/LoggerRef.h"

namespace CryptoNote {

struct BlockMiningParameters {
  BlockTemplate blockTemplate;
  Difficulty difficulty;
};

class Miner {
public:
  Miner(System::Dispatcher& dispatcher, Logging::ILogger& logger);
  ~Miner();

  BlockTemplate mine(const BlockMiningParameters& blockMiningParameters, size_t threadCount, uint32_t cryptonightVariant = 0);

  //NOTE! this is blocking method
  void stop();

private:
  System::Dispatcher& m_dispatcher;
  System::Event m_miningStopped;

  enum class MiningState : uint8_t { MINING_STOPPED, BLOCK_FOUND, MINING_IN_PROGRESS};
  std::atomic<MiningState> m_state;

  std::vector<std::unique_ptr<System::RemoteContext<void>>>  m_workers;

  BlockTemplate m_block;

  Logging::LoggerRef m_logger;

  void runWorkers(BlockMiningParameters blockMiningParameters, size_t threadCount, uint32_t cryptonightVariant = 0);
  void workerFunc(const BlockTemplate& blockTemplate, Difficulty difficulty, uint32_t nonceStep, uint32_t cryptonightVariant = 0);
  bool setStateBlockFound();
};

} //namespace CryptoNote
