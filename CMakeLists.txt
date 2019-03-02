cmake_minimum_required(VERSION 3.13)
project(server01)
add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 14)



add_executable(server01 main.cpp threadpool.cpp threadpool.h epoll.cpp epoll.h base/ConutDownLatch.cpp base/ConutDownLatch.h base/MutexLock.h base/Condition.cpp base/Condition.h base/Thread.cpp base/Thread.h base/CurrentThread.h base/FIleUtil.cpp base/FIleUtil.h base/LogFile.cpp base/LogFile.h)
target_link_libraries(server01 pthread)
