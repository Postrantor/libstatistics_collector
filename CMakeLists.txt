cmake_minimum_required(VERSION 3.5)
project(libstatistics_collector)

# 默认使用 C99 标准 (Default to C99 standard)
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# 默认使用 C++17 标准 (Default to C++17 standard)
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  # 设置 C++ 标准为必需的 (Set C++ standard as required)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# 如果可能，启用严格的编译器标志 (Enable strict compiler flags if possible)
include(CheckCXXCompilerFlag)
set(FLAGS
    -pedantic
    -Wno-long-long
    -Wall
    -Wextra
    -Wcast-qual
    -Wformat
    -Wwrite-strings
    -Wcast-align
    -Wno-error=cast-align
    -Wmissing-declarations
    -Wconversion)
foreach(FLAG ${FLAGS})
  # 检查编译器是否支持当前标志 (Check if the compiler supports the current flag)
  check_cxx_compiler_flag(${FLAG} R${FLAG})
  if(${R${FLAG}})
    # 如果支持，将当前标志添加到警告编译器标志中 (If supported, add the current flag to the warning
    # compiler flags)
    set(WARNING_CXX_FLAGS "${WARNING_CXX_FLAGS} ${FLAG}")
  endif()
endforeach()

if(WIN32)
  add_definitions(-DNOMINMAX)
endif()

# 寻找依赖项 (Find dependencies)
find_package(ament_cmake_ros REQUIRED)
find_package(builtin_interfaces REQUIRED)
find_package(rcl REQUIRED)
find_package(rcpputils REQUIRED)
find_package(statistics_msgs REQUIRED)

# 添加库 (Add library)
add_library(
  ${PROJECT_NAME}
  src/libstatistics_collector/collector/collector.cpp
  src/libstatistics_collector/collector/generate_statistics_message.cpp
  src/libstatistics_collector/moving_average_statistics/moving_average.cpp
  src/libstatistics_collector/moving_average_statistics/types.cpp)

# 设置编译定义 (Set compile definitions)
target_compile_definitions(${PROJECT_NAME}
                           PRIVATE "LIBSTATISTICS_COLLECTOR_BUILDING_LIBRARY")

# 设置包含目录 (Set include directories)
target_include_directories(
  ${PROJECT_NAME}
  PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
         "$<INSTALL_INTERFACE:include/${PROJECT_NAME}>")

# 设置目标依赖 (Set target dependencies)
ament_target_dependencies(${PROJECT_NAME} "builtin_interfaces" "rcl"
                          "rcpputils" "statistics_msgs")

# 安装目标 (Install target)
install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

# 导出旧式 CMake 变量 (Export old-style CMake variables)
ament_export_include_directories("include/${PROJECT_NAME}")
ament_export_libraries(${PROJECT_NAME})

# 导出现代 CMake 目标 (Export modern CMake targets)
ament_export_targets(${PROJECT_NAME})

# 导出依赖包 (Export dependencies)
ament_export_dependencies("builtin_interfaces" "rcl" "rcpputils"
                          "rosidl_default_runtime" "statistics_msgs")

# 如果构建测试 (If building tests)
if(BUILD_TESTING)
  # 查找所需的包 (Find required packages)
  find_package(ament_lint_auto REQUIRED)
  find_package(ament_cmake_gtest REQUIRED)
  find_package(performance_test_fixture REQUIRED)
  find_package(rosidl_default_generators REQUIRED)
  find_package(std_msgs REQUIRED)

  # 自动查找测试依赖项 (Automatically find test dependencies)
  ament_lint_auto_find_test_dependencies()

  # 添加 gtest 测试 (Add gtest test)
  ament_add_gtest(test_collector test/collector/test_collector.cpp)
  # 链接库 (Link libraries)
  target_link_libraries(test_collector ${PROJECT_NAME})
  # 添加目标依赖项 (Add target dependencies)
  ament_target_dependencies(test_collector "rcpputils")

  # 添加 gtest 测试 (Add gtest test)
  ament_add_gtest(
    test_moving_average_statistics
    test/moving_average_statistics/test_moving_average_statistics.cpp)
  # 链接库 (Link libraries)
  target_link_libraries(test_moving_average_statistics ${PROJECT_NAME})
  # 添加目标依赖项 (Add target dependencies)
  ament_target_dependencies(test_moving_average_statistics "rcpputils")

  # 添加 gtest 测试 (Add gtest test)
  ament_add_gtest(
    test_received_message_period
    test/topic_statistics_collector/test_received_message_period.cpp)
  # 链接库 (Link libraries)
  target_link_libraries(test_received_message_period ${PROJECT_NAME})
  # 添加目标依赖项 (Add target dependencies)
  ament_target_dependencies(test_received_message_period "rcl" "rcpputils")

  # 添加 gtest 测试 (Add gtest test)
  ament_add_gtest(test_received_message_age
                  test/topic_statistics_collector/test_received_message_age.cpp)
  # 链接库 (Link libraries)
  target_link_libraries(test_received_message_age ${PROJECT_NAME})
  # 添加目标依赖项 (Add target dependencies)
  ament_target_dependencies(test_received_message_age "rcl" "rcpputils")

  # 生成接口 (Generate interfaces)
  rosidl_generate_interfaces(
    libstatistics_collector_test_msgs "test/msg/DummyMessage.msg"
    "test/msg/DummyCustomHeaderMessage.msg" DEPENDENCIES "std_msgs"
    SKIP_INSTALL)

  # 启用在 test_received_message_age 中使用 dummy_message.hpp 的功能 (Enable use of
  # dummy_message.hpp in test_received_message_age)
  rosidl_get_typesupport_target(
    cpp_typesupport_target libstatistics_collector_test_msgs
    "rosidl_typesupport_cpp")
  # 链接库 (Link libraries)
  target_link_libraries(test_received_message_age "${cpp_typesupport_target}")

  # 添加性能测试 (Add performance test)
  add_performance_test(benchmark_iterative
                       test/benchmark/benchmark_iterative.cpp)
  # 如果目标存在 (If the target exists)
  if(TARGET benchmark_iterative)
    # 链接库 (Link libraries)
    target_link_libraries(benchmark_iterative ${PROJECT_NAME})
  endif()
endif()

# 安装 include 目录 (Install include directory)
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})

ament_package()
