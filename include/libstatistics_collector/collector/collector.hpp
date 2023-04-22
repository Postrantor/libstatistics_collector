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

#ifndef LIBSTATISTICS_COLLECTOR__COLLECTOR__COLLECTOR_HPP_
#define LIBSTATISTICS_COLLECTOR__COLLECTOR__COLLECTOR_HPP_

#include <mutex>
#include <string>

#include "libstatistics_collector/moving_average_statistics/moving_average.hpp"
#include "libstatistics_collector/moving_average_statistics/types.hpp"
#include "libstatistics_collector/visibility_control.hpp"

#include "metric_details_interface.hpp"

#include "rcpputils/thread_safety_annotations.hpp"

namespace libstatistics_collector
{
namespace collector
{

/**
 * @class Collector
 * @brief 简单的类，用于收集观测数据并为给定的观测生成统计信息。
 *        Simple class in order to collect observed data and generate statistics for the given observations.
 */
class Collector : public MetricDetailsInterface
{
public:
  // 公共构造函数，用于创建一个默认的 Collector 实例。
  // Public constructor for creating a default Collector instance.
  LIBSTATISTICS_COLLECTOR_PUBLIC
  Collector() = default;

  // 公共虚析构函数，允许子类正确地销毁。
  // Public virtual destructor, allowing subclasses to be properly destroyed.
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual ~Collector() = default;

  /**
   * 添加观测到的测量值。这将聚合测量值并通过 moving_average 类计算统计数据。
   * Add an observed measurement. This aggregates the measurement and calculates statistics
   * via the moving_average class.
   *
   * @param measurement 观测到的测量值 (the measurement observed)
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual void AcceptData(const double measurement);

  /**
   * 返回所有观测数据的统计信息。
   * Return the statistics for all of the observed data.
   *
   * @return 所有观测测量值的 StatisticData（StatisticData for all the observed measurements）
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual moving_average_statistics::StatisticData GetStatisticsResults() const;

  /**
   * 清除/重置所有当前测量值。
   * Clear / reset all current measurements.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual void ClearCurrentMeasurements();

  /**
   * 如果已调用 start，则返回 true，否则返回 false。
   * Return true if start has been called, false otherwise.
   *
   * @return 此收集器的启动状态（the started state of this collector）
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  bool IsStarted() const;

  /**
   * 返回此类的详细打印状态表示
   * Return a pretty printed status representation of this class
   *
   * @return 详细显示当前状态的字符串（a string detailing the current status）
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual std::string GetStatusString() const;

  // TODO(dabonnie): uptime (once start has been called)

  /**
   * @brief 开始收集数据。在构造之后调用。注意：这会锁定递归互斥体类成员 'mutex'。
   *        Start collecting data. Meant to be called after construction. Note: this locks the recursive mutex class
   *        member 'mutex'.
   * @return 如果启动成功，返回 true；如果发生错误，返回 false。
   *         true if started, false if an error occurred.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual bool Start();

  /**
   * @brief 停止收集数据。作为拆卸方法（在销毁之前），但应该将类置于可重新启动的状态，即可以调用 start 来恢复收集。
   *        Stop collecting data. Meant to be a teardown method (before destruction, but should place the
   *        class in a restartable state, i.e., start can be called to be able to resume collection.
   *
   *        这个方法调用 ClearCurrentMeasurements。
   *        This calls ClearCurrentMeasurements.
   *
   * @return 如果停止成功，返回 true；如果发生错误，返回 false。
   *         true if stopped, false if an error occurred.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual bool Stop();

private:
  /**
   * @brief 重写此函数以执行必要的启动步骤。Override this function to perform necessary starting steps.
   *
   * @return 如果设置成功，则返回 true；否则返回 false。Return true if setup was successful, false otherwise.
   */
  virtual bool SetupStart() RCPPUTILS_TSA_REQUIRES(mutex_) = 0;

  /**
   * @brief 重写此函数以执行必要的拆卸操作。Override this function to perform necessary teardown.
   *
   * @return 如果拆卸成功，则返回 true；否则返回 false。Return true if teardown was successful, false otherwise.
   */
  virtual bool SetupStop() RCPPUTILS_TSA_REQUIRES(mutex_) = 0;

  // 定义一个互斥锁，用于同步访问共享资源。Define a mutex for synchronizing access to shared resources.
  mutable std::mutex mutex_;

  // 定义一个 moving_average_statistics::MovingAverageStatistics 类型的变量，用于收集数据。Define a variable of type moving_average_statistics::MovingAverageStatistics for collecting data.
  moving_average_statistics::MovingAverageStatistics collected_data_;

  // 定义一个布尔变量 started_，并使用 RCPPUTILS_TSA_GUARDED_BY(mutex_) 修饰符保护它。Define a boolean variable started_ and protect it with the RCPPUTILS_TSA_GUARDED_BY(mutex_) modifier.
  bool started_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = false;
};

} // namespace collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__COLLECTOR__COLLECTOR_HPP_
