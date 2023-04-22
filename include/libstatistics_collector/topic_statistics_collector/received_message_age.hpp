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

#ifndef LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_AGE_HPP_
#define LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_AGE_HPP_

#include <chrono>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>

#include "constants.hpp"

#include "libstatistics_collector/topic_statistics_collector/topic_statistics_collector.hpp"

#include "builtin_interfaces/msg/time.hpp"
#include "rcl/time.h"

namespace libstatistics_collector
{
namespace topic_statistics_collector
{

/**
 * @brief 如果消息没有头部，则为 false
 *        False if the message does not have a header
 * @tparam M 消息类型 Message type
 */
template <typename M, typename = void> struct HasHeader : public std::false_type
{
};

/**
 * @brief 如果消息具有名为 'header' 的字段，并且具有名为 'stamp' 的子字段，类型为 builtin_interfaces::msg::Time，则为 true
 *        True if the message has a field named 'header' with a subfield named 'stamp' of
 *        type builtin_interfaces::msg::Time
 * @tparam M 消息类型 Message type
 */
template <typename M>
struct HasHeader<
  M,
  typename std::enable_if<
    std::is_same<builtin_interfaces::msg::Time, decltype(M().header.stamp)>::value>::type>
: public std::true_type
{
};

/**
 * @brief 返回一个布尔标志，指示时间戳未设置，并在消息没有头部时返回零
 *        Return a boolean flag indicating the timestamp is not set
 *        and zero if the message does not have a header
 */
template <typename M, typename Enable = void> struct TimeStamp
{
  /**
   * @brief 提取消息头部的函数
   *        Function to extract the header from the message
   * @tparam M 要从中提取头部的消息类型 The message type to extract the header from
   * @tparam Enable 启用类型 Enable type
   * @return 一个包含布尔值和 int64_t 的 std::pair 对象 A std::pair object containing a bool and an int64_t
   */
  static std::pair<bool, int64_t> value(const M &) { return std::make_pair(false, 0); }
};

/**
 * @brief 如果消息的头部存在，则返回消息头部的时间戳（以纳秒为单位）
 *        Returns a message header's timestamp, in nanoseconds, if the message's
 *        header exists.
 */
template <typename M> struct TimeStamp<M, typename std::enable_if<HasHeader<M>::value>::type>
{
  /**
   * @brief 提取消息头部时间戳的函数
   *        Function to extract the header timestamp from the message
   * @tparam M 要从中提取头部时间戳的消息类型 The message type to extract the header timestamp from
   * @return 一个包含布尔值和 int64_t 的 std::pair 对象 A std::pair object containing a bool and an int64_t
   */
  static std::pair<bool, int64_t> value(const M & m)
  {
    const auto stamp =
      m.header.stamp; // 获取消息头部的时间戳 Get the timestamp from the message header
    const auto nanos = RCL_S_TO_NS(static_cast<int64_t>(stamp.sec)) +
                       stamp.nanosec; // 将时间戳转换为纳秒 Convert the timestamp to nanoseconds
    return std::make_pair(
      true,
      nanos); // 返回包含布尔值和纳秒的 std::pair 对象 Return a std::pair object containing a bool and the nanoseconds
  }
};

/**
 * @class ReceivedMessageAgeCollector
 * @brief 用于测量从 ROS2 订阅者接收到的消息的年龄的类。Class used to measure the received message age from a ROS2 subscriber.
 *
 * @tparam T 从订阅者/侦听器接收的消息类型。The message type to receive from the subscriber/listener.
 */
template <typename T> class ReceivedMessageAgeCollector : public TopicStatisticsCollector<T>
{
public:
  // 默认构造函数。Default constructor.
  ReceivedMessageAgeCollector() = default;

  // 默认析构函数。Default destructor.
  virtual ~ReceivedMessageAgeCollector() = default;

  /**
   * @brief 处理新传入的消息。如果存在有效的 Header，则计算消息年龄。
   * Handle a new incoming message. Calculate message age if a valid Header is present.
   *
   * @param received_message 要计算年龄的消息。The message to calculate age of.
   * @param now_nanoseconds 以纳秒为单位接收到消息的时间。Time the message was received in nanoseconds.
   */
  void OnMessageReceived(
    const T & received_message, const rcl_time_point_value_t now_nanoseconds) override
  {
    // 从消息头获取时间戳。Get timestamp from header.
    const auto timestamp_from_header = TimeStamp<T>::value(received_message);

    // 如果时间戳有效，则进行比较。Only compare if the timestamp is valid.
    if (timestamp_from_header.first) {
      // 只有在非零的情况下才进行比较。Only compare if non-zero.
      if (timestamp_from_header.second && now_nanoseconds) {
        // 计算消息年龄（纳秒）。Calculate message age in nanoseconds.
        const std::chrono::nanoseconds age_nanos{now_nanoseconds - timestamp_from_header.second};
        // 将消息年龄转换为毫秒。Convert message age to milliseconds.
        const auto age_millis = std::chrono::duration<double, std::milli>(age_nanos);

        // 接受数据。Accept the data.
        collector::Collector::AcceptData(static_cast<double>(age_millis.count()));
      } // else no valid time to compute age
    }
  }

  /**
   * @brief 返回消息年龄度量名称。Return message age metric name.
   *
   * @return 代表消息年龄度量名称的字符串。A string representing message age metric name.
   */
  std::string GetMetricName() const override { return topic_statistics_constants::kMsgAgeStatName; }

  /**
   * @brief 返回消息年龄度量单位。Return message age metric unit.
   *
   * @return 代表消息年龄度量单位的字符串。A string representing message age metric unit.
   */
  std::string GetMetricUnit() const override
  {
    return topic_statistics_constants::kMillisecondUnitName;
  }

protected:
  // 设置开始状态。Setup start state.
  bool SetupStart() override { return true; }

  // 设置停止状态。Setup stop state.
  bool SetupStop() override { return true; }
};

} // namespace topic_statistics_collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_AGE_HPP_
