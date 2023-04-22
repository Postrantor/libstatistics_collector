# libstatistics_collector

![License](https://img.shields.io/github/license/ros-tooling/libstatistics_collector)
[![Test libstatistics_collector](https://github.com/ros-tooling/libstatistics_collector/workflows/Test%20libstatistics_collector/badge.svg)](https://github.com/ros-tooling/libstatistics_collector/actions?query=workflow%3A%22Test+libstatistics_collector%22)

## Description

This C++ library provides the following:

- A `Collector` interface for implementing classes that collect observed data and generate statistics for them
- A `TopicStatisticsCollector` interface for implementing classes that collect and perform measurements for ROS2 topic statistics. Classes for calculating ROS 2 message age and message period statistics are also implemented.
- A `MovingAverageStatistics` class for calculating moving average statistics

> - 一个用于实施类的`collector`接口
> - 一个用于实现对 ROS2 主题统计的测量类的类的 `topicstatisticsCollector` 接口。还实施了用于计算 ROS 2 消息年龄和消息周期统计信息的类。
> - 用于计算移动平均统计数据

## Quality Declaration

This package claims to be in the Quality Level 1 category, see the [Quality Declaration](./QUALITY_DECLARATION.md) for more details.
