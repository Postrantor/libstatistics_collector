// Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <mutex>
#include <sstream>
#include <string>

#include "libstatistics_collector/collector/collector.hpp"
#include "libstatistics_collector/moving_average_statistics/moving_average.hpp"
#include "libstatistics_collector/moving_average_statistics/types.hpp"

namespace libstatistics_collector
{
namespace collector
{

/**
 * @brief 开始收集数据 (Start collecting data)
 *
 * @return true 如果成功开始收集数据 (if successfully started collecting data)
 * @return false 如果已经开始收集数据或者出现其他错误 (if already started collecting data or other errors occurred)
 */
bool Collector::Start()
{
  // 创建一个独占锁，保护对 started_ 的访问 (Create a unique lock to protect access to started_)
  std::unique_lock<std::mutex> ulock{mutex_};

  // 如果已经开始收集数据，返回 false (If already started collecting data, return false)
  if (started_) {
    return false;
  }

  // 设置 started_ 为 true (Set started_ to true)
  started_ = true;

  // 调用 SetupStart() 函数 (Call the SetupStart() function)
  return SetupStart();
}

/**
 * @brief 停止收集数据 (Stop collecting data)
 *
 * @return true 如果成功停止收集数据 (if successfully stopped collecting data)
 * @return false 如果还未开始收集数据或者出现其他错误 (if not yet started collecting data or other errors occurred)
 */
bool Collector::Stop()
{
  bool ret = false;

  // 创建一个独占锁，保护对 started_ 的访问 (Create a unique lock to protect access to started_)
  {
    std::unique_lock<std::mutex> ulock{mutex_};

    // 如果还未开始收集数据，返回 false (If not yet started collecting data, return false)
    if (!started_) {
      return false;
    }

    // 设置 started_ 为 false (Set started_ to false)
    started_ = false;

    // 调用 SetupStop() 函数 (Call the SetupStop() function)
    ret = SetupStop();
  }

  // 清除当前的测量数据 (Clear current measurements)
  ClearCurrentMeasurements();

  return ret;
}

/**
 * @brief 添加新的测量数据 (Add a new measurement)
 *
 * @param measurement 新的测量数据 (The new measurement data)
 */
void Collector::AcceptData(const double measurement)
{
  collected_data_.AddMeasurement(measurement);
}

/**
 * @brief 获取统计结果 (Get statistics results)
 *
 * @return moving_average_statistics::StatisticData 统计结果数据结构 (Statistics result data structure)
 */
moving_average_statistics::StatisticData Collector::GetStatisticsResults() const
{
  return collected_data_.GetStatistics();
}

/**
 * @brief 清除当前的测量数据 (Clear current measurements)
 */
void Collector::ClearCurrentMeasurements() { collected_data_.Reset(); }

/**
 * @brief 判断是否已经开始收集数据 (Check if started collecting data)
 *
 * @return true 如果已经开始收集数据 (if already started collecting data)
 * @return false 如果还未开始收集数据 (if not yet started collecting data)
 */
bool Collector::IsStarted() const
{
  // 创建一个独占锁，保护对 started_ 的访问 (Create a unique lock to protect access to started_)
  std::unique_lock<std::mutex> ulock{mutex_};

  return started_;
}

/**
 * @brief 获取收集器状态字符串 (Get collector status string)
 *
 * @return std::string 收集器状态字符串 (Collector status string)
 */
std::string Collector::GetStatusString() const
{
  // 创建一个字符串流 (Create a stringstream)
  std::stringstream ss;

  // 将收集器
  ss << "started=" << (IsStarted() ? "true" : "false") << ", "
     << StatisticsDataToString(GetStatisticsResults());
  return ss.str();
}

} // namespace collector
} // namespace libstatistics_collector
