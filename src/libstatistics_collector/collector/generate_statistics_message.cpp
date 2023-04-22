// Copyright 2020 Amazon.com, Inc. or its affiliates. All Rights Reserved.
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

#include "libstatistics_collector/collector/generate_statistics_message.hpp"

#include <string>
#include <utility>

#include "statistics_msgs/msg/statistic_data_type.hpp"

namespace libstatistics_collector
{
namespace collector
{

/**
 * @brief 生成统计消息 (Generate Statistic Message)
 *
 * @param node_name 节点名称 (Node Name)
 * @param metric_name 指标名称 (Metric Name)
 * @param unit 单位 (Unit)
 * @param window_start 窗口开始时间 (Window Start Time)
 * @param window_stop 窗口结束时间 (Window Stop Time)
 * @param data 统计数据 (Statistic Data)
 * @return MetricsMessage 统计消息实例 (MetricsMessage Instance)
 */
using statistics_msgs::msg::MetricsMessage;
using statistics_msgs::msg::StatisticDataPoint;
using statistics_msgs::msg::StatisticDataType;

MetricsMessage GenerateStatisticMessage(
  const std::string & node_name,
  const std::string & metric_name,
  const std::string & unit,
  const builtin_interfaces::msg::Time window_start,
  const builtin_interfaces::msg::Time window_stop,
  const libstatistics_collector::moving_average_statistics::StatisticData & data)
{
  MetricsMessage msg; // 创建统计消息实例 (Create MetricsMessage instance)

  // 设置节点名称、指标名称和单位 (Set node name, metric name and unit)
  msg.measurement_source_name = node_name;
  msg.metrics_source = metric_name;
  msg.unit = unit;

  // 设置窗口开始和结束时间 (Set window start and stop time)
  msg.window_start = window_start;
  msg.window_stop = window_stop;

  msg.statistics.reserve(5); // 为统计数据预留 5 个空间 (Reserve space for 5 statistic data points)

  // 添加平均值到统计数据 (Add average to the statistics)
  msg.statistics.emplace_back();
  msg.statistics.back().data_type = StatisticDataType::STATISTICS_DATA_TYPE_AVERAGE;
  msg.statistics.back().data = data.average;

  // 添加最大值到统计数据 (Add maximum to the statistics)
  msg.statistics.emplace_back();
  msg.statistics.back().data_type = StatisticDataType::STATISTICS_DATA_TYPE_MAXIMUM;
  msg.statistics.back().data = data.max;

  // 添加最小值到统计数据 (Add minimum to the statistics)
  msg.statistics.emplace_back();
  msg.statistics.back().data_type = StatisticDataType::STATISTICS_DATA_TYPE_MINIMUM;
  msg.statistics.back().data = data.min;

  // 添加样本数量到统计数据 (Add sample count to the statistics)
  msg.statistics.emplace_back();
  msg.statistics.back().data_type = StatisticDataType::STATISTICS_DATA_TYPE_SAMPLE_COUNT;
  msg.statistics.back().data = static_cast<double>(data.sample_count);

  // 添加标准差到统计数据 (Add standard deviation to the statistics)
  msg.statistics.emplace_back();
  msg.statistics.back().data_type = StatisticDataType::STATISTICS_DATA_TYPE_STDDEV;
  msg.statistics.back().data = data.standard_deviation;

  return msg; // 返回统计消息实例 (Return MetricsMessage instance)
}

} // namespace collector
} // namespace libstatistics_collector
