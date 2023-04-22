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

#ifndef LIBSTATISTICS_COLLECTOR__COLLECTOR__GENERATE_STATISTICS_MESSAGE_HPP_
#define LIBSTATISTICS_COLLECTOR__COLLECTOR__GENERATE_STATISTICS_MESSAGE_HPP_

#include <string>

#include "builtin_interfaces/msg/time.hpp"
#include "statistics_msgs/msg/metrics_message.hpp"

#include "libstatistics_collector/moving_average_statistics/types.hpp"
#include "libstatistics_collector/visibility_control.hpp"

namespace libstatistics_collector
{
namespace collector
{

/**
 * 返回一个有效的 MetricsMessage，可以发布到 ROS 主题上
 * Return a valid MetricsMessage ready to be published to a ROS topic
 *
 * @param node_name 数据来源的节点名称 (the name of the node that the data originates from)
 * @param metric_name 指标名称（如 "cpu_usage"，"memory_usage" 等）(the name of the metric ("cpu_usage", "memory_usage", etc.))
 * @param unit 单位名称（如 "percentage"，"mb" 等）(name of the unit ("percentage", "mb", etc.))
 * @param window_start 测量窗口开始时间 (measurement window start time)
 * @param window_stop 测量窗口结束时间 (measurement window end time)
 * @param data 从窗口中测量得出的统计数据 (statistics derived from the measurements made in the window)
 * @return 包含 data 参数中统计数据的 MetricsMessage (a MetricsMessage containing the statistics in the data parameter)
 */
LIBSTATISTICS_COLLECTOR_PUBLIC
statistics_msgs::msg::MetricsMessage GenerateStatisticMessage(
  const std::string & node_name, // 节点名称 (node name)
  const std::string & metric_name, // 指标名称 (metric name)
  const std::string & unit, // 单位名称 (unit name)
  const builtin_interfaces::msg::Time window_start, // 测量窗口开始时间 (measurement window start time)
  const builtin_interfaces::msg::Time window_stop, // 测量窗口结束时间 (measurement window end time)
  const libstatistics_collector::moving_average_statistics::StatisticData & data); // 统计数据 (statistic data)

} // namespace collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__COLLECTOR__GENERATE_STATISTICS_MESSAGE_HPP_
