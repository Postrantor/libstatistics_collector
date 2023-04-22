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

#ifndef LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR_HPP_
#define LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR_HPP_

#include <chrono>
#include <string>

#include "rcl/time.h"

#include "libstatistics_collector/collector/collector.hpp"

namespace libstatistics_collector
{
namespace topic_statistics_collector
{

/**
 * @file topic_statistics_collector.hpp
 */

/**
 * @brief Interface to collect and perform measurements for ROS2 topic statistics.
 *
 * 接口用于收集和执行 ROS2 主题统计的测量。
 *
 * @tparam T the ROS2 message type to collect
 * @tparam T 要收集的 ROS2 消息类型
 */
template <typename T> class TopicStatisticsCollector : public collector::Collector
{
public:
  /**
   * @brief Default constructor
   * 默认构造函数
   */
  TopicStatisticsCollector() = default;

  /**
   * @brief Virtual destructor
   * 虚拟析构函数
   */
  virtual ~TopicStatisticsCollector() = default;

  /**
   * @brief Handle receiving a single message of type T.
   * 处理接收类型为 T 的单个消息。
   *
   * @param received_message T the ROS2 message type to collect
   * @param received_message T 要收集的 ROS2 消息类型
   * @param now_nanoseconds nanoseconds the time the message was received. Any metrics using this time assumes the
   * following 1). the time provided is strictly monotonic 2). the time provided uses the same source
   * as time obtained from the message header.
   * @param now_nanoseconds 纳秒接收到的消息的时间。使用此时间的任何度量都假定以下条件：
   * 1) 提供的时间是严格单调的；
   * 2) 提供的时间与从消息头获得的时间使用相同的源。
   */
  virtual void
  OnMessageReceived(const T & received_message, const rcl_time_point_value_t now_nanoseconds) = 0;
};

} // namespace topic_statistics_collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR_HPP_
