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

#include <sstream>
#include <string>

#include "libstatistics_collector/moving_average_statistics/types.hpp"

namespace libstatistics_collector
{
namespace moving_average_statistics
{

/**
 * @brief 将统计数据转换为字符串 (Converts the statistics data into a string)
 * 
 * @param results 统计数据结构体 (StatisticData structure containing the statistical data)
 * @return std::string 转换后的字符串形式的统计数据 (String representation of the statistical data)
 */
std::string StatisticsDataToString(const StatisticData & results)
{
  // 创建一个字符串流对象，用于存储转换后的统计数据 (Create a stringstream object to store the converted statistical data)
  std::stringstream ss;

  // 将平均值 (average) 添加到字符串流中 (Add the average value to the stringstream)
  ss << "avg=" << std::to_string(results.average) << ", ";

  // 将最小值 (min) 添加到字符串流中 (Add the minimum value to the stringstream)
  ss << "min=" << std::to_string(results.min) << ", ";

  // 将最大值 (max) 添加到字符串流中 (Add the maximum value to the stringstream)
  ss << "max=" << std::to_string(results.max) << ", ";

  // 将标准差 (standard_deviation) 添加到字符串流中 (Add the standard deviation to the stringstream)
  ss << "std_dev=" << std::to_string(results.standard_deviation) << ", ";

  // 将样本数量 (sample_count) 添加到字符串流中 (Add the sample count to the stringstream)
  ss << "count=" << std::to_string(results.sample_count);

  // 返回转换后的字符串形式的统计数据 (Return the string representation of the statistical data)
  return ss.str();
}

} // namespace moving_average_statistics
} // namespace libstatistics_collector
