cmake_minimum_required(VERSION 4.0)
project(ProducerConsumer)

set(CMAKE_CXX_STANDARD 20)

# Producer executable
add_executable(producer producer.cpp)
target_link_libraries(producer pthread rt)

# Consumer executable
add_executable(consumer consumer.cpp)
target_link_libraries(consumer pthread rt)

# Cleanup executable
add_executable(cleanup cleanup.cpp)
target_link_libraries(cleanup pthread rt)