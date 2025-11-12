cmake_minimum_required(VERSION 3.15)
project(simple_xframework)

set(CMAKE_CXX_STANDARD 14)

find_package(X11 REQUIRED)

add_executable(simple_xframework
        System/Debug.h
        System/EventHandlers.h
        System/Form.h
        System/Object.h
        System/System.h
        System/Types.h
        teste.cpp)

target_link_libraries(simple_xframework PRIVATE ${X11_LIBRARIES})
target_include_directories(simple_xframework PRIVATE ${X11_INCLUDE_DIRS})