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

#ifndef LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__CONSTANTS_HPP_
#define LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__CONSTANTS_HPP_

#include <string>

namespace libstatistics_collector
{
namespace topic_statistics_collector
{

namespace topic_statistics_constants
{
// 定义消息年龄统计名称常量（Define the message age statistic name constant）
constexpr const char kMsgAgeStatName[] = "message_age";
// 定义消息周期统计名称常量（Define the message period statistic name constant）
constexpr const char kMsgPeriodStatName[] = "message_period";
// 定义毫秒单位名称常量（Define the millisecond unit name constant）
constexpr const char kMillisecondUnitName[] = "ms";

// 定义收集统计信息主题名称参数常量（Define the collect statistics topic name parameter constant）
constexpr const char kCollectStatsTopicNameParam[] = "collect_topic_name";
// 定义发布统计信息主题名称参数常量（Define the publish statistics topic name parameter constant）
constexpr const char kPublishStatsTopicNameParam[] = "publish_topic_name";
} // namespace topic_statistics_constants

} // namespace topic_statistics_collector
} // namespace libstatistics_collector

#endif // LIBSTATISTICS_COLLECTOR__TOPIC_STATISTICS_COLLECTOR__CONSTANTS_HPP_
