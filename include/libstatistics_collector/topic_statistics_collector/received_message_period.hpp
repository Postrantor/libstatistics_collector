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

#ifndef LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_PERIOD_HPP_
#define LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_PERIOD_HPP_

#include <chrono>
#include <mutex>
#include <string>

#include "constants.hpp"
#include "libstatistics_collector/topic_statistics_collector/topic_statistics_collector.hpp"

#include "rcl/time.h"

namespace libstatistics_collector
{
namespace topic_statistics_collector
{

constexpr const int64_t kUninitializedTime{0};

/**
 * @class ReceivedMessagePeriodCollector
 * @brief 用于测量从 ROS2 订阅者接收到的消息（tparam T）的周期。此类是线程安全的，在执行成员 OnMessageReceived 时获取互斥锁。
 *        Class used to measure the received message (tparam T) period from a ROS2 subscriber. This class
 *        is thread safe and acquires a mutex when the member OnMessageReceived is executed.
 *
 * @tparam T 要从订阅者/监听器接收的消息类型。The message type to receive from the subscriber / listener.
 */
template <typename T> class ReceivedMessagePeriodCollector : public TopicStatisticsCollector<T>
{
public:
  /**
   * @brief 构造一个 ReceivedMessagePeriodCollector 对象。
   *        Construct a ReceivedMessagePeriodCollector object.
   *
   */
  ReceivedMessagePeriodCollector() { ResetTimeLastMessageReceived(); }

  virtual ~ReceivedMessagePeriodCollector() = default;

  /**
 * 处理接收到的消息并测量其接收周期。此成员是线程安全的，并获取锁以防止在设置 time_last_message_received_ 成员时出现竞争条件。
 * Handle a message received and measure its received period. This member is thread safe and acquires
 * a lock to prevent race conditions when setting the time_last_message_received_ member.
 *
 * @param received_message 接收到的消息 (Received message)
 * @param now_nanoseconds 消息接收时间（纳秒）(Time the message was received in nanoseconds)
 */
  void OnMessageReceived(const T & received_message, const rcl_time_point_value_t now_nanoseconds)
    override RCPPUTILS_TSA_REQUIRES(mutex_)
  {
    // 使用 std::mutex 创建一个独占锁对象 (Create a unique_lock object using std::mutex)
    std::unique_lock<std::mutex> ulock{mutex_};

    // 忽略 received_message 参数 (Ignore the received_message parameter)
    (void)received_message;

    // 如果上次接收到消息的时间未初始化，则将其设置为当前时间 (If the time of last message received is uninitialized, set it to the current time)
    if (time_last_message_received_ == kUninitializedTime) {
      time_last_message_received_ = now_nanoseconds;
    } else {
      // 计算两个消息之间的时间差（纳秒）(Calculate the time difference between two messages in nanoseconds)
      const std::chrono::nanoseconds nanos{now_nanoseconds - time_last_message_received_};
      // 将纳秒转换为毫秒 (Convert nanoseconds to milliseconds)
      const auto period = std::chrono::duration<double, std::milli>(nanos);
      // 更新上次接收到消息的时间 (Update the time of last message received)
      time_last_message_received_ = now_nanoseconds;
      // 将周期（毫秒）作为数据传递给收集器 (Pass the period (milliseconds) as data to the collector)
      collector::Collector::AcceptData(static_cast<double>(period.count()));
    }
  }

  /**
 * 返回消息周期度量名称
 * Return message period metric name
 *
 * @return 表示消息周期度量名称的字符串 (A string representing message period metric name)
 */
  std::string GetMetricName() const override
  {
    return topic_statistics_constants::kMsgPeriodStatName;
  }

  /**
 * 返回消息周期度量单位
 * Return message period metric unit
 *
 * @return 表示消息周期度量单位的字符串 (A string representing message period metric unit)
 */
  std::string GetMetricUnit() const override
  {
    return topic_statistics_constants::kMillisecondUnitName;
  }

protected:
  /**
   * @brief 重置 time_last_message_received_ 成员。 (Reset the time_last_message_received_ member.)
   * @return 始终返回 true。 (Always return true.)
   */
  bool SetupStart() override
  {
    // 调用 ResetTimeLastMessageReceived() 函数来重置成员变量 time_last_message_received_。
    // (Call the ResetTimeLastMessageReceived() function to reset the member variable time_last_message_received_.)
    ResetTimeLastMessageReceived();

    // 返回 true，表示操作成功。 (Return true, indicating that the operation was successful.)
    return true;
  }

  /**
   * @brief 停止设置。 (Stop the setup.)
   * @return 始终返回 true。 (Always return true.)
   */
  bool SetupStop() override
  {
    // 直接返回 true，表示操作成功。 (Return true directly, indicating that the operation was successful.)
    return true;
  }

private:
  /**
   * 重置 time_last_message_received_ 到预期的 kUninitializedTime。
   * Resets time_last_message_received_ to the expected uninitialized_time_.
   */
  void ResetTimeLastMessageReceived() { time_last_message_received_ = kUninitializedTime; }

  /**
   * 默认未初始化时间。
   * Default uninitialized time.
   */
  rcl_time_point_value_t
    // time_last_message_received_ 是一个 rcl_time_point_value_t 类型的变量，用于存储上一次接收到消息的时间。
    // time_last_message_received_ is a variable of type rcl_time_point_value_t, used to store the time when the last message was received.
    time_last_message_received_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = kUninitializedTime;

  // mutex_ 是一个可变的 std::mutex 类型的变量，用于保护 time_last_message_received_ 的访问。
  // mutex_ is a mutable variable of type std::mutex, used to protect access to time_last_message_received_.
  mutable std::mutex mutex_;
};

} // namespace topic_statistics_collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__RECEIVED_MESSAGE_PERIOD_HPP_
