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

#ifndef LIBSTATISTICS_COLLECTOR__COLLECTOR__METRIC_DETAILS_INTERFACE_HPP_
#define LIBSTATISTICS_COLLECTOR__COLLECTOR__METRIC_DETAILS_INTERFACE_HPP_

#include <string>

#include "libstatistics_collector/visibility_control.hpp"

namespace libstatistics_collector
{
namespace collector
{

/**
 * @class MetricDetailsInterface
 * @brief 用于表示单个指标的名称和单位的接口，
 *        用于指标消息生成和发布。
 *        Interface to represent a single metric's name and unit,
 *        which are used for metric message generation and publication.
 */
class LIBSTATISTICS_COLLECTOR_PUBLIC MetricDetailsInterface
{
public:
  // 析构函数，使用默认实现
  // Destructor, using the default implementation
  virtual ~MetricDetailsInterface() = default;

  /**
   * @brief 返回单个指标的名称。
   *        Return a single metric's name.
   * @return 表示指标名称的字符串
   *         a string representing the metric name
   */
  virtual std::string GetMetricName() const = 0;

  /**
   * @brief 返回单个指标的度量单位。
   *        Return a single metric's measurement unit.
   * @return 表示指标单位的字符串
   *         a string representing the metric unit
   */
  virtual std::string GetMetricUnit() const = 0;
};

} // namespace collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__COLLECTOR__METRIC_DETAILS_INTERFACE_HPP_
