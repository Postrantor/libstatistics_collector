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

#ifndef LIBSTATISTICS_COLLECTOR__MOVING_AVERAGE_STATISTICS__MOVING_AVERAGE_HPP_
#define LIBSTATISTICS_COLLECTOR__MOVING_AVERAGE_STATISTICS__MOVING_AVERAGE_HPP_

#include <cmath>

#include <algorithm>
#include <limits>
#include <mutex>
#include <numeric>
#include <type_traits>

#include "types.hpp"

#include "libstatistics_collector/visibility_control.hpp"

#include "rcpputils/thread_safety_annotations.hpp"

namespace libstatistics_collector
{
namespace moving_average_statistics
{

/**
 * @class MovingAverageStatistics
 * @brief 一个用于计算移动平均统计的类。该类在常量内存和常量时间内运行。注意：需要手动调用 reset() 以开始新的测量窗口。
 *        A class for calculating moving average statistics. This operates in constant memory and constant time.
 *        Note: reset() must be called manually in order to start a new measurement window.
 *
 * 统计数据包括平均值、最大值、最小值和标准偏差（总体）。所有这些都是在不存储观测数据的情况下在线计算的。
 * The statistics calculated are average, maximum, minimum, and standard deviation (population). All are
 * calculated online without storing the observation data.
 *
 * 具体而言，平均值是一个运行总和，方差是通过 Welford 的在线算法获得的（用于计算标准偏差）。
 * Specifically, the average is a running sum and the variance is obtained by Welford's online algorithm
 * (reference: https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm)
 * for standard deviation.
 *
 * 当统计数据不可用时，例如没有进行观测，返回 NaN。
 * When statistics are not available, e.g. no observations have been made, NaNs are returned.
 */
class MovingAverageStatistics
{
public:
  // 构造函数，默认构造函数
  // Constructor, default constructor
  LIBSTATISTICS_COLLECTOR_PUBLIC
  MovingAverageStatistics() = default;

  // 析构函数，默认析构函数
  // Destructor, default destructor
  LIBSTATISTICS_COLLECTOR_PUBLIC
  ~MovingAverageStatistics() = default;

  /**
   *  返回所有记录数据的算术平均值。如果没有观察值，则返回 NaN。
   *  Returns the arithmetic mean of all data recorded. If no observations have been made, returns NaN.
   *
   *  @return 所有记录数据的算术平均值，或在样本计数为0时返回 NaN。
   *          The arithmetic mean of all data recorded, or NaN if the sample count is 0.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  double Average() const RCPPUTILS_TSA_REQUIRES(mutex_);

  /**
   *  返回记录的最大值。如果列表长度为零，则返回 NaN。
   *  Returns the maximum value recorded. If size of list is zero, returns NaN.
   *
   *  @return 记录的最大值，或在数据大小为零时返回 NaN。
   *          The maximum value recorded, or NaN if size of data is zero.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  double Max() const RCPPUTILS_TSA_REQUIRES(mutex_);

  /**
   *  返回记录的最小值。如果列表长度为零，则返回 NaN。
   *  Returns the minimum value recorded. If size of list is zero, returns NaN.
   *
   *  @return 记录的最小值，或在数据大小为零时返回 NaN。
   *          The minimum value recorded, or NaN if size of data is zero.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  double Min() const RCPPUTILS_TSA_REQUIRES(mutex_);

  /**
   *  返回所有记录数据的标准差（总体）。如果列表长度为零，则返回 NaN。
   *  Returns the standard deviation (population) of all data recorded. If size of list is zero, returns NaN.
   *
   *  方差是通过 Welford 的在线算法得到的，
   *  Variance is obtained by Welford's online algorithm,
   *  参见 https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm
   *  see https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Welford%27s_online_algorithm
   *
   *  @return 所有记录数据的标准差（总体），或在数据大小为零时返回 NaN。
   *          The standard deviation (population) of all data recorded, or NaN if size of data is zero.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  double StandardDeviation() const RCPPUTILS_TSA_REQUIRES(mutex_);

  /**
   *  返回一个 StatisticData 对象，包含平均值、最小值、最大值、标准差（总体）和样本计数。
   *  Return a StatisticData object, containing average, minimum, maximum, standard deviation (population),
   *  and sample count.
   *  如果没有观察值，平均值、最小值、最大值和标准差为 NaN。
   *  For the case of no observations, the average, min, max, and standard deviation are NaN.
   *
   *  @return 包含平均值、最小值、最大值、标准差（总体）和样本计数的 StatisticData 对象。
   *          StatisticData object, containing average, minimum, maximum, standard deviation (population),
   *          and sample count.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  StatisticData GetStatistics() const;

  /**
   * 重置所有计算值。相当于为移动平均设置一个新窗口。
   * Reset all calculated values. Equivalent to a new window for a moving average.
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  void Reset();

  /**
   * 观察给定窗口的样本。输入项用于计算统计数据。
   * Observe a sample for the given window. The input item is used to calculate statistics.
   * 注意：任何输入值为 NaN 的数据将被丢弃，不会作为测量值添加。
   * Note: any input values of NaN will be discarded and not added as a measurement.
   *
   * @param item 观察到的项目
   * @param item The item that was observed
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  virtual void AddMeasurement(const double item);

  /**
   * 返回观察到的样本数量
   * Return the number of samples observed
   *
   * @return 观察到的样本数量
   * @return the number of samples observed
   */
  LIBSTATISTICS_COLLECTOR_PUBLIC
  uint64_t GetCount() const;

private:
  mutable std::mutex mutex_;
  double average_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = 0;
  double min_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = std::numeric_limits<double>::max();
  double max_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = std::numeric_limits<double>::min();
  double sum_of_square_diff_from_mean_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = 0;
  uint64_t count_ RCPPUTILS_TSA_GUARDED_BY(mutex_) = 0;
};

} // namespace moving_average_statistics
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__MOVING_AVERAGE_STATISTICS__MOVING_AVERAGE_HPP_
