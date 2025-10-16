cmake_minimum_required(VERSION 4.0)
project(ProducerConsumer)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProducerConsumer main.cpp
        producer.cpp
        producer.h
        consumer.cpp
        consumer.h
        table.cpp
        table.h)
